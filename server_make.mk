OBJS	= myhttpd.o networking.o
SOURCE	= myhttpd.c networking.c
HEADER	= networking.h.h
OUT	= myhttpd
CC	= gcc
FLAGS	= -c -g

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT)

myhttpd.o: myhttpd.c
	$(CC) $(FLAGS) myhttpd.c
	
networking.o: networking.c
	$(CC) $(FLAGS) networking.c

clean:
	rm -f $(OBJS)