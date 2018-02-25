#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

int main (int argc, char* argv[]) {
    int sockfd = 0;
    int n;
    char buffer[1024];
    struct sockaddr_in serv_addr; 
    struct hostent *hostinfo;
    
    if (argc != 2) {
        fprintf(stderr, "usage: curl url\n");
        exit(0);
    }

    //memset(recvBuff, '0', sizeof(recvBuff));
    
    //get host information
    hostinfo = gethostbyname(argv[1]);
    if (!hostinfo) {
        printf("Could not resolve host: %s\n", argv[1]);
        exit(1);
    }

    //print host information
    //char *ip = inet_ntoa(((struct in_addr *) hostinfo->h_addr)->s_addr);

    printf("Hostname: %s\n", hostinfo->h_name);
    printf("Address type #: %d\n", hostinfo->h_addrtype);
    printf("Address length: %d\n", hostinfo->h_length);
    //printf("Address ip: %d\n", ip);
     
    //socket creates an endpoint for communication and returns a descriptor.  
    //socket(int domain, int type, int protocol)
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 
   
    //create server address 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = *(struct in_addr *) *hostinfo->h_addr_list; 
    serv_addr.sin_port = htons(80); 
    //serv_addr.sin_addr.s_addr = inet_addr("172.217.9.196");

    //initiate a connection on a socket
    //connect(int socket, const struct sockaddr *address, socklen_t address_len);
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        int errsv = errno;
        printf("\nconnect error occured %d\n", errsv);
        return 1;
    }
    
    //get request to server
    char * request = "GET / HTTP/1.0\r\n\r\n";
    if(send(sockfd , request , strlen(request) , 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    //write server response to standart output
    while((n = read(sockfd, buffer, sizeof buffer)) > 0) {
        write(1, &buffer, n);
    }

    exit(0);
}
