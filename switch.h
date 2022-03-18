/*
 * file: switch.h
 * 
 * contains prototypes, enums and structs for switch.c
 */

/*
 * entry for forwarding table
 */

enum bool {FALSE, TRUE};

struct FT_entry {
   int valid;
   int dst;
   int port;
};

struct switch_job{
//	enum switch_job_type type;
	struct packet *packet;
	int in_port_index;
	int out_port_index;
//	char fname_download[100];
//	char fname_upload[100];
	int ping_timer;
//	int file_upload_dst;
	struct switch_job *next;
};

struct sw_job_queue {
	struct switch_job *head;
	struct switch_job *tail;
	int occ;
};

void fwd_table_add();

void switch_main(int switch_id);

