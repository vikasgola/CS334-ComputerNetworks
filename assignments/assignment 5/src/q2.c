#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include <arpa/inet.h>

// function to populate the IP address to sin_addr in variable of type struct sockaddr_in.
void populateSockAddr(struct sockaddr_in *addr){
    printf("Input your IP address: ");
    // storing input in array of char of 32 size
    char ip[32];
    // scaned the input from stdin
    scanf("%s",ip);

    // populating the "addr" with input IP address
    if(inet_aton((const char*)ip, &(addr->sin_addr)) == -1){
        perror("Error populating IP address!!!\n");
    }else{
        printf("Successfully populated IP address.\n");
    }
}


// function to print IP address in ascii format contained in variable of type struct sockaddr_in.
void printPopulatedAddr(struct sockaddr_in addr){
    char *ip;
    // converting IP address from network format to ascii and storing ip pointer. 
    ip = inet_ntoa(addr.sin_addr);

    // checking of error in getting ip
    if(ip == NULL){
        perror("Error on reading populated IP address.\n");
    }else{
        printf("Populated IP address is %s\n",ip);
    }
}

int main(int argc, char **argv){
    // define variable of type struct sockaddr_in for storing IP address
    struct sockaddr_in addr;
    // populateSockAddr will populate the "addr" with the input IP address.
    populateSockAddr(&addr);
    // printPopulatedAddr will print the IP address to standard ouput
    //      stored in "addr" in ascii foramt
    printPopulatedAddr(addr);
    return 0;
}