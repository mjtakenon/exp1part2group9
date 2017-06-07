#include "exp1_http.h"

char g_hostname[256];
pthread_mutex_t g_mutex;
int g_error_count;

typedef struct{
  int pfileid;
  int port;
  int ps_num;
  int is_error;
  double start;
  double tcp_end;
  double http_end;
  double end;
} sdata;

void* exp1_eval_thread(void* param);
void exp1_session_error();
int main(int argc, char** argv);
