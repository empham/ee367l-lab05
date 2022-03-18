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

#define MAX_FT_ENTRIES 100 // maximum number of entries in a forwarding table
#define MAX_FILE_BUFFER 1000
#define MAX_MSG_LENGTH 100
#define MAX_DIR_NAME 100
#define MAX_FILE_NAME 100
#define PKT_PAYLOAD_MAX 100
#define TENMILLISEC 10000   /* 10 millisecond sleep */

/* Job queue operations */

/* Add a job to the job queue */
void sw_job_q_add(struct sw_job_queue *j_q, struct switch_job *j)
{
if (j_q->head == NULL ) {
	j_q->head = j;
	j_q->tail = j;
	j_q->occ = 1;
}
else {
	(j_q->tail)->next = j;
	j->next = NULL;
	j_q->tail = j;
	j_q->occ++;
}
}

/* Remove job from the job queue, and return pointer to the job*/
struct switch_job *sw_job_q_remove(struct sw_job_queue *j_q)
{
struct switch_job *j;

if (j_q->occ == 0) return(NULL);
j = j_q->head;
j_q->head = (j_q->head)->next;
j_q->occ--;
return(j);
}

/* Initialize job queue */
void sw_job_q_init(struct sw_job_queue *j_q)
{
j_q->occ = 0;
j_q->head = NULL;
j_q->tail = NULL;
}

int sw_job_q_num(struct sw_job_queue *j_q)
{
return j_q->occ;
}

/* Forwarding table operations */
void fwd_table_add() {
   printf("unfinished method\n");
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
   struct switch_job *new_job;
   struct switch_job *new_job2;

   struct sw_job_queue job_q;

   /* forwarding table */
   struct FT_entry fwd_table [MAX_FT_ENTRIES];
   enum bool found = FALSE;

   /* initialize forwarding table */
   for (int i=0; i < MAX_FT_ENTRIES; i++) {
      fwd_table[i].valid = 0;
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
   sw_job_q_init(&job_q);

   while(1) {
		/*
		* Get packets from incoming links and translate to jobs
		* Put jobs in job queue
		*/

		for (k = 0; k < node_port_num; k++) { /* Scan all ports */

			in_packet = (struct packet *) malloc(sizeof(struct packet));
			n = packet_recv(node_port[k], in_packet);

			if ((n > 0) && ((int) in_packet->dst == switch_id)) {
				new_job = (struct switch_job *) 
					malloc(sizeof(struct switch_job));
				new_job->in_port_index = k;
				new_job->packet = in_packet;
            // new_job->type = JOB_PING_SEND_REPLY; @note not sure if I'm gonna use job types
				
            sw_job_q_add(&job_q, new_job);
         }
			else {
				free(in_packet);
			}
		}

	   /*
		* Execute one job in the job queue
		*/

		if (sw_job_q_num(&job_q) > 0) {

			/* Get a new job from the job queue */
			new_job = sw_job_q_remove(&job_q);

         /* find source and destination of packet */
         src = new_job->packet->src;
         dst = new_job->packet->dst;

         /* check if entry for src exists in forwarding table */
         found = FALSE;
         for (i=0; i < MAX_FT_ENTRIES; i++) {
            if ( (fwd_table[i].dst == src) && (fwd_table[i].valid == 1) )
               found == TRUE;
         }
         if (found == FALSE) {
            fwd_table_add();
         }

         /* check if entry for dst exists in forwarding table */


			/* Send packets on all ports except src port*/	
				for (k=0; k<node_port_num; k++) {
					packet_send(node_port[k], new_job->packet);
				}
				free(new_job->packet);
				free(new_job);
      }
	   
      /* The switch goes to sleep for 10 ms */
	   usleep(TENMILLISEC);

   }// end while(1)
}
