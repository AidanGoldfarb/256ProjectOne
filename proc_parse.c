#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define CYCLE_RATE sysconf(_SC_CLK_TCK)

void run_default();
double *get_cpu_data(char *buf);
double *get_disk_data(char *buf);
//clock_t clock(void);

int main(int argc, char **argv){
	if(argc == 1){ //DEFAULTs
		run_default();
		return 0;
	}
	else if(argc != 3){
		fprintf(stdout, "usage: %s <read_rate> <printout_rate>\n" , argv[0]);
		exit(EXIT_FAILURE);
	}
	else{
		//run_default();

		/*
			Univsersal vars
		*/
			char buf[255];
			FILE *pipe;
			int read_rate = atoi(argv[1]);
			int printout_rate = atoi(argv[2]);
			time_t read_start_time = time(NULL);
			time_t print_start_time = time(NULL);
			int uptime;
			long count = 0;
		/*
		*/

		/*
			Processor mode vars
		*/
			double cpu_total,cpu_user_mode, cpu_system_mode, cpu_idle;
			double *cpu_arr; //[TOTAL, USER, SYSTEM, IDLE]
		/*
		
		*/

		/*
			Memory vars
		*/
			double mem_total, mem_free; 
		/*

		*/

		/*
			Disk vars
		*/
			double disk_sectors_read, disk_sectors_written, disk_time_spent_reading, disk_time_spent_writing;
			double *disk_arr;
		/*

		*/

		/*
			Kernel context switch vars
		*/
			double kernel_switches, kernel_switch_rate;
		/*

		*/

		/*
			Process creation vars
		*/
			double processes_created;
		/*

		*/
		while(read_rate > 0 && printout_rate > 0){
				time_t cur_time = time(NULL);
				
				if((cur_time - read_start_time) > read_rate){ //READ
					++count;
					FILE *uptime_pipe = popen("/bin/cat /proc/uptime", "r");
					fgets(buf, sizeof(buf), uptime_pipe);
					uptime = atof(buf);
					

					/*
						The percentage of time the processor(s) spend in user mode, system mode, 
						and the percentage of time the processor(s) are idle

						cpu (user mode) (user mode low) (system mode) (idle) (iowait) (irq) (softirq) (steal) (guest) (guest_nice)
					*/
						pipe = popen("/bin/cat /proc/stat | grep \"cpu \"", "r");
						fgets(buf, sizeof(buf), pipe);
						cpu_arr = get_cpu_data(buf);
						cpu_total += cpu_arr[0];
						cpu_total /= count;
						cpu_user_mode += cpu_arr[1]; 
						cpu_user_mode /= count;
						cpu_system_mode += cpu_arr[2];
						cpu_system_mode /= count;
						cpu_idle += cpu_arr[3];
						cpu_idle /= count;

						pclose(pipe);

					/*

					*/


					/*
						The amount and percentage of available (or free) memory
					*/
						pipe = popen("/bin/cat /proc/meminfo | grep MemTotal", "r");
						fgets(buf, sizeof(buf), pipe);
						mem_total += (1.0*atoi(buf+16));
						mem_total /= count;
						pclose(pipe);

						pipe = popen("/bin/cat /proc/meminfo | grep MemFree", "r");
						fgets(buf, sizeof(buf), pipe);
						mem_free += (1.0*atoi(buf+17));
						mem_free /= count;
						pclose(pipe);
			


					/*
						The rate (number of sectors per second) of disk read/write in the system
					*/
						pipe = popen("/bin/cat /proc/diskstats | grep \"nvme0n1 \"", "r");
						fgets(buf, sizeof(buf), pipe);
						disk_arr = get_disk_data(buf);
						disk_sectors_read += disk_arr[0];
						disk_sectors_read /= count;
						disk_sectors_written += disk_arr[1];
						disk_sectors_written /= count;
						disk_time_spent_reading += disk_arr[2];
						disk_time_spent_reading /= count;
						disk_time_spent_writing += disk_arr[3];
						disk_time_spent_writing /= count;

						pclose(pipe);


					/*
						The rate (number per second) of context switches in the kernel
					*/
						pipe = popen("/bin/cat /proc/stat | grep ctxt", "r");
						fgets(buf, sizeof(buf), pipe);
						kernel_switches = atof(buf+5);
						
						kernel_switch_rate = kernel_switches/uptime;

						pclose(pipe);
						pclose(uptime_pipe);

					/*

					*/


					/*
						The rate (number per second) of process creations in the system
					*/
						pipe = popen("/bin/cat /proc/loadavg", "r");
						fgets(buf, sizeof(buf), pipe);
						processes_created = atof(buf+17);
					/*
					
					*/

					read_start_time = time(NULL);
				}

				if((cur_time - print_start_time)+1 > (printout_rate)){//PRINT
					/*
						The percentage of time the processor(s) spend in user mode, system mode, 
						and the percentage of time the processor(s) are idle

						cpu (user mode) (user mode low) (system mode) (idle) (iowait) (irq) (softirq) (steal) (guest) (guest_nice)
					*/
						printf("\n\n\u2022 Time spend in:\n \tUser mode: %lf%%\n\tSystem mode: %lf%%\n\tIn idle: %lf%%\n", cpu_user_mode/cpu_total, cpu_system_mode/cpu_total, cpu_idle/cpu_total);
					/*

					*/



					/*
						The amount and percentage of available (or free) memory
					*/
						printf("\u2022 Free memory: %lf kB\n\t\t%lf%% of total\n", mem_free, (1.0*mem_free/mem_total)*100);
					
					/*
					
					*/

					/*
						The rate (number of sectors per second) of disk read/write in the system
					*/
						printf("\u2022 Disk rates: \n\tReads (sectors/second): %lf\n\tWrites (sectors/second) %lf\n", 
							(disk_sectors_read/(disk_time_spent_reading)/1000),(disk_sectors_written/(disk_time_spent_writing)/1000));
					/*

					*/

					/*
						The rate (number per second) of context switches in the kernel
					*/
						printf("\u2022 Context switch rate in kernel: %lf\n", kernel_switch_rate);
					/*

					*/


					/*
						The rate (number per second) of process creations in the system
					*/
						printf("\u2022 Process creation rate: %lf per second\n", 1000*processes_created/uptime);
					/*
					
					*/
					printf("\n#############################################");
					print_start_time = time(NULL);
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
	printf("Uptime: %.6s seconds\n", path);
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

/*
	Parses cpu data from buf and returns array of form
	arr = [TOTAL, USER, SYSTEM, IDLE]
*/
double *get_cpu_data(char *buf){
	double *arr = malloc(4*sizeof(double));
	double cpu_total,cpu_user_mode, cpu_user_mode_low, cpu_system_mode, 
		   cpu_idle, cpu_iowait, cpu_irq, cpu_softirq, cpu_steal, 
		   cpu_guest, cpu_guest_nice;

	/* 
	 Parse line
	*/
	char *cpu_info = strtok(buf, " ");
	cpu_info = strtok(NULL, " ");
	cpu_user_mode = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_user_mode_low = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_system_mode = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_idle = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_iowait = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_irq = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_softirq = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_steal = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_guest = atof(cpu_info);
	cpu_info = strtok(NULL, " ");
	cpu_guest_nice = atof(cpu_info);
	/*
	 End parse
	*/
	cpu_total = cpu_user_mode + cpu_user_mode_low + cpu_system_mode + 
				cpu_idle + cpu_iowait + cpu_irq + cpu_softirq + cpu_steal + 
				cpu_guest + cpu_guest_nice;
	arr[0] = cpu_total;
	arr[1] = cpu_user_mode;
	arr[2] = cpu_system_mode;
	arr[3] = cpu_idle;
	return arr;
}

/*
	Parses disk data from buff and returns array of form
	arr = [SECTORS_READ, SECTORS_WRITTEN, READ_TIME, WRITE_TIME]
*/
double *get_disk_data(char *buf){
	double *arr = malloc(4*sizeof(double));

	double disk_sectors_read, disk_sectors_written, 
		   disk_time_spent_reading, 
		   disk_time_spent_writing;

	char *disk_info = strtok(buf, " ");
	disk_info = strtok(NULL, " ");
	disk_info = strtok(NULL, " ");
	disk_info = strtok(NULL, " ");
	disk_sectors_read = atof(disk_info);
	disk_info = strtok(NULL, " ");
	disk_time_spent_reading = atof(disk_info);
	disk_info = strtok(NULL, " ");
	disk_info = strtok(NULL, " ");
	disk_info = strtok(NULL, " ");
	disk_sectors_written = atof(disk_info);
	disk_info = strtok(NULL, " ");
	disk_time_spent_writing = atof(disk_info);

	arr[0] = disk_sectors_read;
	arr[1] = disk_sectors_written;
	arr[2] = disk_time_spent_reading;
	arr[3] = disk_time_spent_writing;

	return arr;
}