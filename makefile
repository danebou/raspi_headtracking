src = $(wildcard *.c)
obj = $(src:.c=.o)


CC=gcc
CFLAGS =
LDFLAGS = 
LDLIBS = -lhidapi-hidraw

hidtest: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f $(obj) myprog