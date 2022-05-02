/* ee367L-lab05
 *
 * file: smain.c
 *
 * test program to test socket functionality
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


int main(int argc, char *argv[]) {
   //printf("%s, %s %s\n", argv[0], argv[1], argv[2]);
   //printf("%d\n", atoi(argv[1]));
   
   int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	char string[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	if (argc != 3) {
	    fprintf(stderr,"usage:<hostname> <port>\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);
	printf("client: connecting to %s\n", s);

   // send message that user entered
   if (send(sockfd, argv[1], MAXDATASIZE-1, 0) == -1)
				perror("send");
	
   freeaddrinfo(servinfo); // all done with this structure

	if ((numbytes = recv(sockfd, string, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}

	string[numbytes] = '\0';

	printf("client: received '%s'\n",string);

	close(sockfd);

	return 0;

}