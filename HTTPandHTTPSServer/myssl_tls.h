#ifndef __MYSSL_TLS__
#define __MYSSL_TLS__

enum SSLTLSTYPE{
	SSL_TLS_SERVER,
	SSL_TLS_CLIENT
};

extern void openssl_exit_essage(void);

extern void set_crt_and_key(SSL_CTX *ctx, const char *crtFile, const char *keyFile);

extern void show_crts(SSL *ssl);

extern SSL_CTX *init_and_set_CTX(enum SSLTLSTYPE type);

#endif
