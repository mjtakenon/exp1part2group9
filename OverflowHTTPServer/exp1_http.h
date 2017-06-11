#ifndef __EXP1_HTTP_H__
#define __EXP1_HTTP_H__

enum{
	/*exp1_http_session*/
	EXP1HTTPSESSIONBUFSIZE    = 2048,
	/*typedef struct exp1_info_type*/
	EXP1HTTPMETHODSIZE        = 64,
	EXP1HTTPURIPATHSIZE       = 256,
	EXP1HTTPREALPATHSIZE      = 256,
	EXP1HTTPCONTENTTYPESIZE   = 64,
	/*exp1_parse_header*/
	EXP1HTTPSTATUSSIZE        = 1024,
	/*exp1_http_reply*/
	EXP1HTTPHEADERSIZE        = 16384,
	/*exp1_send_file
	 *url: http://www.infraexpert.com/info/5adsl.htm
	 *Ethernet LAN MTU = Ethernet frame(1518) - Ethernet header & FCS(18) = 1500
	 *PPPoE Ethernet LAN MTU = Ethernet LAN MTU - PPPoE header(6) - PPP header(2) = 1492
	 *Ethernet LAN MSS = MTU - IP header(20) - TCP header(20) - 1460
	 *PPPoE Ethernet LAN MTU = PPPoE Ethernet LAN MTU - IP header(20) - TCP header(20) - 1452
	 */
	EXP1HTTPETHERNETLANMSS    = 1460
};

typedef struct{
	char cmd[EXP1HTTPMETHODSIZE];
	char path[EXP1HTTPURIPATHSIZE];
	char real_path[EXP1HTTPREALPATHSIZE];
	char type[EXP1HTTPCONTENTTYPESIZE];
	int code;
	int size;
} exp1_info_type;

extern int  exp1_http_session(int sock);

extern int  exp1_parse_header(char *buf, int size, exp1_info_type *info);

extern void exp1_parse_status(char *status, exp1_info_type *pinfo);

extern void exp1_check_file(exp1_info_type *info);

extern void exp1_http_reply(int sock, exp1_info_type *info);

extern void exp1_send_404(int sock);

extern void exp1_send_file(int sock, char *filename);

extern int  exp1_tcp_listen(int port);

#endif /*__EXP1_HTTP_H__*/
