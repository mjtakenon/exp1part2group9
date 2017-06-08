SimpleHTTPServer.out: SimpleHTTPServer.c exp1_http.c
	gcc -pedantic-errors -Wall -o3 -g -o SimpleHTTPServer.out SimpleHTTPServer.c exp1_http.c -lcrypto -lssl -lpthread
SimpleHTTPServer.o: SimpleHTTPServer.c
	gcc -c SimpleHTTPServer.c
exp1_http.o: exp1_http.c
	gcc -c exp1_http.c
