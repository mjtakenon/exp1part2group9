#include "exp1.h"
#include "exp1_http.h"

void exp1_send_404(int sock){
	char buf[EXP1HTTPHEADERSIZE];
	int ret;

	sprintf(buf, "HTTP/1.0 404 Not Found\r\n\r\n");
	printf("%s", buf);
	ret = send(sock, buf, strlen(buf), 0);

	if(ret < 0){
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}
}
