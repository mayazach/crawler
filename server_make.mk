OBJS	= myhttpd.o 
SOURCE	= myhttpd.c
HEADER	= 
OUT	= myhttpd
CC	= gcc
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

myhttpd.o: myhttpd.c
	$(CC) $(FLAGS) myhttpd.c

clean:
	rm -f $(OBJS)