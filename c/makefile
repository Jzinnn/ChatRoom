TARGE:= c
OBJS:=main.o c.o u.o conio.o
CC:=gcc
CFLAGS:= -c -o

$(TARGE):$(OBJS)
	$(CC) $(OBJS) -o $(TARGE) -lpthread
%.o:%.c
	$(CC) $< $(CFLAGS) $@
clean:
	rm $(OBJS) 