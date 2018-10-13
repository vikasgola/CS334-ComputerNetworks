#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

// get socket IPA and port number and print to standard output
void displayIPAandPort(int sockfd){
    struct sockaddr_in addr;
    socklen_t size;

    // get ipa and port number from socket
    if(getsockname(sockfd,(struct sockaddr*)&addr,&size) == -1){
        perror("Error in getting sockname!!!\n");
    }

    // prints ipa and port number to standard ouput
    printf("IP Address: %s\nPort Number: %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

int main(int argc, char** argv){
    // define a socket desciptor
    int sockfd;

    // intializing the socket descriptor
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("Error in creating socket!!!\n");
    }

    // call displayIPAandPort function
    displayIPAandPort(sockfd);

    return 0;
}