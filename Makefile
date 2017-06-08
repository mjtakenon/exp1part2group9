SimpleHTTPServer.out: SimpleHTTPServer.c exp1_http.c
<<<<<<< HEAD
	gcc -pedantic-errors -Wall -o3 -g -o SimpleHTTPServer.out SimpleHTTPServer.c exp1_http.c -lcrypto -lssl -lpthread
=======
	gcc -pedantic-errors -Wall -o0 -g -o SimpleHTTPServer.out SimpleHTTPServer.c exp1_http.c -lcrypto -lssl -lpthread
>>>>>>> upstream/master
SimpleHTTPServer.o: SimpleHTTPServer.c
	gcc -c SimpleHTTPServer.c
exp1_http.o: exp1_http.c
	gcc -c exp1_http.c
