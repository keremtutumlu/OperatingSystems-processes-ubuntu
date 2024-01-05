CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: findtopk findtopk_mqueue findtopk_thread makeNum

findtopk: findtopk.c
	$(CC) $(CFLAGS) -o findtopk findtopk.c

findtopk_mqueue: findtopk_mqueue.c
	$(CC) $(CFLAGS) -o findtopk_mqueue findtopk_mqueue.c

findtopk_thread: findtopk_thread.c
	$(CC) $(CFLAGS) -o findtopk_thread findtopk_thread.c
	
makeNum: makeNum.c
	$(CC) $(CFLAGS) -o makeNum makeNum.c
