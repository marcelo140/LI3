CFLAGS += -O2 -ansi

obj = main.o datacheck.o

gereVendas: $(obj)
	$(CC) $(obj) -o gereVendas -lm

debug: CFLAGS := -g -ansi
debug: gereVendas

main.o: datacheck.h
datacheck.o: datacheck.h

.PHONY: clean
clean:
	-rm gereVendas *.o
