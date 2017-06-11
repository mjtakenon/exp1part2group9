#include "exp1.h"

int exp1_tcp_listen(int port){
	int sock;
	/*
	 *sin_len(変数のサイズ)
	 *sin_family(アドレスファミリ、AF_INET=internetwork:TCP,UDP,etc.)
	 *sin_port(ポート番号)
	 *sin_addr(IPアドレス)
	 *sin_zero(気にしない)
	 */
	struct sockaddr_in addr;
	int yes = 1;
	int ret;

	/*OSにソケットの作成を依頼し、ソケットディスプリクタを作成する*/
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char *) &addr, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	ret = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
	if(ret < 0) {
		perror("bind");
		exit(1);
	}

	/*キュー分だけ開く*/
	ret = listen(sock, 5);
	if(ret < 0) {
		perror("reader: listen");
		close(sock);
		exit(-1);
	}

	return sock;
}
