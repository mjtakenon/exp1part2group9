CC      = gcc
CFLAGS  = -pg -c -pedantic-errors -O0 -Wall -g -fno-stack-protector -z execstack
LD      = gcc
LDFLAGS = -pg -fno-stack-protector -z execstack -O0 -g -o $(PROGRAM)
PROGRAM = exp1_http
CSRCS   = exp1_http.c exp1_tcp_listen.c exp1_http_session.c exp1_parse_header.c exp1_parse_status.c exp1_check_file.c exp1_http_reply.c exp1_send_404.c exp1_send_file.c 
OBJS    = $(CSRCS:.c=.o)
LIBS    =

.c.o:
	$(CC) $(CFLAGS) $<

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS)
	rm $(OBJS)

clean: ;
	rm $(PROGRAM)

clean objs:
	rm $(OBJS)
