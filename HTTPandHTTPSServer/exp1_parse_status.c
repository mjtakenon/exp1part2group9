#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

void exp1_parse_status(char *status, exp1_info_type *pinfo){
	char cmd[EXP1HTTPSMETHODSIZE];
	char path[EXP1HTTPSURIPATHSIZE];
	/*char *pext;*/
	int i, j;

	enum state_type{
		SEARCH_CMD,
		SEARCH_PATH,
		SEARCH_END
	}state;

	state = SEARCH_CMD;
	j = 0;
	for(i = 0;i < strlen(status);i++){
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

	strcpy(pinfo->cmd,  cmd);
	strcpy(pinfo->path, path);
}
