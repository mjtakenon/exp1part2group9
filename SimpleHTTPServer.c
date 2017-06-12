#include "SimpleHTTPServer.h"

/*ちょっときもいけどごめんね*/
char hexbuffer[4][40 + 1];
int bufno = 0;

/*佐藤君実装auth64のエンコードデコード実装*/
char* base64_d(char* str_base64){	/*decode*/

	char tmpfile[] = "tmp.txt";
	char cmd[1024];
	char *s;
	FILE *fp;
	fp = fopen("tmp.txt", "w+");
    fclose(fp);
	sprintf(cmd, "echo %s | base64 -d > %s", str_base64, tmpfile);
	system(cmd);

    fp = fopen("tmp.txt", "r");
	do{
		s = (char *)malloc(sizeof(char) * 100);
	}while(s == NULL);

	fgets(s, 100, fp);/*名前しか取得できなかったぞ馬鹿やろう*/
	remove(tmpfile);

	return s;
}

char* base64_e(char* str_base64){	/*encode*/

    char tmpfile[] = "tmp.txt";
	char cmd[1024];
	char *s;
	FILE *fp;

	printf("base64e\n");
	do{
		s = (char *)malloc(sizeof(char) * 100);
	}while(s == NULL);

	sprintf(cmd, "(echo -en \"%s\" | base64) > %s", str_base64, tmpfile);
	printf("base : %s\ncmd : %s\n",str_base64,cmd);
	printf("system : %d\n", system(cmd) );/*32512が帰ってくる。なんか挙動が良く分からない。*/
	if((fp = fopen("tmp.txt", "r")) == NULL){
		printf("not found base file\n");
	}
	fgets(s, 100, fp);
	/*remove(tmpfile);*/
	printf("s : %s\n",s);
	return s;
}
/*ここまで*/

/*digest認証*/
char* get_md5(char* data){
    MD5_CTX c;

    unsigned char md[MD5_DIGEST_LENGTH];
    char *mdString;
    int r, i;

	do{
		mdString = (char *)malloc(sizeof(char) * 33);
	}while(mdString == NULL);

    r = MD5_Init(&c);
    if(r != 1) {
        perror("init");
        exit(1);
    }

    r = MD5_Update(&c, data, strlen(data));
    if(r != 1) {
        perror("update");
        exit(1);
    }

    r = MD5_Final(md, &c);
    if(r != 1) {
        perror("final");
        exit(1);
    }

    for(i = 0; i < 16; i++)
         sprintf(&mdString[i * 2], "%02x", (unsigned int)md[i]);

    return mdString;
}
/*ここまで*/

/*sha1ハッシュ実装*/
char *sha1_to_hex_r(char *buffer, const unsigned char *sha1)
{
	char hex[] = "0123456789abcdef";
	char *buf = buffer;
	int i;
	unsigned int val;

	for (i = 0; i < 20; i++) {
		val = *sha1++;
		*buf++ = '\\';
		*buf++ = 'x';
		*buf++ = hex[val >> 4];
		*buf++ = hex[val & 0xf];
	}
	*buf = '\0';

	return buffer;
}

char *sha1_to_hex(const unsigned char *sha1)
{
	return sha1_to_hex_r(hexbuffer[3 & ++bufno], sha1);
}

char *calc_sha1(const char *body)
{
    SHA_CTX c;
    unsigned char sha1[41];

    SHA1_Init(&c);
    SHA1_Update(&c, body, strlen(body));
    SHA1_Final(sha1, &c);

    return sha1_to_hex(sha1);

}
/*ここまで。帰ってきたポインタは必ずfreeする事*/

