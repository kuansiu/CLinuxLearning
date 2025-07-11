server:main.o worker.o initTcp.o epoll.o sendfd.o transFile.o
	gcc main.o worker.o initTcp.o epoll.o sendfd.o transFile.o -o server -lpthread
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
transFile.o:transFile.c
	gcc -c transFile.c -o transFile.o -g -Wall