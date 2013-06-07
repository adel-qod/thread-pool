CC = clang
CFLAGS = -gstabs -pedantic -W -Wall
OBJS = main.o testFunctions.o threadPool.o
LFLAGS = -lpthread
threadPoolTest: $(OBJS)
	$(CC) $(CFLAGS) -o threadPoolTest $(OBJS) $(LFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

testFunctions.o: testFunctions.c
	$(CC) $(CFLAGS) -c testFunctions.c

threadPool.o: threadPool.c
	$(CC) $(CFLAGS) -c threadPool.c

clean:
	rm threadPoolTest *.o
