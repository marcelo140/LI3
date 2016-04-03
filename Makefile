CFLAGS += -O2 -ansi -Wall
VPATH = src
obj = main.o datacheck.o node.o catalog.o clients.o products.o

gereVendas: $(obj)
	$(CC) $(obj) -o gereVendas 

debug: CFLAGS := -g -ansi
debug: gereVendas

main.o: datacheck.h
datacheck.o: datacheck.h
catalog.o: catalog.h
node.o: node.h
clients.o: clients.h
products.o: products.h

.PHONY: clean
clean:
	-rm gereVendas *.o
	-rm Vendas_1MValidas.txt

doc:
	doxygen doxygen.conf
