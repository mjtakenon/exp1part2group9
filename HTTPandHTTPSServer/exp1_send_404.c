#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

void exp1_send_404(SSL *ssl){
	char buf[EXP1HTTPSHEADERSIZE];
	int ret;

	sprintf(buf, "HTTP/1.0 404 Not Found\r\n\r\n");
	printf("%s", buf);
	ret = SSL_write(ssl, buf, strlen(buf));
	if(ret < 0){
		int sock = SSL_get_fd(ssl);
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
}
