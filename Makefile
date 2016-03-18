CFLAGS += -O2 -ansi -Wall

obj = main.o datacheck.o avl.o 

gereVendas: $(obj)
	$(CC) $(obj) -o gereVendas -lm

debug: CFLAGS := -g -ansi
debug: gereVendas

main.o: datacheck.h avl.h
datacheck.o: datacheck.h
avl.o: avl.h

.PHONY: clean
clean:
	-rm gereVendas *.o
	-rm Vendas_1MValidas.txt
