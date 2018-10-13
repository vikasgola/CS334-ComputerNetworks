#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


// server on port number 9923
#define PORT 9923

int main(int argc, char **argv){

    int sockfd, connfd;
    char *request = "please send me time!";
    struct sockaddr_in serveraddr, clientaddr;
    char buffer[1000];

    // create a socket descriptor
    if((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1){
        printf("Error in creating socket descriptor!!!\n");
        return 0;
    }

    // filling server address with server adderss and port
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);


    // sending hello message to server
    int nreadbytes,len;
    if(sendto(sockfd, request, strlen(request), MSG_CONFIRM, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0){
        printf("Error in sendding hello to client!!!\n");
        return 0;
    }
    printf("Hello message sent.\n");

    // get time from server
    if((nreadbytes = recvfrom(sockfd,(char *)buffer, 1000, MSG_WAITALL, (struct sockaddr *) &serveraddr, &len)) < 0){
        printf("Error in getting time to client!!!\n");
        return 0;
    }
    buffer[nreadbytes] = '\0';

    printf("Time: %s",buffer);

    // closing socket descriptor
    close(sockfd);
    return 0;
}
