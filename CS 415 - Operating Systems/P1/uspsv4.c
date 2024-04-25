/* Elias Charles : eliasc : CIS 415 Proj. 1
 *
 * This is my own work.
 *
 */


#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <signal.h>
#include "p1fxns.h"
#include "ADTs/queue.h"

#define UNUSED __attribute__((unused))
#define USAGE "usage: ./uspsv? [-q <quantum in msec>][workload_file]\n"
#define MAX 128
#define MAXBUF 4096
#define MIN_QUANTUM 20
#define MAX_QUANTUM 2000
#define MS_PER_TICK 20

typedef struct pcb {
	pid_t pid;
	int ticks;
	bool isalive;
	bool usr1;
	char **args;
	int size;
} PCB;

PCB arr[MAX];
int num_procs = 0;
volatile int active_processes = 0;
volatile int usr1 = 0;
pid_t parent;
const Queue *q = NULL;
PCB *curr = NULL;
int ticks_in_quantum = 0;

static int pid2idx(pid_t pid) {
	int i;
	for (i = 0; i < num_procs; i++){
		if (arr[i].pid == pid)
			return i;
	}
	return -1;
}

void readProc(void);

static void usr2_handler(UNUSED int sig){
	//received
}

static void chld_handler(UNUSED int sig){
	pid_t pid;
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		if (WIFEXITED(status) || WIFSIGNALED(status)) {
			active_processes--;
			arr[pid2idx(pid)].isalive = false;
			kill(parent, SIGUSR2);
			//fprintf(stderr, "%d: exited\n", pid);
		}
	}
}

static void alrm_handler(UNUSED int sig){
	if (curr != NULL){ /* process awaiting execution */
		if (curr->isalive){
			--curr->ticks;
			if (curr->ticks > 0){
				return;
			}
			kill(curr->pid, SIGSTOP); //printf("swap\n");
			readProc();
			q->enqueue(q, ADT_VALUE(curr));
		}
		curr = NULL;
	}
	while (q->dequeue(q, ADT_ADDRESS(&curr))){
		if (!curr->isalive){ /* ignore dead process */
			continue;
		}
		curr->ticks = ticks_in_quantum;
		readProc();
		if (curr->usr1){
			curr->usr1 = false;
			kill(curr->pid, SIGUSR1);	/* start execution */
			
		}
		else{
			kill(curr->pid, SIGCONT);	/* continue execution */
		}
		return;
	}
}
void initHandlers(void){
	struct itimerval timer;
	timer.it_value.tv_sec = MS_PER_TICK/1000;
	timer.it_value.tv_usec = (MS_PER_TICK*1000) % 1000000;
	timer.it_interval = timer.it_value;

	signal(SIGUSR2, usr2_handler);
	signal(SIGCHLD, chld_handler);
	signal(SIGALRM, alrm_handler);

	if(setitimer(ITIMER_REAL, &timer, NULL) == -1){
		for (int i = 0; i < num_procs; i++){
			kill(arr[i].pid, SIGSTOP);
		}
	}
}

void readProc(){

	char fp[32]; char buf[MAXBUF]; char pidbuf[32];
	char stfp[32]; char scfp[32];
	int i, n, fd, idx = 0;
	for (i = 0; i < num_procs; i++){
		p1itoa(arr[i].pid, pidbuf);
		p1strcpy(fp, "/proc/"); p1strcat(fp, pidbuf);
		p1strcpy(stfp, fp); p1strcat(stfp,"/status");
		p1strcpy(scfp, fp); p1strcat(scfp,"/sched");

		fd = open(scfp, O_RDONLY);
		if (fd == -1){
			p1putstr(1, "PID:\t"); p1putstr(1, pidbuf); p1putstr(1, "\texited\n");	
			continue;
		}

		while ((n = p1getline(fd, buf, MAXBUF)) != 0){
			if (idx == 0) // process name
				p1putstr(1, buf);
			if (idx == 4){ // runtime
				p1putstr(1, buf);
				break;
			}
			idx++;
		}
		close(fd); buf[0] = '\n'; idx = 0;

		fd = open(stfp, O_RDONLY);
		while ((n = p1getline(fd, buf, MAXBUF)) != 0){
			if (idx == 2) // state
				p1putstr(1, buf);
			if (idx == 16) // Vmpeak
				p1putstr(1, buf);
			if (idx == 17){ // Vmsize
				p1putstr(1, buf);
				break;
			}
			idx++;
		}
		close(fd); buf[0] = '\n'; idx = 0;

		if (i < num_procs-1)
			p1putstr(1, "\n");
	}
	p1putstr(1, "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n");
}

