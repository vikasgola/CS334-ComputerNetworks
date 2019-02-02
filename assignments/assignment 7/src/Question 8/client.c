#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<netdb.h>
#include	<stdio.h>
#include	<errno.h>
#include 	<string.h>
#include 	<stdlib.h>
#include 	<arpa/inet.h>
#include 	<unistd.h>

#define SERV_PORT       9923
#define MAXLINE         80 

void file_handle(int sockfd)
{
    char name[128],fname[128], rcvdata[1024];
    FILE *fp;
    int s;
    do
    {
        printf("Enter the file name to download: ");
        scanf("%s", name);
        send(sockfd, name, sizeof(name), 0); //send filename
        printf("Enter the new file name: ");
        scanf("%s", fname);
        fp = fopen(fname, "w"); // New file created
        
        while(1)
        {
            rcvdata[0] = '\0';
            s=recv(sockfd, rcvdata, sizeof(rcvdata), 0); //data received
            rcvdata[s] = '\0';
            if(strcmp(rcvdata, "error") == 0)
            {
                printf("File is not available\n");
                fclose(fp);
                break;
            }else if(strcmp(rcvdata, "size_error") == 0){
                printf("File is greater than 1 KB\n");
                fclose(fp);
                break;
            }else{
                if(strcmp(rcvdata, "completed") != 0)
                {
                    fputs(rcvdata, stdout);
                    printf("\n");
                    fprintf(fp,"%s", rcvdata);
                }
                recv(sockfd, rcvdata, sizeof(rcvdata), 0);
                
                if(strcmp(rcvdata, "completed") == 0)
                {
                    printf("File is transferred\n");
                    printf("\n");
                    fclose(fp);
                    break;
                } 
            }
        }       
    }
    while(1);
    close(sockfd);    
}

int main(int argc, char **argv)
{
    int                     sockfd;
    struct sockaddr_in      servaddr;
    if (argc != 2)
    {
		fprintf(stderr, "usage: %s <IPaddress>\n", argv[0]);
        exit(-1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("error in socket\n");
    }
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) 
	{
		perror("error in connecting to server\n");
        close(sockfd);
        exit(-1);
    }
    file_handle(sockfd);
    close(sockfd);
    return(0);
}
