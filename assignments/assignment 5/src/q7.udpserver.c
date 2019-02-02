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
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind to serveraddr
    if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
        printf("Error in binding IP address with socket!!!\n");
        return 0;
    }

    // complete all request one by one
    for(;;){
        // read bytes sent by client
        int nreadbytes,len;
        if((nreadbytes = recvfrom(sockfd,(char *)buffer, 1000 , MSG_WAITALL, (struct sockaddr *) &clientaddr, &len)) <= 0){
            printf("Error in sendding time to client!!!\n");
            return 0;
        }
        buffer[nreadbytes] = '\0';

        // showing received message from client
        printf("Request Received: %s\n",buffer);


        // get time in buffer character array
        time_t ticks;
        ticks = time(NULL);
        snprintf(buffer, sizeof(buffer), "%.24s\r\n", ctime(&ticks));

        // sending time to client 
        if(sendto(sockfd, (char *)buffer, 1000, MSG_CONFIRM, (const struct sockaddr *) &clientaddr, len) <= 0){
            printf("Error in sendding time to client!!!\n");
            return 0;
        }

        printf("Request Completed.\n");
    }

    // closing socket descriptor
    close(sockfd);
    return 0;
}
