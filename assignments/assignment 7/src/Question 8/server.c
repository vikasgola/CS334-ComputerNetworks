#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVPORT        9923
#define MAXLINE         80
#define LISTENQ         10

int fsize(FILE *fp) // to check file size
{
    int prev  =ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fseek(fp, prev, SEEK_SET); //go back to where we were
    return size;
}


void file_handle(int connfd)   // for file handeling
{
    FILE *fp;
    char rcvline[128], fileread[1024];
    while(1)
    {
        int n = recv(connfd, rcvline, 100, 0);  // Got file name
        rcvline[n] = '\0';
        fp = fopen(rcvline, "rb"); //open in read mode
        if(fp == NULL){
            send(connfd, "error in sending message\n", 5, 0);
        }else{
            if (fsize(fp)<1024) //check if size of file is less than 1KB
            {
                printf("File open sucessful, Sending the file to client.\n");
                while(fgets(fileread, sizeof(fileread), fp))  // file data is now in fileread buffer
                {
                    if(send(connfd, fileread, sizeof(fileread), 0)<0) // Send data to socket
                    {
                        printf("Cant send\n");
                    }
                    else
                    {
                        send(connfd,"completed",sizeof("completed"),0);   
                    }
                    fclose(fp);
                }
            }else{
                printf("Requested file size greater than 1 KB\n");
                send(connfd, "size_error", sizeof("size_error"), 0);
                fclose(fp);
            }
        }
    }
}

int main()
{
    
    char ch,file[100];
    int				    sockfd, connfd;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
		printf("error in creating socket\n"); 
		exit(-1);
	}
    //printf("Socket created\n");
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERVPORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        printf("error in bind\n");
        close(sockfd);
        exit(-1);
    }
	//printf("socket binded sucessfull\n");

	if ((listen(sockfd, LISTENQ)) == -1)
    {
        close(sockfd);
        perror("error in listen\n");
        exit(-1);
    }
	printf("Server Running on port:%d\n",SERVPORT);

		clilen = sizeof(cliaddr);
		if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) == -1) 
        {
            close(sockfd);
            perror("error in accepting connection\n");
            exit(-1);
        }
 		printf("Connection accepted\n");

        file_handle(connfd);
       
    return(0);
}
