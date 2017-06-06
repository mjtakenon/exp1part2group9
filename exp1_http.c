#include "exp1_http.h"

int exp1_tcp_listen(int port)
{
  int sock;
  /*インターネット用ソケット構造体*/
  struct sockaddr_in addr;
  int yes = 1;
  int ret;

  /*PF_INETはインターネット通信(PF_UNIXはローカル)、SOCK_STREAMはTCP通信を指定,3引数は基本0(プロトコル1種類の場合)*/
  sock = socket(PF_INET, SOCK_STREAM, 0);
  /*ソケットが開けなかった場合エラー*/
  if(sock < 0) {
    perror("socket");
    exit(1);
  }

  /*addrを0で埋める*/
  bzero((char *) &addr, sizeof(addr));
  /*定数を入れる(PF_INETはネット用)*/
  addr.sin_family = PF_INET;
  /*0.0.0.0が入る、全てのクライアントからの接続を受け付ける(32bitに変換)*/
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  /*通信を行うポート(16bit)*/
  addr.sin_port = htons(port);
  /*ソケットオプションの設定,
  SOL_SOCKETはソケットに対してオプションをセットする
  SO_REUSEADDRで同じポートを複数回bindできる?(TIME_WAIT状態のポートが存在していてもbindできるようになる)*/
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  /*ソケットに名前を付ける(ソケット番号にポート番号とIPアドレスを関連付ける)*/
  ret =  bind(sock, (struct sockaddr *)&addr, sizeof(addr));
  if(ret < 0) {
    perror("bind");
    exit(1);
  }

  /*listenはsock番号を接続待ちソケットとして認識させる*/
  /*同時に接続を受け付けるためのキューの大きさは5*/
  ret = listen(sock, 5);
  if(ret < 0) {
    perror("reader: listen");
    close(sock);
    exit(-1);
  }
  return sock;
}

int exp1_tcp_connect(const char *hostname, int port)
{
  int sock;
  int ret;
  struct sockaddr_in addr;
  struct hostent *host;
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  addr.sin_family = AF_INET;
  /*ホスト名からIPアドレスを取得*/
  host = gethostbyname(hostname);
  addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
  addr.sin_port = htons(port);

  ret = connect(sock, (struct sockaddr *)&addr, sizeof addr);
  if(ret < 0){
    return -1;
  }else{
    return sock;
  }
}

double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}