void execWorkload(void){
	int i, sig;
	sigset_t signals;
	sigemptyset(&signals);
	sigaddset(&signals, SIGUSR1);
	sigprocmask(SIG_SETMASK, &signals, NULL);

	for (i = 0; i < num_procs; i++){
		pid_t pid;
		PCB *tmp = arr + i;
		switch(pid = fork()) {
			case -1: /* fork failed, no subprocess created */
				p1putstr(2, "ERROR: fork failed\n");
				exit(EXIT_FAILURE);
				break;

			case 0: /* child process created, execute program */
				sigwait(&signals, &sig);
				execvp(*arr[i].args, arr[i].args);
				p1putstr(2, "ERROR: exec failed\n");
				exit(1);
				break;

			default: /* parent process*/
				tmp->pid = pid;
				tmp->isalive = true;
				tmp->usr1 = true;
				q->enqueue(q, ADT_VALUE(tmp));
				break;
		}
	}
}


void procWorkload(int fd){
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * reads workload and allocates onto heap
	 * returns num lines in the file
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	char buf[BUFSIZ], next[BUFSIZ];
	int n, idx, j; int i = 0, wcount = 0;


	while ((n = p1getline(fd, buf, BUFSIZ)) != 0){
		if (buf[n-1] == '\n')
			buf[n-1] = '\0';
		idx = 0;
		/* Count # words in line & allocate onto heap */
		while ((idx = p1getword(buf, idx, next)) != -1){ wcount++; }
		arr[i].args = malloc((wcount+1) * sizeof(char *));
		arr[i].size = wcount+1; wcount = 0; /* +1 due to trailing NULL */

		idx = 0, j = 0;
		/* Allocate each word onto heap and store in job structure*/
		while ((idx = p1getword(buf, idx, next)) != -1 ){
			arr[i].args[j++] = p1strdup(next);
		}
		arr[i++].args[j] = NULL;
	}
	num_procs = i;
	active_processes = num_procs; 
}

void freeWorkload(){
	
	for (int i = 0; i < num_procs; i++){
		PCB tmp = arr[i];
		for (int j = 0; j < tmp.size; j++){
			free(tmp.args[j]);
		}
		free(tmp.args);
	}
}


int main(int argc, char **argv){
	int opt;
	int val = -1;
	char *p;

	/* check for env var */
	if ((p = getenv("USPS_QUANTUM_MSEC")) != NULL)
		val = atoi(p);

	/* process cmdline args */
	while ((opt = getopt(argc, argv, "q:")) != -1){
		switch (opt){
			case 'q':
				val = atoi(optarg);
				break;

			default: /* unexpected option */
				p1putstr(2, USAGE);
				exit(EXIT_FAILURE);
				break;
		}
	}

	/* Check that environment variable has been assigned (and is greater than 0)*/
	if (val < MIN_QUANTUM || val > MAX_QUANTUM || optind+1 < argc ){ /* optind+1 < argc implies more than 1 file provided */
		p1putstr(2, USAGE);
		exit(EXIT_FAILURE);
	}

	val = MS_PER_TICK * ((val+1) / MS_PER_TICK); /* calculate quantum / ticks */
	ticks_in_quantum = val / MS_PER_TICK;

	/* Check that file descriptor is valid */
	int fd = STDIN_FILENO; 
	if (argc > optind){		
		fd = open(argv[optind], O_RDONLY);
		if (fd == -1){
			p1putstr(2, "ERROR: File cannot be opened\n");
			exit(EXIT_FAILURE);
		}
	}

	q = Queue_create(doNothing);
	parent = getpid();
	procWorkload(fd); close(fd);
	initHandlers();
	alrm_handler(SIGALRM);
	execWorkload();
	



	while(active_processes > 0){
		pause();
	}
	readProc();
	/* Clean up */
	freeWorkload();
	q->destroy(q);
	return 0;
}