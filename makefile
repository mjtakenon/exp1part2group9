server: exp1_tcp_talk.c httpserver.c
	gcc -pedantic-errors -O3 -o server exp1_tcp_talk.c httpserver.c
client: exp1_tcp_talk.c clientbenti.c
	gcc -pedantic-errors -O3 -o client exp1_tcp_talk.c clientbenti.c
