/*
 * file: switch.c
 *
 * code for a switch node
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <unistd.h>
#include <fcntl.h>

#include "main.h"
#include "net.h"
#include "man.h"
#include "host.h"
#include "packet.h"
#include "switch.h"
#include "jobs.h"

#define MAX_FT_ENTRIES 100 // maximum number of entries in a forwarding table
#define MAX_FILE_BUFFER 1000
#define MAX_MSG_LENGTH 100
#define MAX_DIR_NAME 100
#define MAX_FILE_NAME 100
#define PKT_PAYLOAD_MAX 100
#define TENMILLISEC 10000   /* 10 millisecond sleep */


/* Forwarding table operations */
void fwd_table_add(struct FT_entry ** fwd_table, int dst, int port_index) {
   if (port_index > MAX_FT_ENTRIES) 
      printf("DEBUG: fwd_table_add(): Port index out of bounds!\n");
   else
      printf("DEBUG: fwd_table_add(): dst = %d, Port index = %d\n", dst, port_index);
/*
   struct FT_entry new_entry = fwd_table[port_index];
   
   new_entry.valid = 1;
   new_entry.dst   = dst;
   new_entry.port  = port_index;
*/

   struct FT_entry* new_entry = (struct FT_entry*) malloc(sizeof(struct FT_entry));
   
   new_entry->valid = 1;
   new_entry->dst   = dst;
   new_entry->port  = port_index;
   fwd_table[port_index] = new_entry;

}

/*
 * main
 */

void switch_main(int switch_id) {

   struct net_port *node_port_list;
   struct net_port **node_port;  // Array of pointers to node ports
   int node_port_num;            // Number of node ports
  
   int i, k, n;
   int dst, src;
   char name[MAX_FILE_NAME];
   char string[PKT_PAYLOAD_MAX+1];

   FILE *fp;
   struct packet *in_packet; /* Incoming packet */
   struct packet *new_packet;

   struct net_port *p;
   struct job *new_job;
   struct job *new_job2;

   struct job_queue job_q;

   /* forwarding table */
   struct FT_entry* fwd_table [MAX_FT_ENTRIES];
   enum bool found = FALSE;

   /* initialize forwarding table */
   for (int i=0; i < MAX_FT_ENTRIES; i++) {
      //fwd_table[i]->valid = 0;
      fwd_table[i] = NULL;
   }

/*
 * Create an array node_port[ ] to store the network link ports
 * at the switch.  The number of ports is node_port_num
 */
   node_port_list = net_get_port_list(switch_id);

	/*  Count the number of network link ports */
   node_port_num = 0;
   for (p=node_port_list; p!=NULL; p=p->next) {
	   node_port_num++;
   }
	/* Create memory space for the array */
   node_port = (struct net_port **) 
	   malloc(node_port_num*sizeof(struct net_port *));

	/* Load ports into the array */
   p = node_port_list;
   for (k = 0; k < node_port_num; k++) {
	   node_port[k] = p;
	   p = p->next;
   }	
/* Initialize the job queue */
   job_q_init(&job_q);

   while(1) {
		/*
		* Get packets from incoming links and translate to jobs
		* Put jobs in job queue
		*/

		for (k = 0; k < node_port_num; k++) { /* Scan all ports */

			in_packet = (struct packet *) malloc(sizeof(struct packet));
			n = packet_recv(node_port[k], in_packet);

			if (n > 0) {
            printf( "DEBUG: switch recieved packet!\n");
				new_job = (struct job *) 
					malloc(sizeof(struct job));
				new_job->in_port_index = k;
				new_job->packet = in_packet;
				
            job_q_add(&job_q, new_job);
         }
			else {
				free(in_packet);
			}
		}

	   /*
		* Execute one job in the job queue
		*/

		if (job_q_num(&job_q) > 0) {

			/* Get a new job from the job queue */
			new_job = job_q_remove(&job_q);

         /* find source and destination of packet */
         src = new_job->packet->src;
         dst = new_job->packet->dst;

         /* check if entry for src exists in forwarding table */
         found = FALSE;
         //printf("DEBUG: switch.c, execute job: src = %d, dst = %d, fwd_table[in_port_index]->dst = %d\n",
         //         src, dst, fwd_table[new_job->in_port_index]->dst);
         for (i=0; i < MAX_FT_ENTRIES; i++) {
            if (fwd_table[i] == NULL)
               continue;

            else if ( (fwd_table[i]->dst == src) && (fwd_table[i]->valid == 1) ) {
               printf("DEBUG: switch.c, execute job: fwd_table entry found!\n");
               found = TRUE;
               break;
            }
         }
         printf("DEBUG: switch.c, execute job: found = %d\n", found);
         if (found == FALSE) {
            printf("DEBUG: switch.c, execute job: src = %d, dst = %d, in_port_index = %d\n",
                  src, dst, new_job->in_port_index);
            fwd_table_add(fwd_table, src, new_job->in_port_index);
            printf("DEBUG: switch.c, execute job: src = %d, dst = %d, fwd_table[in_port_index]->dst = %d\n",
                  src, dst, fwd_table[new_job->in_port_index]->dst);
         }

         /* check if entry for dst exists in forwarding table */
         found = FALSE;
         for (i=0; i < MAX_FT_ENTRIES; i++) {
            if (fwd_table[i] == NULL)
               continue;
            else if ( (fwd_table[i]->dst == dst) && (fwd_table[i]->valid == 1) ) {
               found = TRUE;
               break;
            }
         }
         /* if found, forward packet to specified port */
         if (found == TRUE) {
            packet_send(node_port[i], new_job->packet);
         }
			/* otherwise, send packets on all ports except src port*/	
         else {
				for (k=0; k<node_port_num; k++) {
               if (k != new_job->in_port_index)
					   packet_send(node_port[k], new_job->packet);
				}
				free(new_job->packet);
				free(new_job);
         }
      }
	   
      /* The switch goes to sleep for 10 ms */
	   usleep(TENMILLISEC);

   }// end while(1)
}
