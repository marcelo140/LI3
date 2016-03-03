CFLAGS += -O2 -ansi -Wall

obj = main.o datacheck.o mockArray.o

gereVendas: $(obj)
	$(CC) $(obj) -o gereVendas -lm

debug: CFLAGS := -g -ansi
debug: gereVendas

main.o: datacheck.h mockArray.h
datacheck.o: datacheck.h
mockArray.o: mockArray.h

.PHONY: clean
clean:
	-rm gereVendas *.o
