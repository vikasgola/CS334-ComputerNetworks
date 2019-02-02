
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

  
#define MAXLINE 1024

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

  
int main(int argc, char**argv) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "connect me\n"; 
    int PORT;
    char*IP;
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
    
    if(argc != 3){
        printf("Usage: ./client ip/hostname port_number/helloservice\n");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[2], "helloservice") == 0){
        PORT = 3993;
    }else{
        PORT = atoi(argv[2]);
    }
    IP = nameToIP(argv[1]);
  
    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    inet_pton(AF_INET, IP, &servaddr.sin_addr);     
    
    int n, len; 
      
    if(sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr)) < 0){
        printf("error in sending hello to server");
        exit(0);
     } 
          
    if((n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len)) < 0){
        printf("error in getting msg from server");
        exit(0);
    }
    buffer[n] = '\0'; 
    printf("%s\n", buffer); 
  
    close(sockfd); 
    return 0; 
}
