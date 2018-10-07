
goal: testmemdump mem test_array

testmemdump: mymemdump.c testmemdump.c obj/orgmemdump.o
	gcc -std=gnu99 -g -o testmemdump testmemdump.c mymemdump.c obj/orgmemdump.o

mem: mymemdump.c mem.c
	gcc -std=gnu99 -g -o mem mymemdump.c mem.c

test_array: test_array.c array.c
	gcc -std=gnu99 -g -o test_array test_array.c array.c
clean:
	rm -f testmemdump  mem test_array total.out

