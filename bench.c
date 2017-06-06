#include "exp1.h"
#include <unistd.h>

int main(int argc, char** argv)
{
  int num,ps_num, tstart;
  int i,j,opt;
  int errorcount = 0;
  int aopt = 0;
  int bopt = 0;
  double total_average = 0;
  double tcp_average = 0;
  double http_average = 0;
  pthread_t *th;
  sdata ***data;
  
  if(argc != 5){
    printf("usage: %s [ip address] [# of thread] [# of process] [port]\n", argv[0]);
    exit(-1);
  }
  strcpy(g_hostname, argv[1]);

  g_error_count = 0;
  pthread_mutex_init(&g_mutex, NULL);
 
  num = atoi(argv[2]);  
  ps_num = atoi(argv[3]);
  th = malloc(sizeof(pthread_t) * num);
  
  data = (sdata ***)malloc(sizeof(sdata **) * num);
  for(i = 0;i < num;i++){
    data[i] = (sdata **)malloc(sizeof(sdata *) * ps_num);
    for(j = 0; j < ps_num; j++){
      data[i][j] = (sdata *)malloc(sizeof(sdata));
    }
  }
  srand(0);
  for(i = 0; i < num; i++){
    for(j = 0; j < ps_num; j++){
      int fileid = rand() % 100;
      data[i][j]->pfileid = fileid;
      data[i][j]->port = atoi(argv[4]);
      data[i][j]->start = exp1_gettimeofday_sec();
      data[i][j]->ps_num = ps_num;
      data[i][j]->is_error = 0;
    }
    pthread_create(&th[i], NULL, exp1_eval_thread, data[i]);
  }

  for(i = 0; i < num; i++){
    pthread_join(th[i], NULL);
  }

  for(i = 0;i < num;i++){
    for(j = 0; j < ps_num; j++){
      if(data[i][j]->is_error == 1)
      {
        errorcount++;
        continue;
      }
      tcp_average += (data[i][j]->tcp_end - data[i][j]->start);
      http_average += (data[i][j]->http_end - data[i][j]->start);
      total_average += (data[i][j]->end - data[i][j]->start);
    }
  }
  for(i = 0;i < num;i++){
    for(j = 0; j < ps_num; j++){
      free(data[i][j]);
    }
  }
  for(i = 0;i < num;i++){
    free(data[i]);
  }
  free(data);
  free(th);

  tcp_average /= (num*ps_num)-errorcount;
  http_average /= (num*ps_num)-errorcount;
  total_average /= (num*ps_num)-errorcount;
  
  printf("tcp:%10.10f\nhttp:%10.10f\ntotal:%10.10f\n",tcp_average,http_average,total_average);

  printf("session error ratio is %1.3f\n",(double)g_error_count / (double) num);


}
