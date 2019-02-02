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
#define MAXLINE         4096                 

void str_cli(FILE *fp, int sockfd)
{
	char	sendline[MAXLINE], recvline[MAXLINE];
	int r;
	recvline[MAXLINE] = '\0';
	sendline[MAXLINE] = '\0';
	do
	{
        recvline[0] = '\0';
	    sendline[0] = '\0';
        printf("Client: ");
		if ((fgets(sendline, MAXLINE, fp)) != 0)  
        {
            write(sockfd, sendline, sizeof(sendline));
        }
		
		if((r = read(sockfd, recvline, MAXLINE)) > 0) 
        {
            printf("Server: ");
            fputs(recvline, stdout);
        }
	}
    while(1);
    close(sockfd);
}


int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

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
    str_cli(stdin, sockfd);		/* do it all */
	close(sockfd);
	exit(0);
}
