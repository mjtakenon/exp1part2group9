#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

int exp1_https_session(SSL *ssl){
	char buf[2048];
	int recv_size = 0;
	exp1_info_type info;
	int ret = 0;

	while(ret == 0){
		int size;
		
		size = SSL_read(ssl, buf + recv_size, 2048);
		if(size == -1){
			return -1;
		}else if(size == 0){
			return -1;
		}

		recv_size += size;
		ret = exp1_parse_header(buf, recv_size, &info);
	}

	exp1_https_reply(ssl, &info);

	return 0;
}
