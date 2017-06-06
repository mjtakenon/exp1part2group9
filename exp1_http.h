#include "exp1.h"

typedef struct
{
  char cmd[64];
  char path[256];
  char real_path[256];
  char type[64];
  int code;
  int size;
} exp1_info_type;

int exp1_tcp_listen(int port);
int exp1_tcp_connect(const char *hostname, int port);
double gettimeofday_sec();
