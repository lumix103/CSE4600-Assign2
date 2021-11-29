// g++ -o tcpServer tcpServer.cpp
// ./tcpServer Server
#include <stdio.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#define MAX 80
#define PORT 54550

void close_all_fd(int sockfd, fd_set& master_set);

int main(int argc, char** argv) {
   char buff[MAX], clientName[MAX];
   int on = 1;
   int comms_closed;
   int stop_server = 0;
   struct sockaddr_in serv_addr;
   int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
   if (sock_fd < 0) {
      printf("Failed to create socket...\n");
      exit(EXIT_FAILURE);
   } 

   int sock_opt = setsockopt(sock_fd, SOL_SOCKET, 
      SO_REUSEADDR, (char*)&on, sizeof(on));

   if (sock_opt < 0) {
      printf("Failed to setup socket options...\n");
      close(sock_fd);
      exit(EXIT_FAILURE);
   }

   int control_device = ioctl(sock_fd, FIONBIO, (char*)&on);

   if(control_device < 0) {
      printf("Failed to setup control device...\n");
      close(sock_fd);
      exit(EXIT_FAILURE);
   }

   bzero(&serv_addr, sizeof(serv_addr));
   
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(PORT);

   int bind_sock = bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   if (bind_sock < 0) {
      printf("Failed to bind socket...\n");
      close(sock_fd);
      exit(EXIT_FAILURE);
   }

   int listen_sock = listen(sock_fd, 5);
   if (listen_sock < 0) {
      printf("Failed to listen...\n");
      close(sock_fd);
      exit(EXIT_FAILURE);
   }

   fd_set master_set, ready_set;

   FD_ZERO(&master_set);
   int copy_sock_fd = sock_fd;
   FD_SET(sock_fd, &master_set);


   struct timeval timeout;
   
   timeout.tv_sec  = 180;
   timeout.tv_usec = 0;

   int ready_desc;
   int new_desc;
   do {
      memcpy(&ready_set, &master_set, sizeof(master_set));
      int waiting_desc = select(copy_sock_fd + 1, &ready_set,
                                  NULL, NULL, &timeout);
      if (waiting_desc < 0) {
         printf("Failed to select a socket...\n");
         close_all_fd(sock_fd, master_set);
         exit(EXIT_FAILURE);
      }

      if (waiting_desc == 0) {
         printf("Connection timed out...\n");
         break;
      }
      ready_desc = waiting_desc;
      for(int i = 0; i <= copy_sock_fd && ready_desc > 0; i++) {
         if(FD_ISSET(i, &ready_set)) {
            ready_desc -= 1;
            if (i == sock_fd) {
               do {
                  new_desc  = accept(sock_fd, NULL, NULL);
                  if(new_desc < 0) {
                     if(errno != EWOULDBLOCK) {
                        perror("Server accept failed...\n");
                        stop_server = 1;
                     }
                     break;
                  }
                  printf("Server accepeted new connection...\n");
                  FD_SET(new_desc, &master_set);
                  if(new_desc > copy_sock_fd)
                     copy_sock_fd = new_desc;
               } while(new_desc != -1);
            } else {
               comms_closed = 0;

                  ready_desc = recv(i, buff, sizeof(buff), 0);
                  if (ready_desc < 0) {
                     if(errno != EWOULDBLOCK) {
                        perror("Server failed to recieve...\n");
                        comms_closed = 1;
                     }
                     //break;
                  }

                  if (ready_desc == 0) {
                     printf("Client closed connection...\n");
                     comms_closed = 1;
                     //break;
                  }

                  for(int j =0; j < FD_SETSIZE; j++) {
                     if (i!=j && j!=sock_fd && FD_ISSET(j, &master_set)) {
                        ready_desc = send(j, buff, ready_desc, 0);
                        if (ready_desc<0) {
                           perror("Failed to send data...\n");
                           comms_closed = 1;
                           //break;
                        }
                     }
                  }

               if (comms_closed == 1) {
                  close(i);
                  FD_CLR(i,&master_set);
                  if (i == copy_sock_fd) {
                     while(FD_ISSET(copy_sock_fd, &master_set) == 0)
                        copy_sock_fd -= 1;
                  }
               }
            }
         }
      }
   } while(stop_server == 0);
   
   close_all_fd(sock_fd, master_set);
   return EXIT_SUCCESS;  
}

void close_all_fd(int sockfd, fd_set& master_set) {
   for(int i = 0; i <= sockfd; i++)
   {
      if(FD_ISSET(i, &master_set))
         close(i);
   }
}

/*
   for(int j =0; j < copy_sock_fd; j++) {
                     if (i != j && j!=sock_fd && FD_ISSET(j, &master_set)) {
                        printf("%s\n",buff);
                        ready_desc = send(j, buff, ready_desc, 0);
                        if (ready_desc<0) {
                           perror("Failed to send data...\n");
                           comms_closed = 1;
                           //break;
                        }
                     }
                  }
*/