TESTER_FILES := $(patsubst %.c, obj/%.o, $(wildcard *.c))
EXTRA_TESTER_FILES := $(patsubst %Test.c, ../obj/%.o, $(wildcard *Test.c))

CFLAGS := -g

tester: $(TESTER_FILES) $(EXTRA_TESTER_FILES)
	$(CC) -g -o ../$@ ../src/queue.c ../src/products.c ../src/clients.c $^

obj/%.o: %.c
	@mkdir -p obj
	$(CC) -ansi -pedantic -g -o $@ -c $<

obj/main.o: avlTest.h catalogTest.h 
obj/catalogTest.o: catalogTest.h ../obj/catalog.o
obj/avlTest.o: avlTest.h ../obj/avl.o ../obj/queue.o
obj/revenueTest.o: revenueTest.h ../obj/revenue.o
obj/hashTTest.o: hashTTest.h ../obj/hashT.o

.PHONY: clear
clear:
	-@rm -f ../tester
	-@rm -rf obj
	
