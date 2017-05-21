#include <stdio.h>
#include <pthread.h>
#include <ctime>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#define MAX_CONNECTIONS 1

int main(int argc, char** argv )
{
    int serverSocket,
        doorSocket,
        serverPort = 2654;
    
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
    
    // Start the server
    listen(serverSocket , 3);
    
    
    
    return 0;
}
