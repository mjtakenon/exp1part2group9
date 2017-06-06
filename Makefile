SimpleHTTPServer.out: SimpleHTTPServer.c exp1_http.c
	gcc -pedantic-errors -o3 -pthread -lcrypto -o SimpleHTTPServer.out SimpleHTTPServer.c exp1_http.c
SimpleHTTPServer.o: SimpleHTTPServer.c
	gcc -c SimpleHTTPServer.c
exp1_http.o: exp1_http.c
	gcc -c exp1_http.c
