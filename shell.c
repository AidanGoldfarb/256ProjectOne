#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define WHITE  "\x1B[0m"
#define RED  "\x1B[31m"

void delay(int sec);
char *get_current_dir_name(void);

int main(void){
	while(1){
		char *input = calloc(255,1);
		char *cur_dir = get_current_dir_name();
		printf(RED "%s" WHITE "$ ", cur_dir);
		fgets(input, sizeof(input)+2, stdin);
		input[strcspn(input, "\r\n")] = 0;
		if(strcmp(input, "exit") == 0){
			exit(0);
		}
		// else if(strcmp(input, "") == 0){
		// 	fflush(stdout);
		// 	continue;
		// }
		else if(*input == 'c' && *(input+1) == 'd'){
			printf("input: %s\n", input);
			const char *path = input+3;
			printf("Path: %s\n", path);
			if(chdir(path) == 0){
				printf("Successfully changed dir\n");
			}
			else{
				printf("Failure to change directory\n");
			}
		}
		else{
			printf("Command '%s' not found.\nTry: sudo apt install %s\n", input, input);
		}
		fflush(stdin);
		fflush(stdout);
		free(input);
		}

}

void delay(int sec){
	int mil = 1000000*sec;
	clock_t start = clock();
	while(clock()<start+mil);
}