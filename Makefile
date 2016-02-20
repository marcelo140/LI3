obj = main.o datacheck.o

gereVendas: $(obj)
	$(CC) $(obj) -o gereVendas

main.o: datacheck.h
datacheck.o: datacheck.h

.PHONY: clean
clean:
	-rm gereVendas *.o
