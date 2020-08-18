#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void run_default();
//clock_t clock(void);

int main(int argc, char **argv){
	if(argc == 1){ //DEFAULT
		run_default();
		return 0;
	}
	else if(argc != 3){
		fprintf(stdout, "usage: %s <read_rate> <printout_rate>\n" , argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		//run_default();
		int read_rate = atoi(argv[1]);
		int printout_rate = atoi(argv[2]);
		long total_memory, free_memory;

		char path[255];
		time_t rstart_time = time(NULL); //in milliseconds
		time_t pstart_time = time(NULL);
		while(read_rate > 0 && printout_rate > 0){
				time_t cur_time = time(NULL);
				FILE *memory_pipe = popen("/bin/cat /proc/meminfo | grep MemTotal", "r");
				fgets(path, sizeof(path), memory_pipe);
				total_memory = atoi(path+16);
				pclose(memory_pipe);

				memory_pipe = popen("/bin/cat /proc/meminfo | grep MemFree", "r");
				fgets(path, sizeof(path), memory_pipe);
				free_memory = atoi(path+17);
				pclose(memory_pipe);
				
				if((cur_time - rstart_time) > read_rate){
					rstart_time = clock();
				}
				//printf("%ld\n", (cur_time - start_time));
				if((cur_time - pstart_time)+1 > (printout_rate)){
					//printf("difference: %ld\nrate: %d\n", cur_time - time, printout_rate*1000);
					//printf("total memory %ld\n", total_memory);
					printf("free memory: %ld kB\n%lf%% of total\n", free_memory, (1.0*free_memory/total_memory)*100);
					pstart_time = time(NULL);
				}
				
		}
	}
	return 0;
}

/*	Processor type
	Kernel version
	The amount of memory configured into this computer
	Amount of time since the system was last booted
*/
void run_default(){
	char path[255];
	FILE *processor_pipe = popen("/bin/cat /proc/cpuinfo | grep \"model name	:\" | head -1", "r");
	FILE *kernel_pipe = popen("/bin/cat /proc/version", "r");
	FILE *memory_pipe = popen("/bin/cat /proc/meminfo | grep MemTotal", "r");
	FILE *uptime_pipe = popen("/bin/cat /proc/uptime", "r");
	fgets(path, sizeof(path), processor_pipe);
	printf("Processor type: %s\n", path+13);
	fgets(path, sizeof(path), kernel_pipe);
	printf("Kernel version: %s\n", path);
	fgets(path, sizeof(path), memory_pipe);
	printf("Configured memory: %s\n", path+16);
	fgets(path, sizeof(path), uptime_pipe);
	printf("Uptime: %s seconds\n", path+8);
	// while(fgets(path, sizeof(path), processor_pipe) != NULL){
	// 	fprintf(stdout,"%s\n", path);
	// }
	// while(fgets(path, sizeof(path), kernel_pipe) != NULL){
	// 	fprintf(stdout,"%s\n", path);
	// }
	// while(fgets(path, sizeof(path), memory_pipe) != NULL){
	// 	fprintf(stdout,"%s\n", path);
	// }
	// while(fgets(path, sizeof(path), uptime_pipe) != NULL){
	// 	fprintf(stdout,"Uptime: %s", path);
	// }
	pclose(processor_pipe);
	pclose(kernel_pipe);
	pclose(memory_pipe);
	pclose(uptime_pipe);
}