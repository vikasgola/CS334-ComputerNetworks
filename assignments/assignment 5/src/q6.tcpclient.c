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

// port number of server
#define PORT 9923


void getTime(int sockfd, FILE *fd){
    char recv[1000];
    int nreadbyte = 0;
    // reading 1000 bytes from sockfd
    while((nreadbyte = read(sockfd, recv, 1000)) > 0){
        recv[nreadbyte] = 0;
        // showing the output
        fputs(recv, stdout);
    }
}

void ipToName(char *ipstr){
    struct hostent *host;
    struct in_addr ip;

    // filling structure in_addr from string ipstr
    if(inet_aton(ipstr,&ip) ==-1){
        perror("Error ascii to network conversion!!!\n");
        exit(0);
    }
    
    // filling struture host of type hostent
    if( (host = gethostbyaddr(&ip,sizeof(struct in_addr),AF_INET)) == NULL ){
        printf("Error getting host name of %s\n",ipstr);
        return;
    }

    char **names = host->h_aliases;

    // printing name of host
    printf("HOSTNAME: %s\n",host->h_name);
}


int main(int argc, char **argv){
    int sockfd;
    struct sockaddr_in serveraddr;

    if(argc != 2){
        printf("Use: ./q5.client 127.0.0.1");
        return 0;
    }

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
    inet_pton(AF_INET, argv[1], &serveraddr.sin_addr);

    // connect to server with sockfd
    if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1){
        printf("Error connecting to server. Try Again!!!\n");
        return 0;
    }

    // print Hostname of server
    ipToName(argv[1]);

    // print port number of server
    printf("PORT NO.: %d\n",PORT);

    // get time from server and print
    getTime(sockfd, stdout);

    // closing the socket descriptor
    close(sockfd);
    return 0;
}