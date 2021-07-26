makechange: makechange.o
	gcc $? -Wall -o $@

makechange.o: makechange.c
	gcc -c -Wall $<

clean:
	rm -f makechange.o makechange
