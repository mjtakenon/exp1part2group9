#include "exp1.h"

int exp1_do_talk(int sock){
	fd_set fds;
	char buf[2048];
	int selectval;

	FD_ZERO(&fds);
	FD_SET(0, &fds);
	FD_SET(sock, &fds);
	memset(buf, 0, sizeof(buf));
	selectval = select(sock + 1, &fds, NULL, NULL, NULL);

	if(selectval != 0){
		if(FD_ISSET(0, &fds) != 0){
			int ret;
			fgets(buf, 2048, stdin);
			ret = write(sock, buf, strlen(buf));
			if(ret < 0){
				perror("write");
			}
		}

		if(FD_ISSET(sock, &fds) != 0){
			int ret = recv(sock, buf, 2048, 0);
			if(ret > 0){
				write(1, buf, ret);
			}else{
				return -1;
			}
		}
	}else if(selectval < 0){
		perror("select");
		exit(-1);
	}

	return 1;
}
