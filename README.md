# 256 Project One

![Platform](https://img.shields.io/badge/Platform-Linux-brightgreen)

## Part I
Reads data from the kernel and prints it in an informative format.

### Usage
  - Run `make`
  - Execute `./proc_parse [read_rate] [print_rate]`
    - `read_rate` specifies how frequently (in seconds) that data is read from the kernel and averaged
    - `print_rate` specifies how frequently (in seconds) that the averages are printed to `stdout`
### Requirements
  - *In this part, you are asked to write a program to report the behavior of the Linux kernel. Your program should run in two different versions*
  - **Version One:**
    - Prints to `stdout`
      - Processor type
        - Parsed from `/proc/cpuinfo`
      - Kernel version
        - Parsed from `/proc/version`
      - Amount of configured memory
        - Parsed from `/proc/meminfo`
      - Uptime
        - Parsed from `/proc/uptime`
    
   
   - **Version Two:**
     - Prints continuously based on print_rate
       - The percentage of time the processor(s) spend in user mode, system mode, and the percentage of time the processor(s) are idle
         - Parsed from `/proc/stat`
       - The amount and percentage of available (or free) memory
         - Parsed from `/proc/meminfo`
       - The rate (number of sectors per second) of disk read/write in the system
         - Parsed from `/proc/diskstats/`
       - The rate (number per second) of context switches in the kernel
         - Parsed from `/proc/stat`
       - The rate (number per second) of process creations in the system
         - Parsed from `/proc/loadavg`

## Part 2
Implements a very primitive shell that knows how to launch new programs in the foreground and the background as well as recognizes several internal commands

### Usage
 - Run `make`
 - `./shell` to launch shell
 
#### Requirements
 - Recognizes the following internal commands
   - exit
     - Uses system call exit()
   - jobs
     - Custom naive job database
   - cd
     - Uses system call chdir()
   - fg\*
   - bg\*
   
     \* Not yet implemented
 - Run programs
   - `./program [arg1][arg2]...[argN]` supported
