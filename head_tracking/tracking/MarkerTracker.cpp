#include "MarkerTracker.h"
#include <algorithm>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

bool sizesort(Feature a, Feature b) { return a.size > b.size; }

MarkerTracker::MarkerTracker(Calibrator & calibrator, vector<Vec3f> model)
	: calibrator(calibrator), model(model)
{
}

Vector3f MarkerTracker::FindLocation(const YUVImage & image)
{
	vector<Feature> features = TrackFeatures(image);

	// Make sure points were found, if not, return NAN vector
	if (features.size() == 0)
		return Vector3f(NAN, NAN, NAN);
	
	// Prep feature points (Convert points to Point2f vector)
	vector<Point2f> features2d(model.size());
	for (int i = 0; i < model.size(); i++)
	{
		features2d[i] = Point2f(features[i].x, features[i].y);
	}
	
	Calibrator::Constants c = calibrator.GetConstants();

	// Get model relative to camera location
	Mat rvec_mc, tvec_mc;
	if (!solvePnP(model, features2d, c.camMat, c.distCoeffs, rvec_mc, tvec_mc, false, CV_P3P))
		return Vector3f(NAN, NAN, NAN); // Could not solve, don't update

	// We get use rvec_mc + tvec_mc to represent the center point of the model into camera space.
	// Now we need to apply the c.rvec_cw and c.tvec_cw to go from model to camera to world space
	// Hence, model to world space. 
	Mat rvec_mw, tvec_mw;
	composeRT(rvec_mc, tvec_mc, c.rvec_cw, c.tvec_cw, rvec_mw, tvec_mw);

	// Get locaton vector from tvec Mat
	Vector3f loc; 
	loc = Vector3f(
		(float) tvec_mw.at<double>(0),
		(float) tvec_mw.at<double>(1),
		(float) tvec_mw.at<double>(2)
	);

	return loc;
}

vector<Feature> MarkerTracker::TrackFeatures(const YUVImage & image)
{
	static Rectangle region = Rectangle(0, 0, image.width, image.height); 
	vector<Feature> features;

	// Look for features in a growing region
	do
	{
		// Grow image rectangle by 2 * area
		// Note: this should also happen the first time since the region is set to 
		// the minimum region to contain the previous track points.
		region = Rectangle(0, 0, image.width, image.height); // TODO:

		// Limit region to image size
		region = Rectangle(min(max(0, region.x), image.width - 1),
			min(max(0, region.y), image.height - 1),
			min(region.width, image.width),
			min(region.height, image.height));
		
		// Find the features
		features = ff.FindFeatures(image, region);
	}
	// When we have found all the features, or we have checked the entire image we are done
	while (features.size() < model.size() 
		&& region != Rectangle(0, 0, image.width, image.height));

	// If we couldn't find enough features and we checked the entire image, no features found :(
	if (features.size() < model.size())
		return vector<Feature>();

	// Take the largest sized features
	sort(features.begin(), features.end(), sizesort);

	// Set rectangle (place to look) for next time
	Rectangle nextReg = Rectangle((int)features[0].x, (int)features[0].y, 0, 0);
	for (auto i = features.begin() + 1; i != features.end(); i++)
	{
		// Set min coordionates
		if (i->x < nextReg.x)
		{
			nextReg.width += (int) (nextReg.x - i->x);
			nextReg.x = (int) i->x;
		}
		if (i->y < nextReg.y)
		{
			nextReg.height += (int) (nextReg.y - i->y);
			nextReg.y = (int) i->y;
		}
		
		// Update width and height
		if (i->x - nextReg.x > nextReg.width)
			nextReg.width = (int)(i->x - nextReg.x);
		if (i->y - nextReg.y > nextReg.height)
			nextReg.height = (int)(i->y - nextReg.y);
	}

	// set region for next time
	region = nextReg;

	return features;
}