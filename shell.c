#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


#define WHITE  "\x1B[0m"
#define RED  "\x1B[31m"

char *get_current_dir_name(void);
int count_args(char *input);
char **get_args(int num_args, char *input);
void delay(int sec);

typedef struct{
	char *job_name;
	int job_id;
	int active;
}job_t;

int num_jobs = 0;

int main(void){
	job_t *jobs[255] = {NULL};
	while(1){
		char input[255];// = malloc(255);
		char *cur_dir = get_current_dir_name();
		printf(RED "%s" WHITE "$ ", cur_dir);
		fgets(input, sizeof(input)+2, stdin);
		input[strcspn(input, "\r\n")] = 0;
		if(strcmp(input,"") == 0){
			continue;
		}
		if(strcmp(input, "exit") == 0){
			exit(0);
		}
		else if(*input == 'c' && *(input+1) == 'd'){
			const char *path = input+3;
			if(chdir(path) == 0){
				//printf("Successfully changed dir\n");
			}
			else{
				printf("Failure to change directory\n");
			}
		}
		else if(*input == '.' && *(input+1) == '/'){//EXEC program
			char *program;
			int num_args = count_args(input);
			char *temp = malloc(255);
			strcpy(temp,input);
			/*
				Parse arguments
			*/
			char *arguments[num_args+1];
			strtok(temp, " ");
			program = temp;
			int i = 0;
			while((temp=strtok(NULL, " ")) != NULL){
				arguments[i++] = temp;
			}
			arguments[num_args] = (char *)0;
			/*
			*/
			job_t *job = malloc(sizeof(job_t)); 
			job->job_name = program;
			job->job_id = num_jobs;
			job->active = 1;
			jobs[num_jobs++] = job;

			pid_t pid = fork();
			if(pid == 0){//in child
				execve(program, arguments, NULL);
				perror("execve failed");
				exit(EXIT_FAILURE);
			}
			else{//in parent
				int waitstatus;
				int wpid = waitpid(pid, &waitstatus, 0);
				if(wpid != -1){
					fprintf(stderr, "%s\n", "Process ended");
					for(int i = 0; i<num_jobs; i++){
						job_t *cur = jobs[i];
						if((strcmp(cur->job_name, program) == 0) && cur->active == 1){
							cur->active = 0;
							break;
						}
					}
				}
			}
			


			free(temp);
		}
		else if(strcmp(input, "jobs") == 0){
			for(int i = 0; i<num_jobs; i++){
				job_t *cur = jobs[i];
				printf("[%d] %s", cur->job_id, cur->job_name);
				if(cur->active == 0){
					printf("\t%s\n", "STOPPED");
				}else{
					printf("\n");
				}
			}
		}
		else{
			printf("Command '%s' not found.\nTry: sudo apt install %s\n", input, input);
		}
		// fflush(stdin);
		// fflush(stdout);
		// free(input);
		}

}

int count_args(char *input){
	char *temp = malloc(255);
	strcpy(temp,input);
	int res = 0;
	strtok(temp, " ");
	while((temp=strtok(NULL, " ")) != NULL){
		res++;
	}
	free(temp);
	return res;
}

void delay(int sec){
	int mil = 1000000*sec;
	clock_t start = clock();
	while(clock()<start+mil);
}