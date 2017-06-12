CC      = gcc
CFLAGS  = -pg -c -pedantic-errors -O3 -Wall
LD      = gcc
LDFLAGS = -pg -o $(PROGRAM)
PROGRAM = exp1_https
CSRCS   = exp1_https.c exp1_tcp_listen.c exp1_https_session.c exp1_parse_header.c exp1_parse_status.c exp1_check_file.c exp1_https_reply.c exp1_send_404.c exp1_send_file.c myssl_tls.c
OBJS    = $(CSRCS:.c=.o)
LIBS    = -lssl -lcrypto

.c.o:
	$(CC) $(CFLAGS) $<

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS)
	rm $(OBJS)

clean: ;
	rm $(PROGRAM)

clean objs:
	rm $(OBJS)
