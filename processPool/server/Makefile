server:main.o worker.o initTcp.o epoll.o sendfd.o
	gcc main.o worker.o initTcp.o epoll.o sendfd.o -o server -lpthread
main.o:main.c
	gcc -c main.c -o main.o -g -Wall
worker.o:worker.c
	gcc -c worker.c -o worker.o -g -Wall
initTcp.o:initTcp.c
	gcc -c initTcp.c -o initTcp.o -g -Wall
epoll.o:epoll.c
	gcc -c epoll.c -o epoll.o -g -Wall
sendfd.o:sendfd.c
	gcc -c sendfd.c -o sendfd.o -g -Wall
