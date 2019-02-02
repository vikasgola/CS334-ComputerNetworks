#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int printIP(const char *host) {
    struct addrinfo hints, *hostinfos;
    int errcode;
    char addrstr[100];
    void *ptr;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    if(getaddrinfo(host, NULL, &hints, &hostinfos) != 0) {
        printf("Error in getting info of address\n");
        return -1;
    }

    printf("Host: %s\n", host);
    while(hostinfos) {
        ptr = &((struct sockaddr_in *)hostinfos->ai_addr)->sin_addr;

        inet_ntop(hostinfos->ai_family, ptr, addrstr, 100);
        printf("IPv%d address: %s (%s)\n", hostinfos->ai_family == PF_INET6 ? 6 : 4,
               addrstr, hostinfos->ai_canonname);
        hostinfos = hostinfos->ai_next;
    }

    printf("\n");
    return 0;
}

int main(int argc, char **argv) {
    if(argc < 2){
        printf("Usage: ./1 hostname1 hostname2 ...\n");
        return EXIT_FAILURE;
    }

    printf("\n");
    for(int i=1;i<argc;i++){
        printIP(argv[i]);
    }

    return 0;
}
