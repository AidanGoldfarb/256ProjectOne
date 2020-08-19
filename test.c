#include <stdio.h>

int main(void){
	char buf [2];
	fgets(buf, sizeof(buf), "hello");
	printf("%s\n", buf);
}