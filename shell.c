#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define WHITE  "\x1B[0m"
#define RED  "\x1B[31m"

char *get_current_dir_name(void);
int count_args(char *input);
void get_args(char *arguments, int num_args, char *input);
void delay(int sec);

int main(void){
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
			int num_args = count_args(input);
			char *arguments[num_args+1];
			memset(arguments,0,num_args*100);
			get_args(*arguments, num_args, input);
			//printf("Args: %s\n", *arguments);

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

void get_args(char *arguments, int num_args, char *input){
	char *temp = malloc(255);
	strcpy(temp,input);
	strtok(temp, " ");
	for(int i = 0; i<num_args; i++){
		arguments[i] = *temp;
		temp = strtok(NULL, " ");
	}
}

void delay(int sec){
	int mil = 1000000*sec;
	clock_t start = clock();
	while(clock()<start+mil);
}