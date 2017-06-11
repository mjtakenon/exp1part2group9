#include "exp1.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "exp1_https.h"

void exp1_check_file(exp1_info_type *info){
	struct stat s;
	int ret;
	char *pext;

	sprintf(info->real_path, "html%s", info->path);
	ret = stat(info->real_path, &s);

	if((s.st_mode & S_IFMT) == S_IFDIR){
		sprintf(info->real_path, "%s/index.html", info->real_path);
	}

	ret = stat(info->real_path, &s);

	if(ret == -1){
		info->code = 404;
	}else{
		info->code = 200;
		info->size = (int) s.st_size;
	}

	pext = strchr(info->path, '.');
	if(pext != NULL && strcmp(pext, ".html") == 0){
		strcpy(info->type, "text/html");
	}else if(pext != NULL && strcmp(pext, ".jpg") == 0){
		strcpy(info->type, "image/jpeg");
	}
}
