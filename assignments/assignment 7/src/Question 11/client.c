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


// get name of host and print IP address of host
char* nameToIP(char *name){
    struct hostent *host;
    
    // get host details in hostent structure
    if( (host = gethostbyname(name)) == NULL){
        printf("Error getting host IP of %s!!!\n",name);
        exit(EXIT_FAILURE);
    }

    // string list to in_addr list
    struct in_addr **addr = (struct in_addr **)host->h_addr_list;

    // printing all ip addresses of host 
    // for(int i=0;addr[i] != NULL;i++){
    //     printf("%s\n",inet_ntoa(*addr[i]));
    // }
    return inet_ntoa(*addr[0]);
}

void sendHTTPrequest(int sockfd,char *host,char* request_type, char* request_file){
    char recv[1000],request[1000];
    int nreadbyte = 0;

    snprintf(request, sizeof(request), "%s %s HTTP/1.1\r\nHOST: %s\r\n\r\n", request_type,request_file,host);
    // writing "Hello Server" to sockfd
    if (send(sockfd, request, sizeof(request), 0) < 0) {
        perror("send");
        return;
    }
    printf("Request Sended\nGetting Response...\n");

    // reading 1000 bytes from sockfd
    while((nreadbyte = read(sockfd, recv, 1000)) > 0){
        recv[nreadbyte] = 0;
        // showing the output
        fputs(recv, stdout);
    }
}

int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in serveraddr;
    int PORT;

    if(argc != 5){
        printf("Use: ./client ip/hostname port GET filename\n");
        return 0;
    }

    // creating the socket descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error in creating socket!!!\n");
        return 0;
    }
    
    PORT = atoi(argv[2]);

    // zero all bytes in serveraddr
    bzero(&serveraddr, sizeof(serveraddr));

    // fill serveraddr with family type, port number and IP address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_pton(AF_INET, nameToIP(argv[1]), &serveraddr.sin_addr);

    // connect to server with sockfd
    if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
        printf("Error connecting to server. Try Again!!!\n");
        return 0;
    }

    // get time from server and print
    sendHTTPrequest(sockfd,argv[1],argv[3],argv[4]);

    // closing the socket descriptor
    close(sockfd);
    return 0;
}
