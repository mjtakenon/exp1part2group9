#ifndef __EXP1_HTTP_H__
#define __EXP1_HTTP_H__

enum{
	/*exp1_https_session*/
	EXP1HTTPSSESSIONBUFSIZE    = 2048,
	/*typedef struct exp1_info_type*/
	EXP1HTTPSMETHODSIZE        = 64,
	EXP1HTTPSURIPATHSIZE       = 256,
	EXP1HTTPSREALPATHSIZE      = 256,
	EXP1HTTPSCONTENTTYPESIZE   = 64,
	/*exp1_parse_header*/
	EXP1HTTPSSTATUSSIZE        = 1024,
	/*exp1_https_reply*/
	EXP1HTTPSHEADERSIZE        = 16384,
	/*exp1_send_file
	 *url: http://www.infraexpert.com/info/5adsl.htm
	 *Ethernet LAN MTU = Ethernet frame(1518) - Ethernet header & FCS(18) = 1500
	 *PPPoE Ethernet LAN MTU = Ethernet LAN MTU - PPPoE header(6) - PPP header(2) = 1492
	 *Ethernet LAN MSS = MTU - IP header(20) - TCP header(20) - 1460
	 *PPPoE Ethernet LAN MTU = PPPoE Ethernet LAN MTU - IP header(20) - TCP header(20) - 1452
	 */
	EXP1HTTPSETHERNETLANMSS    = 1460
};

typedef struct{
	char cmd[EXP1HTTPSMETHODSIZE];
	char path[EXP1HTTPSURIPATHSIZE];
	char real_path[EXP1HTTPSREALPATHSIZE];
	char type[EXP1HTTPSCONTENTTYPESIZE];
	int code;
	int size;
} exp1_info_type;

extern int  exp1_https_session(SSL *ssl);

extern int  exp1_parse_header(char *buf, int size, exp1_info_type *info);

extern void exp1_parse_status(char *status, exp1_info_type *pinfo);

extern void exp1_check_file(exp1_info_type *info);

extern void exp1_https_reply(SSL *ssl, exp1_info_type *info);

extern void exp1_send_404(SSL *ssl);

extern void exp1_send_file(SSL *ssl, char *filename);

extern int  exp1_tcp_listen(int port);

#endif /*__EXP1_HTTP_H__*/
