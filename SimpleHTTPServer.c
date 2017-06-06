#include "SimpleHTTPServer.h"

void exp1_send_file(int sock, char* filename)
{
  FILE *fp;
  int len;
  char buf[16384];

  fp = fopen(filename, "r");
  if(fp == NULL){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  len = fread(buf, sizeof(char), 16384, fp);
  while(len > 0){
    int ret = send(sock, buf, len, 0);
    if(ret < 0){
      shutdown(sock, SHUT_RDWR);
      close(sock);
      break;
    }
    len = fread(buf, sizeof(char), 1460, fp);
  }
  fclose(fp);
}

void exp1_send_301(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 301 Moved Permanently\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_302(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 302 Found\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_303(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 303 See Other\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_401(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 401 Bad request\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_403(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 403 forbidden\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_418(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 418 I'm a teapot\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_send_404(int sock)
{
  char buf[16384];
  int ret;
  sprintf(buf, "HTTP/1.0 404 Not Found\r\n\r\n");
  printf("%s", buf);
  ret = send(sock, buf, strlen(buf), 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
  }
}

void exp1_http_reply(int sock, exp1_info_type *info)
{
  char buf[16384];
  int len;
  int ret;

  if(info->code == 404){
    exp1_send_404(sock);
    printf("404 not found %s\n", info->path);
    return;
  }

  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
  len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  if(strcmp(info->type,"text/php") == 0){
    exp1_send_php(sock, info->real_path);
  }
  else{
    exp1_send_file(sock, info->real_path);
  }
}

void exp1_send_php(int sock, char* filename)
{
  char str[16384];
  char buf[16384];
  char tmp[256];
  int ret,len;
  FILE* fp;
  
  char command[256];
  sprintf(command,"/usr/bin/php %s",filename);
 
  if ( (fp=popen(command,"r")) == NULL) {
      err(EXIT_FAILURE, "%s", command);
  }

   while(fgets(buf, sizeof(buf)-1, fp) != NULL){
     ret = send(sock, buf, strlen(buf), 0);
    if(ret < 0){
      shutdown(sock, SHUT_RDWR);
      close(sock);
      break;
    }
  }
  pclose(fp);
}

int exp1_http_session(int sock)
{
  char buf[2048];
  int recv_size = 0;
  exp1_info_type info;
  int ret = 0;

  while(ret == 0){
    int size = recv(sock, buf + recv_size, 2048, 0);

    if(size == -1){
      return -1;
    }

    recv_size += size;
    ret = exp1_parse_header(buf, recv_size, &info);
  }

  exp1_http_reply(sock, &info);

  return 0;
}

void exp1_check_file(exp1_info_type *info)
{
  struct stat s;
  int ret;
  char* pext;
  sprintf(info->real_path, "html%s", info->path);
  ret = stat(info->real_path, &s);
  printf("%d\n", ret);
  if((s.st_mode & S_IFMT) == S_IFDIR){
    sprintf(info->real_path, "%s/index.html", info->real_path);
  }
  ret = stat(info->real_path, &s);
  printf("real path %s\n", info->path);
  if(ret == -1){ /*retはreal_pathの状態を取得できたかどうかを調べる変数。もしreal_pathがなかったら取得失敗として-1を返す。よってNOT FOUNDとなる。*/
    info->code = 404;
  }else if(ret == 0){
    if((s.st_mode & S_IRWXO) != S_IROTH){ /*S_IRWXOとは、「他人 (others) のアクセス許可用のビットマスク 」のことで、このうち実行権限(X)が与えられていなければ403Forbiddenのページを開くようになっている。*/
      info->code = 403;
    }else if(strcmp(info->path , "/301.html") == 0){
        info->code = 301;
    }else if(strcmp(info->path , "/302.html") == 0){
        info->code = 302;
    }else if(strcmp(info->path , "/303.html") == 0){
        info->code = 303;
    }else if(strcmp(info->path , "/418.html") == 0){
        info->code = 418;
    }else{
    info->code = 200;
  }
    info->size = (int) s.st_size;
  }

  pext = strstr(info->path, ".");
  if(pext != NULL && strcmp(pext, ".html") == 0){
    strcpy(info->type, "text/html");
  }else if(pext != NULL && strcmp(pext, ".css") == 0){
    strcpy(info->type, "text/css");
  }else if(pext != NULL && strcmp(pext, ".jpg") == 0){
    strcpy(info->type, "image/jpeg");
  }else if(pext != NULL && strcmp(pext, ".php") == 0){
    strcpy(info->type,"text/php");
  }else if(pext != NULL && strcmp(pext, ".mp4") == 0){
    strcpy(info->type, "video/mp4");
  }else if(pext != NULL && strcmp(pext, ".png") == 0){
    strcpy(info->type, "image/png");
  }else if(pext != NULL && strcmp(pext, ".gif") == 0){
    strcpy(info->type, "image/gif");
  }else if(pext != NULL && strcmp(pext, ".js") == 0){
    strcpy(info->type, "application/javascript");
  }else if(pext != NULL && strcmp(pext, ".json") == 0){
    strcpy(info->type, "application/json");
  }else if(pext != NULL && strcmp(pext, ".xml") == 0){
    strcpy(info->type, "application/xml");
  }else if(pext != NULL && strcmp(pext, ".zip") == 0){
    strcpy(info->type, "application/zip");
  }else if(pext != NULL && strcmp(pext, ".pdf") == 0){
    strcpy(info->type, "application/pdf");
  }else if(pext != NULL && strcmp(pext, ".xml") == 0){
    strcpy(info->type, "application/xml");
  }else if(pext != NULL && strcmp(pext, ".mp3") == 0){
    strcpy(info->type, "audio/mpeg");
  }
}

void exp1_parse_status(char* status, exp1_info_type *pinfo)
{
  char cmd[1024];
  char path[1024];
  char* pext;
  int i, j;

  enum state_type
  {
    SEARCH_CMD,
    SEARCH_PATH,
    SEARCH_END
  }state;

  state = SEARCH_CMD;
  j = 0;
  for(i = 0; i < strlen(status); i++){
    switch(state){
    case SEARCH_CMD:
      if(status[i] == ' '){
        cmd[j] = '\0';
        j = 0;
        state = SEARCH_PATH;
      }else{
        cmd[j] = status[i];
        j++;
      }
      break;
    case SEARCH_PATH:
      if(status[i] == ' '){
        path[j] = '\0';
        j = 0;
        state = SEARCH_END;
      }else{
        path[j] = status[i];
        j++;
      }
      break;
    }
  }
  strcpy(pinfo->cmd, cmd);
  strcpy(pinfo->path, path);
}

int exp1_parse_header(char* buf, int size, exp1_info_type* info)
{
  char status[1024];
  int i, j;

  enum state_type
  {
    PARSE_STATUS,
    PARSE_END
  }state;

  state = PARSE_STATUS;
  j = 0;
  for(i = 0; i < size; i++){
    switch(state){
    case PARSE_STATUS:
      if(buf[i] == '\r'){
        status[j] = '\0';
        j = 0;
        state = PARSE_END;
        exp1_parse_status(status, info);
        exp1_check_file(info);
      }else{
        status[j] = buf[i];
        j++;
      }
      break;
    }

    if(state == PARSE_END){
      return 1;
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  int sock_listen;

  if(argc != 2){
    printf("usage %s [port]\n",argv[0]);
    exit(-1);
  }

  sock_listen = exp1_tcp_listen(atoi(argv[1]));

  while(1){
    struct sockaddr addr;
    int sock_client;
    int len;

    sock_client = accept(sock_listen, &addr, (socklen_t*) &len);
    exp1_http_session(sock_client);

    shutdown(sock_client, SHUT_RDWR);
    close(sock_client);
  }
  exit(0);
}
