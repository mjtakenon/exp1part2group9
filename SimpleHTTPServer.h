#include "exp1_http.h"


enum exp1_myws_cons{
	/*exp1_myws*/
	EXP1ISFIN      = 0x80,
	EXP1RSV1       = 0x40,
	EXP1RSV2       = 0x20,
	EXP1RSV3       = 0x10,
	EXP1OPCODE     = 0x0f,
	EXP1ISMASK     = 0x80,
	EXP1PAYLOAD    = 0x7f,
	EXP1MASKLENGTH = 4,
	/*exp1_set_ws*/
	EXP1SETFIN    = 0x01,
	EXP1SETRSV1   = 0x02,
	EXP1SETRSV2   = 0x04,
	EXP1SETRSV3   = 0x08,
	EXP1SETMASK   = 0x10,
	/*exp1_parse_ws*/
	EXP1WSERROR   = -1,
	EXP1WSSUCCESS = 0,
	EXP1PAY2BYTE  = 0x7e,
	EXP1PAY8BYTE  = 0x7f
};
/*
 * isfin: 1bit
 * rsv1 : 1bit
 * rsv2 : 1bit
 * rsv3 : 1bit
 * opcode : 4bit
 * ismask : 1bit
 * payload : 7bit
 * extrapayload : 64bit
 * maskkey : 32bit
 * payloaddata : payload byte
 */
/*
 * firsthead  = isfin + rsv1 + rsv2 + rsv3 + opcode
 * secondhead = ismask + payload
 */
typedef struct {
	unsigned char firsthead;
	unsigned char secondhead;
	long long int payload;
	unsigned char maskkey[EXP1MASKLENGTH];
	char *payloaddata;
} exp1_myws;


char* base64_d(char* str_base64);
char* base64_e(char* str_base64);
void printline(char *c);
int user_pass_exist(char *pass);
void input_base64(char* status,exp1_info_type *info);

char* get_md5(char* data);
void parse_char(char *line,char *res,char start,char end);
void input_md5(char *pass ,exp1_info_type *info);

char *sha1_to_hex(const unsigned char *sha1);
char *calc_sha1(const char *body);
void input_wskey(char *buf,exp1_info_type *info);
void exp1_send_101(int sock,exp1_info_type *info);
int exp1_parse_ws(char *buf, exp1_myws *myws);
int exp1_set_ws(char *buf, int len, int opcode, exp1_myws *myws, int flag);
char *exp1_do_php(int argc,char argv[4][256]);

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
