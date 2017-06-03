#include <unistd.h>
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

    //cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);

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

    char *hello = "Hello from door ";
    for (int i = 0; i < 10; i++) {
        send(stream_socket, hello , strlen(hello), 0 );
    
        int numbytes = 0;
        char buf[30];
        numbytes = recv(stream_socket,buf,30,0);
        if(numbytes == -1)
        {
            fprintf(stderr, "Error receive \n");
            exit(4);
        }
        buf[numbytes] = '\0';
        printf("Received %s \n", buf);

    }      

    
    int bytes = 0;
    int key;

    //while(1)
    //{
        cam.grab();
        cam.retrieve(frame);
        //DEBUG
        printf("stream socket:%d\nFrame size: %d\n",stream_socket, frameSize);
        //DEBUG
        //if ((bytes = send(stream_socket, frame.data, frameSize, 0)) < 0){
           //std::cerr << "bytes = " << bytes << std::endl;
           //break;
        //} 
        printf("Sent\n");
    //}    

    cam.release();
    close(stream_socket);
    return 0;
}
