TARGE:= s
OBJS:=main.o s.o u.o
CC:=gcc
CFLAGS:= -c -o

$(TARGE):$(OBJS)
	$(CC) $(OBJS) -o $(TARGE) -lpthread -lsqlite3
%.o:%.c
	$(CC) $< $(CFLAGS) $@
clean:
	rm $(OBJS) 