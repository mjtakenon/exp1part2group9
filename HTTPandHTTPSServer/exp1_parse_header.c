#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

int exp1_parse_header(char *buf, int size, exp1_info_type *info){
	char status[EXP1HTTPSSTATUSSIZE];
	int i, j;

	enum state_type{
		PARSE_STATUS,
		PARSE_END
	}state;

	state = PARSE_STATUS;
	j = 0;
	for(i = 0;i < size;i++){
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
			case PARSE_END:
				break;
		}
		if(state == PARSE_END){
			return 1;
		}
	}
	return 0;
}
