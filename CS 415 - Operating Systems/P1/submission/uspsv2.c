/* Elias Charles : eliasc : CIS 415 Proj. 1
 *
 * This is my own work.
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include "p1fxns.h"

#define UNUSED __attribute__((unused))

struct Job {
	pid_t pid;
	char **args;
	int size;
};

void freeWorkload(struct Job *arr, int n);

void executeWorkload(struct Job *sched, int n){
	int sig, i;
	sigset_t signals;
	sigemptyset(&signals);
	sigaddset(&signals, SIGUSR1);
	sigaddset(&signals, SIGUSR2);
	sigaddset(&signals, SIGSTOP);
	sigaddset(&signals, SIGCONT);
	sigprocmask(SIG_SETMASK, &signals, NULL);

	for (i = 0; i < n; i++){
		switch(sched[i].pid = fork()) {
			case -1: /* fork failed, no subprocess created */
				p1putstr(2, "ERROR: fork failed\n");
				freeWorkload(sched, n);
				exit(EXIT_FAILURE);
				break;

			case 0: /* child process created, execute program */
				sigwait(&signals, &sig);
				execvp(*sched[i].args, sched[i].args);
				p1putstr(2, "ERROR: exec failed\n");
				freeWorkload(sched, n);
				exit(EXIT_FAILURE);
				break;

			default: /* parent process*/
				usleep(1000);
				break;
		}
	}

	for (i = 0; i < n; i++){
		kill(sched[i].pid, SIGUSR1);	/* Signal to begin processes */
		printf("start: %d\n", sched[i].pid);
	}
	for (i = 0; i < n; i++){
		kill(sched[i].pid, SIGSTOP);	/* Signal to halt processes */
		printf("stop: %d\n", sched[i].pid);
	}
	for (i = 0; i < n; i++){
		kill(sched[i].pid, SIGCONT);	/* Signal to resume processes */
		printf("cont: %d\n", sched[i].pid);
	}
	for (i = 0; i < n; i++){		/* Wait for each child pid before terminating */
		wait(&sched[i].pid);
	}
}


int procWorkload(int fd, struct Job* sched){
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 * reads workload and allocates onto heap
	 * returns num lines in the file
	 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	 */
	char buf[BUFSIZ], next[BUFSIZ];
	int n, idx, j, i = 0, wcount = 0;
	size_t nbytes = 8;

	while ((n = p1getline(fd, buf, BUFSIZ)) != 0){
		if (buf[n-1] == '\n')
			buf[n-1] = '\0';
		idx = 0;
		/* Count # words in line & allocate onto heap */
		while ((idx = p1getword(buf, idx, next)) != -1){ wcount++; }
		sched[i].args = malloc((wcount+1) * nbytes); /*(char *) = 8 bytes)*/
		sched[i].size = wcount+1; wcount = 0; /* +1 due to trailing NULL */

		idx = 0, j = 0;
		/* Allocate each word onto heap and store in job structure*/
		while ((idx = p1getword(buf, idx, next)) != -1 ){
			sched[i].args[j] = p1strdup(next);
			j++;
		}

		sched[i].args[j] = NULL;
		i++;
	}

	return i;
}

void freeWorkload(struct Job *arr, int n){
	
	for (int i = 0; i < n; i++){
		struct Job tmp = arr[i];
		for (int j = 0; j < tmp.size; j++){
			free(tmp.args[j]);
		}
		free(tmp.args);
	}
}


int main(int argc, char **argv){
	int opt, nlines;
	int val = -1;
	char *p;
	struct Job sched[256];
	
	/* check for env var */
	if ((p = getenv("USPS_QUANTUM_MSEC")) != NULL)
		val = atoi(p);

	while ((opt = getopt(argc, argv, "q:")) != -1){
		switch (opt){
			case 'q':
				val = p1atoi(optarg);
				break;

			default: /* unexpected option */
				p1putstr(2, "usage: ./uspsv? [-q <quantum in msec>][workload_file]\n");
				exit(EXIT_FAILURE);
				break;
		}
	}
	/* Check that environment variable has been assigned (and is greater than 0)*/
	if (val <= 0 || optind+1 < argc ){ /* optind+1 < argc implies more than 1 file provided */
		p1putstr(2, "usage: ./uspsv? [-q <quantum in msec>][workload_file]\n");
		exit(EXIT_FAILURE);
	}

	int fd = STDIN_FILENO; /* default file descriptor is stdin */
	if (argc > optind){
		fd = open(argv[optind], O_RDONLY);
		if (fd == -1){
			p1putstr(2, "ERROR: File cannot be opened\n");
			exit(EXIT_FAILURE);
		}
	}
	
	nlines = procWorkload(fd, sched); 
	close(fd);
	executeWorkload(sched, nlines);
	freeWorkload(sched, nlines);
	return 0;
}