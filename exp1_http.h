#include "exp1.h"

typedef struct
{
  char cmd[64];
  char path[256];
  char real_path[256];
  char type[64];
  int code;
  int size;
  /*認証用追加*/
　char auth[256];
  char post[256];
　Digest_data *digest;
} exp1_info_type;

/*digestdata用構造体*/
typedef struct{
    char username[32];
    char nonce[64];
    char uri[64];
    char nc[16];
    char cnonce[64];
    char response[64];
} Digest_data;

int exp1_tcp_listen(int port);
int exp1_tcp_connect(const char *hostname, int port);
double gettimeofday_sec();
