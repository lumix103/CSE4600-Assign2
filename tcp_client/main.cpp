// g++ -o tcpClient tcpClient.cpp
// ./tcpClient Client
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <cstdint>
#define MAX 80
#define PORT 54550
#define SA struct sockaddr

struct data {
    int sockfd;
    char* name;
};

void* readChat(void * arg) {
    data* serv_data = (data*)arg;
    char buff[MAX], serverName[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        read(serv_data->sockfd, buff, sizeof(buff)); 
        printf("%s", buff); 
    } 
}
void openChat(int sockfd, char *name) 
{ 
    pthread_t read_chat;
    pthread_attr_t read_attr;
    pthread_attr_init(&read_attr);
    data* serv_data = new data;
    serv_data->sockfd = sockfd;
    //strcpy(serv_data->name, name);
    pthread_create(&read_chat, &read_attr, readChat, (void*)serv_data);
    char buff[MAX], serverName[MAX]; 
    int n; 
    for (;;) { 
        bzero(buff, sizeof(buff)); 
        strcat(buff, name); 
        strcat(buff, ": ");
        n = strlen(name) +2; 
        while ((buff[n++] = getchar()) != '\n'); 
        write(sockfd, buff, sizeof(buff)); 
        printf("Message sent. Waiting for server\n");
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
    pthread_join(read_chat, NULL);
    delete serv_data;
} 
int main(int arc, char *argv[]) 
{ 
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
    char *name = argv[1];
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    // Client to server socket connection
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("Could not connect with server...\n"); 
        exit(0); 
    } 
    else
    printf("connected to the server..\n"); 
    // open chat
    openChat(sockfd, name); 
    // close socket 
    close(sockfd); 
}
