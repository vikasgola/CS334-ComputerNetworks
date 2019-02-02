#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
  
#define PORT 3993
#define MAXLINE 1024 
  
 
int main(int argc, char**argv) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    char *hello = "Hello, World"; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed:"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    
    servaddr.sin_family    = AF_INET;  
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ){ 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Server Started on port:%d\n",PORT);

    
    for(;;){  
        int len, n; 
        if((n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len) < 0)){
            printf("Error in getting msg from client");
            continue;
        } 
        buffer[n] = '\0'; 
        
        if(sendto(sockfd, (const char *)hello, strlen(hello),  
            MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
                len) < 0){
            printf("Error in sending msg to client");       
        } 
        printf("Message sent.\n");  
    }
    return 0; 
}
