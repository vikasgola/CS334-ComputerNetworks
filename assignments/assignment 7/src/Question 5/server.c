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
#define MAXLINE         4096
#define LISTENQ         10

void str_echo(int connfd)
{
    int	n;
	char recvline[MAXLINE];
    printf("Ready.\n");
    for(;;)
    { 
        recvline[0] = '\0';
        if ( (n = read(connfd, recvline, MAXLINE)) == 0)	/* connection closed by other end */
        {
            close(connfd);
            printf("Connection closed.\n");
            return;
        }else{
            write(connfd, recvline, sizeof(recvline));
        }
    }
}

int main(int argc, char **argv)
{
	int				    sockfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
        printf("error in bind call\n");
        close(sockfd);
        exit(-1);
    }
	
	if ((listen(sockfd, LISTENQ)) == -1)
    {
        close(sockfd);
        perror("error in listen call\n");
        exit(-1);
    }
    printf("Server Running on port:%d\n",SERVPORT);
;
    clilen = sizeof(cliaddr);
	if ((connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen)) == -1) 
    {
        close(sockfd);
        perror("error in accepting connection\n");
        exit(-1);
    }
	str_echo(connfd);	/* process the request */
    close(connfd);
    close(sockfd);
    return 0;
}
