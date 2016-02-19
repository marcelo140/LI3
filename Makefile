obj = main.o datacheck.o

gereVendas: $(obj) 
	$(CC) main.o $(obj) 

main.o: datacheck.h
datacheck.o: datacheck.h

.PHONY: clean
clean:
	-rm gereVendas *.o
