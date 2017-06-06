#include "exp1_http.h"

char g_hostname[256];
pthread_mutex_t g_mutex;
int g_error_count;

void exp1_session_error()
{
  pthread_mutex_lock(&g_mutex);
  g_error_count++;
  pthread_mutex_unlock(&g_mutex);
}

double exp1_gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

void* exp1_eval_thread(void* param)
{
  int sock;
  int* pfileid;
  int fileid;
  char command[1024];
  char buf[2048];
  int total;
  int ret;

  pfileid = (int*) param;
  fileid = *pfileid;
  free(pfileid);

  sock = exp1_tcp_connect(g_hostname, 15503);
  if(sock < 0){
    exp1_session_error();
    return;
  }

  sprintf(command, "GET /%03d.jpg HTTP/1.0\r\n\r\n", fileid);
  ret = send(sock, command, strlen(command), 0);
  if(ret < 0){
    exp1_session_error();
    return;
  }

  total = 0;
  ret = recv(sock, buf, 2048, 0);
  while(ret > 0){
    total += ret;
    ret = recv(sock, buf, 2048, 0);
  }
}

int main(int argc, char** argv)
{
  int num;
  pthread_t *th;
  int i;
  double start, end;

  if(argc != 3){
    printf("usage: %s [ip address] [# of clients]\n", argv[0]);
    exit(-1);
  }

  strcpy(g_hostname, argv[1]);

  g_error_count = 0;
  pthread_mutex_init(&g_mutex, NULL);

  num = atoi(argv[2]);
  th = malloc(sizeof(pthread_t) * num);

  start = exp1_gettimeofday_sec();

  srand(0);
  for(i = 0; i < num; i++){
    int fileid = rand() % 100;
    int *pfileid = malloc(sizeof(int));
    *pfileid = fileid;
    pthread_create(&th[i], NULL, exp1_eval_thread, pfileid);
  }

  for(i = 0; i < num; i++){
    pthread_join(th[i], NULL);
  }

  end = exp1_gettimeofday_sec();

  printf("total time is %10.10f\n", end - start);
  printf("session error ratio is %1.3f\n",
         (double)g_error_count / (double) num);

  free(th);
}

