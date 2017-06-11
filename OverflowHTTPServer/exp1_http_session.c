#include "exp1.h"
#include "exp1_http.h"

int exp1_http_session(int sock){
	char buf[EXP1HTTPSESSIONBUFSIZE];
	int recv_size = 0;
	exp1_info_type info;
	int ret = 0;
	
	printf("%p\n", buf);
	while(ret == 0){
		int size = recv(sock, buf + recv_size, EXP1HTTPSESSIONBUFSIZE, 0);
		if(size == -1){
			return -1;
		}

		recv_size += size;
		ret = exp1_parse_header(buf, recv_size, &info);
	}

	exp1_http_reply(sock, &info);

	return 0;
}
