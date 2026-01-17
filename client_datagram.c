#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, servlen;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    printf("Please enter the message: ");
    bzero(buffer,1024);
    fgets(buffer,1023,stdin);
    servlen = sizeof(serv_addr);
    n = sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr *)&serv_addr,servlen);
    if (n < 0) 
         error("ERROR in sendto");
    bzero(buffer,1024);
    n = recvfrom(sockfd,buffer,1024,0,(struct sockaddr *)&serv_addr,&servlen);
    if (n < 0) 
         error("ERROR in recvfrom");
    printf("%s\n",buffer);
    return 0;
}
