#include "exp1.h"
#include "exp1_http.h"

void exp1_send_file(int sock, char *filename){
	FILE *fp;
	int len;
	char buf[EXP1HTTPHEADERSIZE];

	fp = fopen(filename, "r");
	if(fp == NULL){
		shutdown(sock, SHUT_RDWR);
		close(sock);
		return;
	}

	len = fread(buf, sizeof(char), EXP1HTTPHEADERSIZE, fp);
	while(len > 0){
		int ret = send(sock, buf, len, 0);
		if(ret < 0){
			shutdown(sock, SHUT_RDWR);
			close(sock);
			break;
		}
		len = fread(buf, sizeof(char), EXP1HTTPETHERNETLANMSS, fp);
	}

	fclose(fp);
}
