/* 
 * jobs.h
 * jobs for hosts and switches
 */

enum host_job_type {
	JOB_SEND_PKT_ALL_PORTS,
	JOB_PING_SEND_REQ,	
	JOB_PING_SEND_REPLY,
	JOB_PING_WAIT_FOR_REPLY,
	JOB_FILE_UPLOAD_SEND,
	JOB_FILE_UPLOAD_RECV_START,
	JOB_FILE_UPLOAD_RECV_END,
	JOB_FILE_DOWNLOAD_SEND,
	JOB_FILE_DOWNLOAD_RECV_START,
	JOB_FILE_DOWNLOAD_RECV_END
};

struct job {
	enum host_job_type type;
	struct packet *packet;
	int in_port_index;
	int out_port_index;
	char fname_download[100];
	char fname_upload[100];
	int ping_timer;
	int file_upload_dst;
	int file_download_src;
	struct job *next;
};

struct job_queue {
	struct job *head;
	struct job *tail;
	int occ;
};


void job_q_add(struct job_queue *j_q, struct job *j);
struct job *job_q_remove(struct job_queue *j_q);
void job_q_init(struct job_queue *j_q);
int job_q_num(struct job_queue *j_q);
