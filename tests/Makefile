TESTER_FILES := $(patsubst %.c, obj/%.o, $(wildcard *.c))
EXTRA_TESTER_FILES := $(patsubst %Test.c, ../obj/%.o, $(wildcard *Test.c))

CFLAGS := -g

tester: $(TESTER_FILES) $(EXTRA_TESTER_FILES)
	$(CC) -g -o ../$@ ../src/products.c ../src/clients.c $^

obj/%.o: %.c
	@mkdir -p obj
	$(CC) -ansi -pedantic -g -o $@ -c $<

obj/main.o: avlTest.h catalogTest.h salesTest.h 
obj/catalogTest.o: catalogTest.h ../src/catalog.h
obj/avlTest.o: avlTest.h ../src/avl.h 
obj/salesTest.o: salesTest.h ../src/sales.h

.PHONY: clear
clear:
	-@rm -f ../tester
	-@rm -rf obj
	
