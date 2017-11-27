#include "Calibrator.h"
#include "utility/Quarternion.h"
#include "utility/Checkerboard.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

Calibrator::Calibrator()
{
	Reset();
}

void Calibrator::Reset()
{
	cout << "Reset calibration\n";
	isCalibrated = false;
	count = 0;
	calImgPoints.clear();
	calObjPoints.clear();
}

bool Calibrator::Add(YUVImage img, Transformation t, float checkerSize, int checkerRows, int checkerCols)
{
	cout << "Adding calibration -(" << count << "):\n";
	cout << "\tCheckerboard: " << checkerRows << "x" << checkerCols << "\n";
	cout << "\tCheckerboard size: " << checkerSize << "\n";
	cout << "\tPosition: (" << t.loc.x << ", " << t.loc.y << ", " << t.loc.z << ")\n";
	cout << "\tQuaternion: (" << t.rot.r << ", " << t.rot.i << ", " << t.rot.j << ", " << t.rot.k << ")\n";

	Size imgSize = Size(img.width, img.height);
	Checkerboard cb = Checkerboard(checkerSize, checkerRows, checkerCols);
	vector<Point2f> imgCorners = GetCheckerCornersFromImage(img, cb);

	// Make sure we found the checkerboard
	if (imgCorners.size() == 0)
	{
		cout << "\tFailed! Could not find Checkerboard.\n";
		return false;
	}
	else
	{
		cout << "\tSuccess:\n";
	}

	// On the first calibration, get camera intristic values using calibrateCamera
	// Rest of them (and first) get extrinstic using solvePnP
	double intrinsicErr;
	if (!isCalibrated)
	{
		intrinsicErr = CalibrateIntrinsicParameters(cb.GetInnerCorners(), imgCorners, imgSize);
		cout << "\t\tIntrinsic Error: " << intrinsicErr << "\n";
	}

	// Take the flat, untransformated checkboard, and rotate it into 3d space
	cb.ApplyTransformation(t);
	vector<Point3f> objCorners = cb.GetInnerCorners();

	// Add new calibrations points
	// Note these are both inner corners
	calObjPoints.insert(calObjPoints.begin(), objCorners.begin(), objCorners.end());
	calImgPoints.insert(calImgPoints.end(), imgCorners.begin(), imgCorners.end());

	double extrinsicErr;
	extrinsicErr = CalibrateExtrinsicParameters(calObjPoints, calImgPoints);
	cout << "\t\tExtrinsic Error: " << extrinsicErr << "\n";

	// We are now calibrated
	isCalibrated = true;
	count++;
	return true;
}

Calibrator::Constants Calibrator::GetConstants()
{
	Constants c;
	lock_guard<mutex> lock(constant_m);
	c = constants;
	return c;
}

double Calibrator::GetReprojectionError()
{
	int totalPoints = 0;
	double totalErr = 0, err;
	Mat imagePoints2;

	Mat rvec_wc, tvec_wc;
	InvertRT(constants.rvec_cw, constants.tvec_cw, rvec_wc, tvec_wc);

	projectPoints(calObjPoints, rvec_wc, tvec_wc, constants.camMat,  // project
		constants.distCoeffs, imagePoints2);
	err = norm(Mat(calImgPoints), imagePoints2, CV_L2);              // difference
	int n = (int)calObjPoints.size();
	totalErr += err*err;                                             // sum it up
	totalPoints += n;

	return sqrt(totalErr / totalPoints);              // calculate the arithmetical mean
}

void Calibrator::InvertRT(InputArray rvec, InputArray tvec, OutputArray rvec_i, OutputArray tvec_i)
{
	tvec_i.create(tvec.size(), tvec.type());
	Mat tvec_i_m = tvec_i.getMat();

	Mat R;
	Rodrigues(rvec, R);
	R = R.t();
	Rodrigues(R, rvec_i);

	tvec_i_m = -R * tvec.getMat();
}

double Calibrator::CalibrateIntrinsicParameters(vector<Point3f> objPoints, vector<Point2f> imgPoints, Size imgSize)
{
	vector<Mat> rvecs, tvecs; // Unused

	vector<vector<Point3f>> wrappedObjPoints = { objPoints };
	vector<vector<Point2f>> wrappedImgPoints = { imgPoints };
	lock_guard<mutex> lock(constant_m);
	double err = calibrateCamera(wrappedObjPoints, wrappedImgPoints, imgSize, 
		constants.camMat, constants.distCoeffs, rvecs, tvecs);
	Vector3f calPos = Vector3f(
		(float)tvecs[0].at<double>(0),
		(float)tvecs[0].at<double>(1),
		(float)tvecs[0].at<double>(2)
	);

	Mat r, t;
	InvertRT(rvecs[0], tvecs[0], r, t);

	return err;
}

double Calibrator::CalibrateExtrinsicParameters(vector<Point3f> objPoints, vector<Point2f> imgPoints)
{
	// Ransac method might be the best considering we have a lot of points
	// This will get us rvecs going from world (object) to camera space,
	Mat rvec_wc, tvec_wc;
	lock_guard<mutex> lock(constant_m);
	solvePnPRansac(objPoints, imgPoints, constants.camMat, constants.distCoeffs, rvec_wc, tvec_wc);

	// Invert to go from camera to world space
	InvertRT(rvec_wc, tvec_wc, constants.rvec_cw, constants.tvec_cw);

	constants.reprojectionErr = GetReprojectionError();

	return constants.reprojectionErr;
}

vector<Point2f> Calibrator::GetCheckerCornersFromImage(YUVImage img, Checkerboard cb)
{
	// Get img Mat
	Size imgSize = Size(img.width, img.height);
	Mat checkImg = Mat(imgSize, CV_8UC3, img.data);

	// Make grayscale
	extractChannel(checkImg, checkImg, 0); // Don't know if this is needed, but we'll do it anyway

	vector<Point2f> corners;
	bool foundCorners = findChessboardCorners(checkImg, Size(cb.cols - 1, cb.rows - 1), corners, CALIB_CB_ADAPTIVE_THRESH);
	if (!foundCorners)
		return vector<Point2f>();
	
	// Refine 
	CvTermCriteria subPixCrit;
	subPixCrit.type = CV_TERMCRIT_ITER;
	subPixCrit.max_iter = 1000;
	cornerSubPix(checkImg, corners, Size(11, 11), Size(-1, -1), subPixCrit);

	return corners;
}