/*websocket header set*/
int exp1_set_ws(char *buf, int len, int opcode, exp1_myws *myws, int flag){
	int i;
	int j;
	int headersize;
	char *changebuf;
	/*set myws*/
	headersize = 2;
	memset(myws, '\0', sizeof(exp1_myws));

	if((flag & EXP1SETFIN) != 0){
		myws->firsthead |= EXP1ISFIN;
	}
	if((flag & EXP1SETRSV1) != 0){
		myws->firsthead |= EXP1RSV1;
	}
	if((flag & EXP1SETRSV2) != 0){
		myws->firsthead |= EXP1RSV2;
	}
	if((flag & EXP1SETRSV3) != 0){
		myws->firsthead |= EXP1RSV3;
	}
	myws->firsthead |= (opcode & 0x0f);

	if((flag & EXP1SETMASK) != 0){
		myws->secondhead |= EXP1ISMASK;
		headersize += EXP1MASKLENGTH;

		srand((unsigned)time(NULL));
		for(i = 0;i < EXP1MASKLENGTH;i++){
			myws->maskkey[i] = rand() % 0xff;
		}
	}

	if(0x7d < len && len < 0x8000){
		myws->secondhead |= 0xfe;
		headersize += 2;
	}else if(0x7fff < len){
		myws->secondhead |= 0xff;
		headersize += 8;
	}else{
		myws->secondhead |= (len & 0xff);
	}
	myws->payload = len;

	/*set send message*/
	do{
		changebuf = (char *)malloc(sizeof(char) * (headersize + 1));
	}while(changebuf == NULL);

	changebuf[0] = myws->firsthead;
	changebuf[1] = myws->secondhead;
	j = 2;
	if((myws->secondhead & EXP1PAYLOAD) == 0x7e){
		for(i = j;i < j + 2;i++){
			changebuf[i] = ((myws->payload >> ((j + 2 - i - 1) * 8))&0xff);
		}
		j += 2;
	}else if((myws->secondhead & EXP1PAYLOAD) == 0x7f){
		for(i = j;i < j + 8;i++){
			changebuf[i] = ((myws->payload >> ((j + 8 - i - 1) * 8))&0xff);
		}
		j += 8;
	}
	if((flag & EXP1SETMASK) != 0){
		for(i = j;i < j + 4;i++){
			changebuf[i] = myws->maskkey[i - j];
		}
		j += 4;
	}
	for(i = len;i >= 0;i--){
		buf[i + j] = buf[i];
	}
	for(i = 0;i < j;i++){
		buf[i] = changebuf[i];
	}
	len += headersize;
	free(changebuf);

	myws->payloaddata = buf + headersize;

	for(j = 0;j < 20;j++){
		int h = 128;
		for(i = 0;i < 8;i++){
			printf("%d", ((buf[j] & h) != 0 ? 1 : 0));
			h/=2;
		}
		printf("\n");
	}

	return len;
}
/*recv websocket header parse and set exp1_myws*/
int exp1_parse_ws(char *buf, exp1_myws *myws){
	int i;
	int j;
	int paybyte;
	char *hbuf;

	hbuf = buf;
	memset(myws, '\0', sizeof(exp1_myws));
	myws->firsthead = *buf++;
	if((myws->firsthead & (EXP1RSV1 | EXP1RSV2 | EXP1RSV3)) != 0){
		return EXP1WSERROR;
	}

	myws->secondhead = *buf++;
	/*
	paybyte = 128;
	for(i = 0;i < 8;i++){
		printf("%d", ((myws->secondhead & paybyte) != 0 ? 1 : 0));
		paybyte/=2;
	}*/
	printf("%d\n", myws->secondhead);
	switch(myws->secondhead & EXP1PAYLOAD){
		case EXP1PAY2BYTE:
			paybyte = 2;
			break;
		case EXP1PAY8BYTE:
			paybyte = 8;
			break;
		default:
			myws->payload = (myws->secondhead & EXP1PAYLOAD);
			paybyte = 0;
			break;
	}
	for(i = 0;i < paybyte;i++){
		myws->payload = (myws->payload << 8) + (unsigned char)*buf++;
	}
	if((myws->secondhead & EXP1ISMASK) != 0){
		for(i = 0;i < EXP1MASKLENGTH;i++){
			myws->maskkey[i] = *buf++;
		}
	}
	do{
		myws->payloaddata = (char *)malloc(sizeof(char)*(myws->payload));
	}while(myws->payloaddata == NULL);

	for(i = 0;i < myws->payload;i++){
		myws->payloaddata[i] = buf[i];
	}

	buf = hbuf;

	for(j = 0;j < 20;j++){
		int h = 128;
		for(i = 0;i < 8;i++){
			printf("%d", ((buf[j] & h) != 0 ? 1 : 0));
			h/=2;
		}
		printf("\n");
	}



	return EXP1WSSUCCESS;
}

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

void input_wskey(char *buf,exp1_info_type *info) {

	buf = strstr(buf,"Sec-WebSocket-Key:");
	parse_char(buf,info->wskey,' ','\r');
}

