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

void helloService(int sockfd, FILE *fd){
    char recv[1000];
    int nreadbyte = 0;

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
    
    if(argc != 3){
        printf("Use: ./client ipaddress port_number/helloservice\n");
        return 0;
    }
    
    if(strcmp(argv[2], "helloservice") == 0){
        PORT = 3993;
    }else{
        PORT = atoi(argv[2]);
    }
    
    char* IP = nameToIP(argv[1]);

    // creating the socket descriptor
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("Error in creating socket!!!\n");
        return 0;
    }

    // zero all bytes in serveraddr
    bzero(&serveraddr, sizeof(serveraddr));

    // fill serveraddr with family type, port number and IP address
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &serveraddr.sin_addr);

    // connect to server with sockfd
    if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
        printf("Error connecting to server. Try Again!!!\n");
        return 0;
    }

    // get time from server and print
    helloService(sockfd, stdout);

    // closing the socket descriptor
    close(sockfd);
    return 0;
}
