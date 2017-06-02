#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#define MAX_CONNECTIONS 1

void *receive(void *s)
{
    int socket = *(int *)s;
    cv::Mat frame;
    frame = cv::Mat::zeros(2592, 1944, CV_8UC3);
    //int frameSize = frame.total() * frame.elemSize();
    int frameSize = 3686400;
    uchar *iptr = frame.data;
    int bytes = 0;
    int key;
    
    if ( ! frame.isContinuous() ) { 
          frame = frame.clone();
    }

    int frameSize2 = frameSize*2;
    if (setsockopt(socket, SOL_SOCKET, SO_RCVBUF, &frameSize2, sizeof(int)) == -1) {
        printf("Error increasing limit!\n");
    }
    
    printf("Starting Transaction\n");
    if ((bytes = recv(socket, iptr, frameSize, MSG_WAITALL)) == -1) {
            std::cerr << "recv failed, received bytes = " << bytes << std::endl;
    }

    cv::imwrite("test.jpg", frame);
    
    printf("Done with thread\n");
    pthread_exit(NULL);
}

int main(int argc, char** argv )
{
    int serverSocket,
        doorSocket,
        serverPort = 2658;
    
    struct sockaddr_in server_ip, client_ip;

    pthread_t threads[MAX_CONNECTIONS];
    
    int ip_size = sizeof(struct sockaddr_in);
    
    // Create unbinded socket
    serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if (serverSocket == -1){
        std::cerr << "Unable to create server socket\n";
        exit(-1);
    }
    
    //AF_UNIX    : UNIX domain protocols
    //AF_INET    : Internet domain protocols
    //SOCK_STREAM: TCP protocol
    //SOCK_DGRAM : UDP protocol
    //SOCK_RAW   : IP protocol
    
    server_ip.sin_addr.s_addr = INADDR_ANY;
    server_ip.sin_family = AF_INET;
    server_ip.sin_port = htons(serverPort);
    
    //Assign a name (port, etc.) to the socket
    if( bind(serverSocket,(struct sockaddr *)&server_ip , sizeof(server_ip)) < 0) {
         perror("Failed to bind the socket");
         exit(1);
    }
    
    int client_idx = 0;
    // Start the server
    listen(serverSocket , 3);

    while(1){
        doorSocket = accept(serverSocket, (struct sockaddr *)&client_ip, (socklen_t*)&ip_size);
        if (doorSocket < 0) {
            perror("FAIL CONNECTING\n");
            exit(1);
        } 
        pthread_create(&(threads[client_idx]),NULL,receive,&doorSocket);
    }
    
    
    return 0;
}
