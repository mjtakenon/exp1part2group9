#include "exp1.h"
#include "exp1_http.h"

void exp1_http_reply(int sock, exp1_info_type *info){
	char buf[EXP1HTTPHEADERSIZE];
	int len;
	int ret;

	if(info->code == 404){
		exp1_send_404(sock);
		printf("404 not found %s\n", info->path);
		return;
	}

	len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
	len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
	len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
	len += sprintf(buf + len, "\r\n");

	ret = send(sock, buf, len, 0);
	if(ret < 0){
		shutdown(sock, SHUT_RDWR);
		close(sock);
		return;
	}

	exp1_send_file(sock, info->real_path);
}