void exp1_send_101(int sock,exp1_info_type *info){
	char buf[2048];
	char *key;
	int i;
    int ret;
	int len;
	char *p;
	int k=0;
	char php[4][256] = {"./html/getkey.php","id","text","true"};
	exp1_myws myws;

	printf("send 101\n");
	/*keyの計算*/
	sprintf(buf,"%s258EAFA5-E914-47DA-95CA-C5AB0DC85B11",info->wskey);
	/*sprintf(buf,"dGhlIHNhbXBsZSBub25jZQ==258EAFA5-E914-47DA-95CA-C5AB0DC85B11");*/
	printf("a\n");
	key = calc_sha1(buf);
	printf("shakey : %s\n",key);
	key = base64_e(key);
	printf("basekey : %s\n",key);

	/*buf fix*/
	p = strchr(key, '\n');
	if(p != NULL){
		*p = '\0';
	}


    len = sprintf(buf, "HTTP/1.1 101 Switching Protocols\r\n");
	len += sprintf(buf + len,"Upgrade: websocket\r\n");
	len += sprintf(buf + len,"Connection: Upgrade\r\n");
	len += sprintf(buf + len,"Sec-WebSocket-Accept: %s\r\n",key);
	len += sprintf(buf + len,"\r\n");

	free(key);

	/*len = exp1_set_ws(buf, len, 0x00, &myws, EXP1SETFIN);*/
	ret = send(sock, buf, len, 0);
	p = exp1_do_php(1,php);
	strcpy(php[0],"./html/chat.php");
	strcpy(php[1],p);
	free(p);
	while ( ret > 0 ) {

		k++;
		memset(buf, '\0', sizeof(buf[0]) * 2048);
		ret = recv(sock, buf, 2048, 0);
		printf("\n\nret ; %d \n",ret);
		exp1_parse_ws(buf, &myws);
		/*first is 0x00, so buf might show NULL.*/
		if ((myws.firsthead & 0x08) != 0) {
			printf("終了\n");
			break;
		}
		printf("%x\n",(myws.firsthead & 0x08));

		for (i = 0; i < myws.payload; i++) {
			myws.payloaddata[i] = myws.payloaddata[i] ^ myws.maskkey[i%EXP1MASKLENGTH];
		}
		printf("%s\n",myws.payloaddata);
		strcpy(php[2],myws.payloaddata);

		free(myws.payloaddata);
		p = exp1_do_php(4,php);
		len = sprintf(buf,"%s",p);
		free(p);
		len = exp1_set_ws(buf, len, 0x01, &myws, EXP1SETFIN);
		/*
		for (i = 0; i < 6; i++) {
			printf("%04x\n",buf[i]);
		}*/
		ret = send(sock,buf,len,0);
		printf("ret : %d\n",ret);
		strcpy(php[3],"false");
		//printf("ここらへん?\n");

	    //printf("buf : %s", buf);
	    //ret = send(sock, buf, strlen(buf)-1, 0);
		//recv(sock, buf, 2048, 0);
		/*printf("packet : %s\n",buf);

		recv(sock, buf, 2048, 0);
		exp1_parse_ws(buf, &myws);
		/*first is 0x00, so buf might show NULL.*/
		/*
		for (i = 0; i < myws.payload; i++) {
			myws.payloaddata[i] = myws.payloaddata[i] ^ myws.maskkey[i%EXP1MASKLENGTH];
		}
		printf("%s\n",myws.payloaddata);*/
		printf("\n%d\n",k);
	}
	printf("でてきたよ\n");
    shutdown(sock, SHUT_RDWR);
    close(sock);
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

void exp1_send_401(int sock, exp1_info_type *info)
{
    char buf[16384];
    int ret;
    int len;
      if (user_pass_exist(info->auth)) {
          len = sprintf(buf,"HTTP/1.0 200 OK\r\n");
          /*len += sprintf(buf + len,"Content-Length: %d\r\n", info->size);*/
          len += sprintf(buf + len,"Content-Type: %s\r\n", "text.html");
          len += sprintf(buf + len,"\r\n");

          ret = send(sock,buf,strlen(buf),0);
		  exp1_send_file(sock,"./html/DigestSUCCESS.html");
          if (ret < 0) {
              shutdown(sock, SHUT_RDWR);
              close(sock);
          }
          return;
      }
      len = sprintf(buf,"HTTP/1.0 401 Authorization Required\r\n");
      len += sprintf(buf + len,"WWW-Authenticate: Basic realm=\"Secret Room of Takeuchi\"\r\n");
      /*len += sprintf(buf + len,"Content-Length: %d\r\n", info->size);*/
      len += sprintf(buf + len,"Content-Type: %s\r\n", "text/html");
      len += sprintf(buf+len,"\r\n");

      printf("%s",buf);
      ret = send(sock,buf,strlen(buf),0);
	  exp1_send_file(sock,"./html/DigestFAILED.html");

      if (ret < 0) {
          shutdown(sock, SHUT_RDWR);
          close(sock);
      }
}

/*Digest認証の実装*/
void parse_char(char *line,char *res,char start,char end) {
  int i=0;
  int j=0;

	if (line == NULL) {
        printf("not status\n");
        return;
    }
    while (line[i] != start) {i++;}
    i++;
    while (line[i] != end){
        res[j] = line[i];
        j++;i++;
    }
    res[j]='\0';
}
void input_md5(char *pass ,exp1_info_type *info){
    char line[64];
    char *point = pass;
	/*printf("Digest_data : %s\n\n",pass);*/
    Digest_data *data = malloc(sizeof(Digest_data));
     pass = strstr(pass,"username");
     parse_char(pass,line,'\"','\"');
     strcpy(data->username,line);
     pass = point;

     pass = strstr(pass,"nonce");
     parse_char(pass,line,'\"','\"');
     strcpy(data->nonce,line);
     pass = point;

     pass = strstr(pass,"uri");
     parse_char(pass,line,'\"','\"');
     strcpy(data->uri,line);
     pass = point;

     pass = strstr(pass,"nc=");
	 /*printf("nc : %s\n\n",pass);*/
     parse_char(pass,line,'=',',');
     strcpy(data->nc,line);
     pass = point;

     pass = strstr(pass,"cnonce");
     parse_char(pass,line,'\"','\"');
     strcpy(data->cnonce,line);
     pass = point;

     pass = strstr(pass,"response");
     parse_char(pass,line,'\"','\"');
     strcpy(data->response,line);
     pass = point;

     info->digest = data;
}

void print_md5(exp1_info_type *info){
    printf("print_md5\n");
    printf("username : %s\n",info->digest->username);
    printf("nonce : %s\n",info->digest->nonce);
    printf("uri : %s\n",info->digest->uri);
    printf("nc : %s\n",info->digest->nc);
    printf("cnonce : %s\n",info->digest->cnonce);
    printf("response : %s\n",info->digest->response);
}

int digest_path(Digest_data *digest){
    char A1[64];
    char A2[64];
    char response[64];
    char buf[512];
	char *ret;
	FILE *fp;

	if (digest == NULL) {
		printf("no digest input\n");
		return 0;
	}
	if((fp = fopen("htaccessD","r")) == NULL){
		printf("file no exist\n");
		return 0;
	}
	ret = fgets(buf,64,fp);
	while (ret != NULL) {
		strtok(buf,"\r\n");;
		/*A1 = ユーザ名 ":" realm ":" パスワード*/
	    strcpy(A1,get_md5(buf));

	    /*A2 = HTTPのメソッド ":" コンテンツのURI*/
	    sprintf(buf,"GET:%s",digest->uri);
	    strcpy(A2,get_md5(buf));

	    /*response = MD5( MD5(A1) ":" nonce ":" nc ":" cnonce ":" qop ":" MD5(A2) )*/
	    sprintf(buf,"%s:%s:%s:%s:auth:%s",A1,digest->nonce,digest->nc,digest->cnonce,A2);
	    strcpy(response,get_md5(buf));

	    printf("\nresponse : %s\n",digest->response);
	    printf("calcresp : %s\n",response);

	    if (strcmp(digest->response,response) == 0) {
	        return 1;
	    }
		ret = fgets(buf,64,fp);
	}
	fclose(fp);
    return 0;
}
void exp1_send_4012(int sock,exp1_info_type *info){
    char buf[16384];
    int ret;
    int len;
    if (info->digest != NULL) {
        print_md5(info);
    }
    if (digest_path(info->digest)) {
        len = sprintf(buf,"HTTP/1.0 200 OK\r\n");
        /*len += sprintf(buf + len,"Content-Length: %d\r\n", info->size);*/
        len += sprintf(buf + len,"Content-Type: %s\r\n", "text/html");
        len += sprintf(buf + len,"\r\n");


        ret = send(sock,buf,strlen(buf),0);
		exp1_send_file(sock,"./html/DigestFAILED.html");

        if (ret < 0) {
            shutdown(sock, SHUT_RDWR);
            close(sock);
        }
        return;
    }
    len = sprintf(buf,"HTTP/1.0 401 Authorization Required\r\n");
    len += sprintf(buf + len,"WWW-Authenticate: Digest realm=\"Digest\", ");
    len += sprintf(buf + len,"nonce=\"bakajane-no\", algorithm=MD5, qop=\"auth\"\r\n");
    /*len += sprintf(buf + len,"Content-Length: %d\r\n", info->size);*/
    len += sprintf(buf + len,"Content-Type: %s\r\n", "text/html");
    len += sprintf(buf+len,"\r\n");


    printf("%s",buf);
    ret = send(sock,buf,strlen(buf),0);
	exp1_send_file(sock,"./html/DigestFAILED.html");
    if (ret < 0) {
        shutdown(sock, SHUT_RDWR);
        close(sock);
    }
}
/*ここまでがDigest*/

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
    printf("404 not found %s\n\n", info->path);
    return;
  }

  if(info->code == 401){
        exp1_send_401(sock,info);
        printf("401 basic\n\n");
        return;
  }
  if(info->code == 4012){
	  printf("401 digest\n\n");
	  exp1_send_4012(sock,info);
	  return;
  }
  if (info->code == 101) {
	  printf("101 websocket upgrade\n\n");

	  exp1_send_101(sock,info);
	  return;
  }


  len = sprintf(buf, "HTTP/1.0 200 OK\r\n");
  len += sprintf(buf + len, "Content-Length: %d\r\n", info->size);
  if (strcmp(info->type,"text/php")==0) {
  	len += sprintf(buf + len, "Content-Type: %s\r\n", "text/html");
  }else{
	len += sprintf(buf + len, "Content-Type: %s\r\n", info->type);
  }
  len += sprintf(buf + len, "\r\n");

  ret = send(sock, buf, len, 0);
  if(ret < 0){
    shutdown(sock, SHUT_RDWR);
    close(sock);
    return;
  }

  if(strcmp(info->type,"text/php") == 0){
	  if (strcmp(info->cmd,"POST")==0){
  		/*使ってないからbufを使いましょう*/
		setenv("POST_STR","",1);/*初期化*/
		sprintf(buf,"echo $%s=\"%s\"","POST_STR",info->post);
	  	setenv("POST_STR",info->post,1);
  	}
    exp1_send_php(sock, info->real_path);
  }
  else{
    exp1_send_file(sock, info->real_path);
  }
}


