#include "exp1.h"

int exp1_tcp_connect(const char *hostname, int port){
	int sock;
	int ret;
	/*
	 *sin_len(変数のサイズ)
	 *sin_family(アドレスファミリ、AF_INET=internetwork:TCP,UDP,etc.)
	 *sin_port(ポート番号)
	 *sin_addr(IPアドレス)
	 *sin_zero(気にしない)
	 */
	struct sockaddr_in addr;
	/*
	 *h_name(マシンの正式名称)
	 *h_aliases(マシンの別名)
	 *h_addrtype(ホストアドレスのタイプ, AF_INETなど)
	 *h_length(アドレスの長さ, IPv4なら4, IPv6なら16)
	 *h_addr_list(IPアドレスのリスト, NULLで終了する)
	 *h_addr(IPアドレス)
	 */
	struct hostent *host;

	/*OSにソケットの作成を依頼し、ソケットディスプリクタを作成する*/
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0) {
		perror("socket");
		exit(1);
	}
	addr.sin_family = AF_INET;
	/*ホストから情報を取得*/
	host = gethostbyname(hostname);
	addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
	addr.sin_port = htons(port);

	/*ソケットディスプリクタを使ってaddr宛に通信を行う*/
	ret = connect(sock, (struct sockaddr *)&addr, sizeof addr);
	if(ret < 0){
		return -1;
	}else{
		return sock;
	}
}
