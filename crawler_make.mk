OBJS	= mycrawler.o networking.o
SOURCE	= mycrawler.c networking.c
HEADER	= networking.h
OUT	= mycrawler
CC	= gcc
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mycrawler.o: mycrawler.c
	$(CC) $(FLAGS) mycrawler.c
	
networking.o: networking.c
	$(CC) $(FLAGS) networking.c

clean:
	rm -f $(OBJS)