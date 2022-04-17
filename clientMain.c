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

   // client process
   int sockfd;
   char response[100];

   if (createSocket(&sockfd, &clientAddr, atoi(argv[1]), CLIENT))
      printf("ERROR: socket couldn't be created!\n");
   
   if (connect(sockfd, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
      close(sockfd);
      perror("ERROR: client: connect");
   }

   int numbytes = recv(sockfd, &response, sizeof(response), 0);
   if (numbytes == -1)
      perror("ERROR: client: recv");
   
   response[numbytes] = '\0';
   printf("client recieved '%s'\n", response);

   close(sockfd);

   while( wait(NULL) > 0 );   // wait for child process to finish

   return 0;
}
