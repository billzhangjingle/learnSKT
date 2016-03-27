.PHONY:clean all
CC=gcc
CFLAGS=-Wall -g
BIN=dm01_threadbase  dm01_threadbase-2 dm02_thead dm03_thread dm04_multhread dm05_threadattr dm06_mutex
all:$(BIN)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ 
	
dm01_threadbase:dm01_threadbase.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

dm01_threadbase-2:dm01_threadbase-2.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
dm02_thead:dm02_thead.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
dm03_thread:dm03_thread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
dm04_multhread:dm04_multhread.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
dm05_threadattr:dm05_threadattr.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
dm06_mutex:dm06_mutex.o
	$(CC) $(CFLAGS) $^ -o $@ -lpthread
clean:
	rm -f *.o $(BIN)
