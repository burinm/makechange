makechange: makechange.o
	gcc $? -o $@

makechange.o: makechange.c
	gcc -c $<

clean:
	rm -f makechange.o makechange
