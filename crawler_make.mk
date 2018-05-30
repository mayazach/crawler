OBJS	= mycrawler.o 
SOURCE	= mycrawler.c
HEADER	= 
OUT	= mycrawler
CC	= gcc
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

mycrawler.o: mycrawler.c
	$(CC) $(FLAGS) mycrawler.c

clean:
	rm -f $(OBJS)