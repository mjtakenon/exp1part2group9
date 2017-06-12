#include "exp1.h"

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"
#include "myssl_tls.h"

static const char *MYCRTFILE = "cert2.crt";
static const char *MYKEYFILE = "cert2.key";

SSL_CTX *ctx;

int main(int argc, char **argv){
	int sock_listen_https;
	struct stat filestat;
	SSL *ssl;

	if(getuid() != 0){
		fprintf(stderr, "use root account\n");
		return 0;
	}
	if(stat(MYCRTFILE, &filestat) != 0){
		fprintf(stderr, "no cert file\n");
		return 0;
	}
	if(stat(MYKEYFILE, &filestat) != 0){
		fprintf(stderr, "no cert file\n");
		return 0;
	}

	ctx = init_and_set_CTX(SSL_TLS_SERVER);
	set_crt_and_key(ctx, MYCRTFILE, MYKEYFILE);
	ssl = SSL_new(ctx);

	sock_listen_https = exp1_tcp_listen(15093);

	while(1){
		struct sockaddr addr;
		int sock_client, accept_test;
		socklen_t len = sizeof(addr);
		
		sock_client = accept(sock_listen_https, (struct sockaddr *)&addr, &len);
		printf("ssl:%d\n", sock_client);
		SSL_set_fd(ssl, sock_client);
		accept_test = SSL_accept(ssl);
		if(accept_test != -1){
			printf("accept : %d\n", accept_test);
			exp1_https_session(ssl);
		}
		
		shutdown(sock_client, SHUT_RDWR);
		close(sock_client);
	}

	return 0;
}
