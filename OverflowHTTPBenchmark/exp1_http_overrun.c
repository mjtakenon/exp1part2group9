#include "exp1.h"

#define EXP1HTTPBENCHMARKBUFSIZE 2048

extern int exp1_do_talk(int);

extern int exp1_tcp_connect(char *, int);

int main(int argc, char **argv){
	int sock;
	int ret;
	char buf[EXP1HTTPBENCHMARKBUFSIZE];
	
	memset(buf, '\0', EXP1HTTPBENCHMARKBUFSIZE);
	gets(buf);
	sock = exp1_tcp_connect(argv[1], 15093);
	if(sock < 0){
		fprintf(stderr, "error!\n");
		return 1;
	}
	ret = send(sock, buf, strlen(buf), 0);
	if(ret < 0){
		perror("send");
		return 1;
	}

	ret = 1;
	while(ret == 1){
		ret = exp1_do_talk(sock);
	}
	return 0;
}
