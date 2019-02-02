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

// port number of server(myself)
#define PORT 3993

int main(int argc, char **argv){
    int listenfd, connfd;
    socklen_t len;

    struct sockaddr_in serveraddr,clientaddr;
    char hellomsg[100]="Hello, World\n";
    time_t ticks;

    // creating the socket descriptor
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error in creating socket!!!\n");
        return 0;
    }

    // zero all bytes in serveraddr
    bzero(&serveraddr, sizeof(serveraddr));

    // fill servereraddr with family type, port number and IP address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);

    // bind to serveraddr
    if(bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
        printf("Error in binding IP address with socket!!!\n");
        return 0;
    }

    listen(listenfd, 100);
    printf("Server Running on port:%d\n",PORT);


    // keep listening for requests iteratively
    for(;;){
        // accept for any connection request and get socket desciptor for that
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &len);
        // send Hello World to client using connected socket descriptor
        write(connfd, hellomsg, strlen(hellomsg));
        // close connected descriptor
        close(connfd);
    }

    // close listening socket descriptor
    close(listenfd);
    exit(0);
}
