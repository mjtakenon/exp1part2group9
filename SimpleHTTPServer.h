#include "exp1_http.h"


char* base64_d(char* str_base64);
char* base64_e(char* str_base64);
char printline(char *c);
int user_pass_exist(char *pass);

void exp1_send_file(int sock, char* filename);
void exp1_send_301(int sock);
void exp1_send_302(int sock);
void exp1_send_303(int sock);
void exp1_send_401(int sock, exp1_info_type *info);
void exp1_send_403(int sock);
void exp1_send_404(int sock);
void exp1_send_418(int sock);
void exp1_http_reply(int sock,exp1_info_type *info);
void exp1_send_php(int sock, char* filename);
int exp1_http_session(int sock);
void exp1_check_file(exp1_info_type *info);
void exp1_parse_status(char* status, exp1_info_type *pinfo);
int exp1_parse_header(char* buf, int size, exp1_info_type* info);
int main(int argc, char** argv);