char *exp1_do_php(int argc,char argv[4][256])
{
  char buf[16384];
  char *ret;
  int i;
  FILE* fp;

  do {
  	ret = (char*)malloc(sizeof(char) * 1024);
} while(ret == NULL);

  sprintf(buf,"/usr/bin/php %s",argv[0]);
  for (i = 1; i < argc; i++) {
	sprintf(buf,"%s %s",buf,argv[i]);
 }

  printf("commad : %s\n",buf);

  if ( (fp=popen(buf,"r")) == NULL) {
      /* err(EXIT_FAILURE, "%s", command); */
	  exit(0);
  }

  fgets(buf, sizeof(buf)-1, fp);
  strcpy(ret,buf);
  pclose(fp);

  return ret;/*絶対freeする事*/

}

void exp1_send_php(int sock, char* filename)
{
  char buf[16384];
  int ret;
  FILE* fp;

  char command[256];
  sprintf(command,"/usr/bin/php %s",filename);

  if ( (fp=popen(command,"r")) == NULL) {
      /* err(EXIT_FAILURE, "%s", command); */
	  exit(0);
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

/*追加(basic認証格納用)*/
void input_base64(char* status,exp1_info_type *info){
    int i=0,j=0;
    char base[256];
    status=strstr(status,"Basic");
    while(status[i] != ' '){i++;}
    i++;

    while(status[i] != '\r')
    {
        base[j] = status[i];
        j++;
        i++;
    }
    base[j] = '\0';

    strcpy(info->auth,base);

}
/*ここまで*/
/*デバック用*/
void printline(char *c){
    int i;
    while(c[i] != '\0'){
        printf("%d ",(int)c[i]);
        i++;
    }
    printf("\n");
}

/*ユーザーパスワードが同じか比較関数*/
int user_pass_exist(char *pass){
    FILE *fp;
    char line[64];
    char *ret;
    if(pass == NULL){
      printf("pass is null\n");
        return 0;
    }
    if((fp = fopen("htaccess","r")) == NULL){
      printf("file not found \n");
        return 0;
    }
    /*printf("%s\n",pass);*/
    /*printline(pass);*/
    ret = fgets(line,64,fp);
    while (ret != NULL) {
      /*fileから読み込んだ行の改行コードの削除*/
      strtok(line,"\r\n");
      printf("pass=%s\nline=%s\n",pass,line);
        if (strcmp(pass,line) == 0) {
            return 1;
        }
        /*printf("%s\n",line);*/
        /*printline(line);*/
        ret = fgets(line,64,fp);
    }
    fclose(fp);
    return 0;
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
	}else if(size == 0){
	  return 0;
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
  /*printf("%d\n", ret);*/
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
  /*追加(basic認証)*/
  pext = strstr(info->path, "/secure");
  if (pext != NULL && strcmp(pext,"/secure")==0){
      info->code = 401;
  }
  /*ここまで*/

  /*追加(Dijest認証)ごみみたいな分岐でごめんなさい*/
  if (pext != NULL && strcmp(pext,"/secure2")==0) {
	  info->code = 4012;
  }
  /*ここまで*/

  strcpy(info->type,"");/*ちょっと修正、多分これでコンテンツ間違い直るはず*/
  pext = strstr(info->path, ".");
  if(pext != NULL && strcmp(pext, ".html") == 0){
    strcpy(info->type, "text/html");
  }else if(pext != NULL && strcmp(pext, ".css") == 0){
    strcpy(info->type, "text/css");
  }else if(pext != NULL && strcmp(pext, ".jpg") == 0){
    strcpy(info->type, "image/jpeg");
  }else if(pext != NULL && strcmp(pext, ".php") == 0){
    strcpy(info->type,"text/php");/*phpのままだとダウンロードされる*/
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
  }else{
	strcpy(info->type, "text/html");
  }
}

void exp1_parse_status(char* status, exp1_info_type *pinfo)
{
  char cmd[1024];
  char path[1024];
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
	case SEARCH_END:
	  break;
    }
  }
  strcpy(pinfo->cmd, cmd);
  strcpy(pinfo->path, path);
}

