/* ee367L-lab05
 *
 * file: socket.c
 *
 * header file for socket.c
 * contains function prototypes and macros
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#include "socket.h"

int createSocket( int* sockfd, struct sockaddr_in* sockAddress, int port, int type) {
   // create socket
   *sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (type == CLIENT) {
      // specify address for socket
      sockAddress->sin_family = AF_INET;
      sockAddress->sin_port = htons(port);
      sockAddress->sin_addr.s_addr = INADDR_ANY;
   }
   else if (type == SERVER) {
      // specify address for socket
      sockAddress->sin_family = AF_INET;
      sockAddress->sin_port = htons(port);
      sockAddress->sin_addr.s_addr = INADDR_ANY;

      //bind the socket to specified IP{ and port
      bind(*sockfd, (struct sockaddr*) sockAddress, sizeof(*sockAddress));
   }
   else {
      printf("ERROR: createSocket: unknown socket type/n");
   }
   return 0;
}
