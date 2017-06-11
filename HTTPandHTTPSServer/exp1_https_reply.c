#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

void exp1_https_reply(SSL *ssl, exp1_info_type *info){
	char buf[EXP1HTTPSHEADERSIZE];
	int len;
	int ret;

	if(info->code == 404){
		exp1_send_404(ssl);
		printf("404 not found %s\n", info->path);
		return;
	}

	len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
	len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
	len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
	len += sprintf(buf + len, "\r\n");

	ret = SSL_write(ssl, buf, len);
	if(ret < 0){
		int sock = SSL_get_fd(ssl);
		shutdown(sock, SHUT_RDWR);
		close(sock);
		return;
	}

	exp1_send_file(ssl, info->real_path);
}
