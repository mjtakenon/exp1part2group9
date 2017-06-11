#include <stdio.h>

int main(void){
	char *argv[] = {"date > /tmp/hacked", NULL};
	char *argv2  = "/bin/date";
	execve(argv2, argv, NULL);
}
