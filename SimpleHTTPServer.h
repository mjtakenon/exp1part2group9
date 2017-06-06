#include "exp1_http.h"

void exp1_send_file(int sock, char* filename);
void exp1_send_404(int sock);
void exp1_http_reply(int sock,exp1_info_type *info);
void exp1_send_php(int sock, char* filename);
int exp1_http_session(int sock);
void exp1_check_file(exp1_info_type *info);
void exp1_parse_status(char* status, exp1_info_type *pinfo);
int exp1_parse_header(char* buf, int size, exp1_info_type* info);
int main(int argc, char** argv);
