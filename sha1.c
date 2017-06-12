#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

char *sha1_to_hex(const unsigned char *sha1)
{
    int bufno = 0;
    char *hexbuffer[4];
    const char hex[] = "0123456789abcdef";
    char *buffer = hexbuffer[3 & ++bufno], *buf = buffer;
    int i;

    for (i = 0; i < 4; i++) {
        do {
            hexbuffer[i] = (char *)malloc(sizeof(char) * 50);
        } while(hexbuffer[i] == NULL);
    }

    for (i = 0; i < 20; i++) {
	unsigned int val = *sha1++;
	*buf++ = hex[val >> 4];
	*buf++ = hex[val & 0xf];
    }
    *buf = '\0';

    return buffer;
}

char *calc_sha1(const char *body)
{
    unsigned char sha1[41];
    SHA_CTX c;
    SHA1_Init(&c);
    SHA1_Update(&c, body, strlen(body));
    SHA1_Final(sha1, &c);

    return sha1_to_hex(sha1);

}

int main()
{
    char body[64];
    char *code;
    scanf("%s",body);
    strtok(body,"\r\n");
    code = calc_sha1(body);
    printf("%s\n", code);
    return 0;
}
