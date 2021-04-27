.POSIX:

EXE=posix-getaddrinfo-example
OBJS=posix-getaddrinfo-example.o

.PHONY: all
all: $(EXE)

.PHONY: clean
clean:
	rm -f $(OBJS)

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) -o $(EXE) $(OBJS)

posix-getaddrinfo-example.o: posix-getaddrinfo-example.c
	$(CC) $(CFLAGS) -c -o $@ $<