
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
#include <netdb.h>
  
#define MAXLINE 1024
#define ECHOLEN 64

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

long delay(struct timeval t1, struct timeval t2){
	long d;
	d = (t2.tv_sec - t1.tv_sec) * 1000000;
	d += t2.tv_usec - t1.tv_usec;
	return(d);
}

  
int main(int argc, char**argv) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "0123456789\n"; 
    int PORT;
    char*IP;
    struct sockaddr_in     servaddr; 
    struct timeval start, end;
    
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    if(argc != 3){
        printf("Usage: ./client ip/hostname port_number\n");
        exit(EXIT_FAILURE);
    }
    
    PORT = atoi(argv[2]);
    IP = nameToIP(argv[1]);
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, IP, &servaddr.sin_addr); 
    
      
    int n, len; 
    
    for(int i=0;i<10;i++){
        gettimeofday(&start, NULL);
        if(sendto(sockfd, (const char *)hello, strlen(hello), 
            MSG_CONFIRM, (const struct sockaddr *) &servaddr,
                sizeof(servaddr)) < 0){
            printf("error in sending hello to server");
            exit(0);
         } 
        fd_set socks;
        struct timeval t;
        FD_ZERO(&socks);
        FD_SET(sockfd, &socks);
        t.tv_sec = 1;
        if(select(sockfd + 1, &socks, NULL, NULL, &t) && ((n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, (struct sockaddr *) &servaddr, 
                    &len)) != -1)){            
            gettimeofday(&end, NULL); /* end delay measurement */
	        printf("RTT = %ld microsecond.\n", delay(start, end));
            
        }else{
        printf("Packet Lost\n");
            continue;
        }
	}
  
    close(sockfd); 
    return 0; 
}
