change: driver.o makechange.o
	gcc $? -Wall -o $@

driver.o: driver.c
	gcc -c -Wall $<

makechange.o: makechange.c
	gcc -c -Wall $<

clean:
	rm -f change driver.o makechange.o
