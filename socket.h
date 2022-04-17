/* ee367L-lab05
 *
 * file: socket.h
 *
 * header file for socket.c
 * contains function prototypes and macros
 */

#define PORT "3523"
#define HOSTNAME "wiliki.eng.hawaii.edu"

#define CLIENT 0
#define SERVER 1

// creates a socket
// RETURNS: socket address
int createSocket( int* sockfd, struct sockaddr_in* sockAddress, int port, int type);

