server:main.o worker.o ThreadPool.o taskQueue.o tidArr.o epoll.o initTcp.o transFile.o
	gcc main.o worker.o ThreadPool.o taskQueue.o tidArr.o epoll.o initTcp.o transFile.o -o server -lpthread
main.o:main.c
	gcc -c main.c -o main.o -g -Wall
worker.o:worker.c
	gcc -c worker.c -o worker.o -g -Wall
ThreadPool.o:ThreadPool.c
	gcc -c ThreadPool.c -o ThreadPool.o -g -Wall
taskQueue.o:taskQueue.c
	gcc -c taskQueue.c -o taskQueue.o -g -Wall
tidArr.o:tidArr.c
	gcc -c tidArr.c -o tidArr.o -g -Wall
epoll.o:epoll.c
	gcc -c epoll.c -o epoll.o -g -Wall
initTcp.o:initTcp.c
	gcc -c initTcp.c -o initTcp.o -g -Wall
transFile.o:transFile.c
	gcc -c transFile.c -o transFile.o -g -Wall
clean:
	rm -f *.o server
rebuilt:clean server