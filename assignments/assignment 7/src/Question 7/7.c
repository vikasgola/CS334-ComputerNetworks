#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

static char *ethernet_mactoa(struct sockaddr *addr) {
    static char buff[256];
    unsigned char *ptr = (unsigned char *) addr->sa_data;

    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
        (ptr[0] & 0xff), (ptr[1] & 0xff), (ptr[2] & 0xff),
        (ptr[3] & 0xff), (ptr[4] & 0xff), (ptr[5] & 0xff));

    return (buff);
}


int main(int argc, char *argv[]) {
    int s;
    struct arpreq areq;
    struct sockaddr_in *sin;
    struct in_addr ipaddr;

    if (argc != 3) {
        fprintf(stderr, "-- Usage: %s device ipaddress\n", argv[0]);
        exit(1);
    }

    /* Get an internet domain socket. */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }
    
    inet_aton(argv[2], (struct in_addr*)&ipaddr.s_addr);


    /* Make the ARP request. */
    memset(&areq, 0, sizeof(areq));
    sin = (struct sockaddr_in *) &areq.arp_pa;
    sin->sin_family = AF_INET;

    sin->sin_addr = ipaddr;
    sin = (struct sockaddr_in *) &areq.arp_ha;
    sin->sin_family = ARPHRD_ETHER;

    strncpy(areq.arp_dev, argv[1], 15);

    if (ioctl(s, SIOCGARP, &areq) == -1) {
        perror("-- Error: unable to make ARP request, error");
        exit(1);
    }

    printf("%s -> %s\n",
    inet_ntoa(((struct sockaddr_in *) &areq.arp_pa)->sin_addr),
    ethernet_mactoa(&areq.arp_ha));

    return 0;
}
