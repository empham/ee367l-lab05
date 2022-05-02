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
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "socket.h"

void sigchld_handler(int s) 
{
   int saved_errno = errno;

   while(waitpid(-1, NULL, WNOHANG) > 0);

   errno = saved_errno;
}

void *get_in_addr(struct sockaddr* sa) 
{
   if(sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
   }

   return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

