all: change change_alt

change: driver.o makechange.o denominations.o
	gcc $? -Wall -o $@

change_alt: driver.o makechange.o denominations_alt.o
	gcc $? -Wall -o $@

driver.o: driver.c
	gcc -c -Wall $<

makechange.o: makechange.c
	gcc -c -Wall $<

denominations.o: denominations.c
	gcc -c -Wall $<

denominations_alt.o: denominations_alt.c
	gcc -c -Wall $<

clean:
	rm -f change change_alt driver.o makechange.o denominations.o denominations_alt.o
