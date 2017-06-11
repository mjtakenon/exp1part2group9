CC      = gcc
CFLAGS  = -pg -c -O3 -Wall
LD      = gcc
LDFLAGS = -pg -o $(PROGRAM)
PROGRAM = exp1_http_overrun
CSRCS   = exp1_http_overrun.c exp1_tcp_connect.c exp1_do_talk.c
OBJS    = $(CSRCS:.c=.o)
LIBS    = 

.c.o:
	$(CC) $(CFLAGS) $<

$(PROGRAM): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS)
	rm $(OBJS)

clean_program:
	rm $(PROGRAM)

clean_objs:
	rm $(OBJS)
