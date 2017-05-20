#include <stdio.h>
#include <ctime>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>

//cv::VideoCapture cam(0); // 0 - default camera
raspicam::RaspiCam_Cv cam;

int main(int argc, char** argv )
{
    cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    //cam.set(CV_CAP_PROP_EXPOSURE, 1+i%30);
    //cam.set(CV_CAP_PROP_EXPOSURE, 100); // 100 - the highest shutter speed

     if(!cam.open()) {
        std::cerr << "Can't access the camera!\n";
        return -1;
    }

    cv::Mat frame;
    cam.grab();
    cam.retrieve (frame);
    int frameSize = frame.total() * frame.elemSize();
    
    

    cam.release();

    return 0;
}
