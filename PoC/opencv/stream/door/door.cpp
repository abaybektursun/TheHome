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
    int   stream_socket, server_port;
    char* server_ip;
    if (argc < 3) {
           std::cerr << "Please provide server IP and socket" << std::endl;
    }
    server_ip   = argv[1];
    server_port = atoi(argv[2]);
    struct  sockaddr_in server_addr;
    socklen_t ip_size = sizeof(struct sockaddr_in);
    if ((stream_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error creating a socket!\n" << std::endl;
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    if (connect(stream_socket, (sockaddr*)&server_addr, ip_size) < 0) {
        std::cerr << "Failed to connect to the server!\n" << std::endl;
    }

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
    
    if (!frame.isContinuous()) {
        frame = frame.clone();
    }
    
    int bytes = 0;
    int key;

    while(1)
    {
        cam.grab();
        cam.retrieve(frame);
        if ((bytes = send(stream_socket, frame.data, frameSize, 0)) < 0){
           std::cerr << "bytes = " << bytes << std::endl;
           break;
        } 
        printf("Sent\n");
    }    
    

    cam.release();

    return 0;
}
