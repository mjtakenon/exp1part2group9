#include <stdio.h>
#include <stdlib.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "myssl_tls.h"

void openssl_exit_message(void){
	ERR_print_errors_fp(stderr);
	exit(-1);
}

void set_crt_and_key(SSL_CTX *ctx, const char *crtFile, const char *keyFile){
	if(SSL_CTX_use_certificate_file(ctx, crtFile, SSL_FILETYPE_PEM) <= 0){
		openssl_exit_message();
	}
	if(SSL_CTX_use_PrivateKey_file(ctx, keyFile, SSL_FILETYPE_PEM) <= 0){
		openssl_exit_message();
	}
	if(!SSL_CTX_check_private_key(ctx)){
		fprintf(stderr, "Privatekey or Certificate is strange.\n");
		exit(-1);
	}
}

void show_crts(SSL *ssl){
	X509 *x509Crt;

	x509Crt = SSL_get_peer_certificate(ssl);
	if(x509Crt != NULL){
		char *oneLine;
		oneLine = X509_NAME_oneline(X509_get_subject_name(x509Crt), 0, 0);
		if(oneLine == NULL){
			openssl_exit_message();
		}
		printf("%s\n", oneLine);
		oneLine = X509_NAME_oneline(X509_get_issuer_name(x509Crt), 0, 0);
		if(oneLine == NULL){
			openssl_exit_message();
		}
		printf("%s\n", oneLine);
		free(oneLine);
		X509_free(x509Crt);
	}
}

SSL_CTX *init_and_set_CTX(enum SSLTLSTYPE type){
	SSL_CTX *ctx;
	const SSL_METHOD *method;

	SSL_library_init();
	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	
	if(type == SSL_TLS_SERVER){
		method = SSLv23_server_method();
	}else{
		method = SSLv23_client_method();
	}
	ctx = SSL_CTX_new(method);
	if(ctx == NULL){
		ERR_print_errors_fp(stderr);
		exit(-1);
	}

	return ctx;
}
