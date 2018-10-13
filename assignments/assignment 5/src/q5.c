#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>

// get name of host and print IP address of host
void nameToIP(char *name){
    struct hostent *host;
    
    // get host details in hostent structure
    if( (host = gethostbyname(name)) == NULL){
        printf("Error getting host IP of %s!!!\n",name);
        return;
    }

    // string list to in_addr list
    struct in_addr **addr = (struct in_addr **)host->h_addr_list;

    // printing all ip addresses of host 
    for(int i=0;addr[i] != NULL;i++){
        printf("%s\n",inet_ntoa(*addr[i]));
    }
}

void ipToName(char *ipstr){
    struct hostent *host;
    struct in_addr ip;

    // filling structure in_addr from string ipstr
    if(inet_aton(ipstr,&ip) ==-1){
        perror("Error ascii to network conversion!!!\n");
        exit(0);
    }
    
    // filling struture host of type hostent
    if( (host = gethostbyaddr(&ip,sizeof(struct in_addr),AF_INET)) == NULL ){
        printf("Error getting host name of %s\n",ipstr);
        return;
    }

    char **names = host->h_aliases;

    // printing name of host
    printf("%s\n",host->h_name);

    // printing all aliases of host
    for(int i=0;names[i] != NULL;i++){
        printf("%s\n",names[i]);
    }
}

int main(int argc, char **argv){

    // checking user has given more than 2 arguments
    if(argc <= 2){
        perror("Usage: ./q5 ip ipaddress1 ipaddress2 ipaddress3");
        perror("Usage: ./q5 name hostname1 hostname2 hostname3");
        exit(0);
    }

    for(int i=2;i<argc;i++){
        // if first argument is ip use ipToName
        if(!strcmp(argv[1],"ip")){
            ipToName(argv[i]);
        //  if first argument is name use nameToIP
        }else if(!strcmp(argv[1],"name")){
            nameToIP(argv[i]);
        }else{
        // otherwise show error and exit
            perror("Usage: ./q5 ip ipaddress1 ipaddress2 ipaddress3");
            perror("Usage: ./q5 name hostname1 hostname2 hostname3");
            exit(0);
        }
    }

    return 0;
}