int exp1_parse_header(char* buf, int size, exp1_info_type* info)
{
  char status[1024];
  int i,j,cp=0,post=0;
  char* pass;
  char *base64d;

  enum state_type
  {
    PARSE_STATUS,
    PARSE_END
  }state;
  printf("buf : \n%s\n",buf);
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
        if(strcmp(info->cmd,"POST")==0){
          pass=strstr(buf,"\r\n\r\n");
          if (pass != NULL) {
            while(pass[cp]=='\n' || pass[cp]=='\r'){cp++;}

            while(pass[cp]!='\r')
            {
              info->post[post] = pass[cp];
              cp++;
              post++;
            }
            info->post[post]='\0';
            /*printf("post:%s\n",info->post);*/
          }
		  else{
			  strcpy(info->post,"");
		  }
        }

        exp1_check_file(info);
      }else{
        status[j] = buf[i];
        j++;
      }
      break;

	case PARSE_END:
	  break;
    }

    if(state == PARSE_END){
        /*Basic認証用文字列取得*/
        pass = strstr(buf,"Basic");
        if (pass != NULL) {
			input_base64(pass,info);
			base64d = base64_d(info->auth);
            strcpy(info->auth, base64d);
			free(base64d);
        }
		/*Digest認証用文字列取得*/
		pass = strstr(buf,"Authorization: Digest");
		if (pass != NULL) {
			input_md5(pass,info);
		}else{
			info->digest = NULL;
		}
		/*webソケット用*/
		pass = strstr(buf,"Upgrade: websocket");
		if (pass != NULL) {
			input_wskey(buf,info);
			info->code = 101;
		}
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
