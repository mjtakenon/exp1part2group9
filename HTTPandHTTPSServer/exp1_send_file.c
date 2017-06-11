#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

void exp1_send_file(SSL *ssl, char *filename){
	FILE *fp;
	int len;
	char buf[EXP1HTTPSHEADERSIZE];

	fp = fopen(filename, "r");
	if(fp == NULL){
		int sock = SSL_get_fd(ssl);
		shutdown(sock, SHUT_RDWR);
		close(sock);
		return;
	}

	len = fread(buf, sizeof(char), EXP1HTTPSHEADERSIZE, fp);
	while(len > 0){
		int ret;
		ret = SSL_write(ssl, buf, len);
		if(ret < 0){
		int sock = SSL_get_fd(ssl);
			shutdown(sock, SHUT_RDWR);
			close(sock);
			break;
		}
		len = fread(buf, sizeof(char), EXP1HTTPSETHERNETLANMSS, fp);
	}

	fclose(fp);
}
