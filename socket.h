/* ee367L-lab05
 *
 * file: socket.h
 *
 * header file for socket.c
 * contains function prototypes and macros
 */

//#define PORT "3523"
//#define HOSTNAME "wiliki.eng.hawaii.edu"

#define BACKLOG 10
#define MAXDATASIZE 100 // max num bytes we can put in a packet

/* Used by both client and server */

// get sockaddr for IPv4 or IPv6
void *get_in_addr(struct sockaddr* sa);

/* used by server only */
void sigchld_handler(int s);

