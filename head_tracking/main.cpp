#include "Controller.h"
#include <iostream>
#include <thread>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main()
{
    cout << "Starting program...\n";

    Controller controller;

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {
        cout << "Could not open camera" << endl;
        return 1;
    }

    cout << "Camera opened" << endl;

    for(;;)
    {
        Mat frame, img;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, img, COLOR_BGR2YUV);
		YUVImage yuvImg = YUVImage(img.data, img.cols, img.rows);
        controller.ProcessCameraFrame(yuvImg);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}