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
#define MIN_QUANTUM 20
#define MAX_QUANTUM 2000
#define MS_PER_TICK 20

struct Job {
	pid_t pid;
	int ticks;
	bool isalive;
	bool usr1;
	char **args;
	int size;
};

struct Job sched[256];
int num_procs = 0;
volatile int active_processes = 0;
volatile int usr1 = 0;
pid_t parent;
const Queue *q = NULL;
struct Job *curr = NULL;
int ticks_in_quantum = 0;

static int pid2idx(pid_t pid) {
	int i;
	for (i = 0; i < num_procs; i++){
		if (sched[i].pid == pid)
			return i;
	}
	return -1;
}


static void usr2_handler(UNUSED int sig){
	//received
}

static void chld_handler(UNUSED int sig){
	pid_t pid;
	int status;

	while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
		if (WIFEXITED(status) || WIFSIGNALED(status)) {
			active_processes--; //printf("%d\n", active_processes);
			sched[pid2idx(pid)].isalive = false;
			kill(parent, SIGUSR2);
			//fprintf(stderr, "%d: exited\n", pid);
		}
	}
}

static void alrm_handler(UNUSED int sig){
	if (curr != NULL){ /* process awaiting execution */
		if (curr->isalive){
			--curr->ticks; //printf("ticks: %d ", curr->ticks); 
			if (curr->ticks > 0){
				return;
			}
			printf("pausing proc: %d\n",curr->pid);
			kill(curr->pid, SIGSTOP); //printf("swap\n");
			//printf("enqueuing pid: %d\n", curr->pid);
			q->enqueue(q, ADT_VALUE(curr));
		}
		curr = NULL;
	}
	while (q->dequeue(q, ADT_ADDRESS(&curr))){
		//printf("dequeuing pid: %d\n", curr->pid);
		if (!curr->isalive){ /* ignore dead process */
			continue;
		}
		curr->ticks = ticks_in_quantum;
		if (curr->usr1){
			curr->usr1 = false;
			printf("starting proc: %d\n",curr->pid);
			kill(curr->pid, SIGUSR1);	/* start execution */
		}
		else{
			printf("continuing proc: %d\n",curr->pid);
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
			kill(sched[i].pid, SIGSTOP);
		}
	}
}

void execWorkload(void){
	int i, sig;
	sigset_t signals;
	sigemptyset(&signals);
	sigaddset(&signals, SIGUSR1);
	sigprocmask(SIG_SETMASK, &signals, NULL);

	for (i = 0; i < num_procs; i++){
		pid_t pid;
		struct Job *tmp = sched + i;
		switch(pid = fork()) {
			case -1: /* fork failed, no subprocess created */
				p1putstr(2, "ERROR: fork failed\n");
				exit(EXIT_FAILURE);
				break;

			case 0: /* child process created, execute program */
				sigwait(&signals, &sig);
				execvp(*sched[i].args, sched[i].args);
				p1putstr(2, "ERROR: exec failed\n");
				exit(1);
				break;

			default: /* parent process*/
				tmp->pid = pid;
				tmp->isalive = true;
				tmp->usr1 = true;
				//printf("enqueuing pid: %d\n", sched[i].pid);
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
		sched[i].args = malloc((wcount+1) * sizeof(char *));
		sched[i].size = wcount+1; wcount = 0; /* +1 due to trailing NULL */

		idx = 0, j = 0;
		/* Allocate each word onto heap and store in job structure*/
		while ((idx = p1getword(buf, idx, next)) != -1 ){
			sched[i].args[j++] = p1strdup(next);
		}
		sched[i++].args[j] = NULL;
	}
	num_procs = i;
	active_processes = num_procs; 
}

void freeWorkload(){
	
	for (int i = 0; i < num_procs; i++){
		struct Job tmp = sched[i];
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
	execWorkload();
	
	while(active_processes > 0){
		pause();
	}
	/* Clean up */
	freeWorkload();
	q->destroy(q);
	return 0;
}