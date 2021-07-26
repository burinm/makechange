change: driver.o makechange.o denominations.o
	gcc $? -Wall -o $@

driver.o: driver.c denominations.c
	gcc -c -Wall $?

makechange.o: makechange.c
	gcc -c -Wall $<

clean:
	rm -f change driver.o makechange.o
