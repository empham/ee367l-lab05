/* ee367L-lab05
 *
 * file: smain.c
 *
 * test program to test socket functionality
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "socket.h"

#define MAX_WAITING 5 

int main(int argc, char *argv[]) {
   struct sockaddr_in clientAddr;
   struct sockaddr_in serverAddr;

   //printf("%s, %s %s\n", argv[0], argv[1], argv[2]);
   //printf("%d\n", atoi(argv[1]));
   
   // server process
   int sockfd;
   char server_msg[100] = "Hello from the server!";
   
   if (createSocket(&sockfd, &serverAddr, atoi(argv[1]), SERVER))
      printf("ERROR: socket couldn't be created!\n");

   listen(sockfd, MAX_WAITING);

   int clientSocket;

   //clientSocket = accept(sockfd, (struct sockaddr *)&clientAddr, NULL); // NULL is where cAddr_size would go
   clientSocket = accept(sockfd, NULL, NULL); // NULL is where cAddr_size would go
   if (clientSocket == -1) 
      perror("ERROR: server: accept");

   send(clientSocket, server_msg, sizeof(server_msg), 0);

   close(sockfd);
  
   return 0;
}
