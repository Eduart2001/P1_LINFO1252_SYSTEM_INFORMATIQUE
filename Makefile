
all:
	make philosophes
	make prod_cons
	make writer_reader

philosophes:
	gcc src/philosophes.c -o lib/philosophes -lpthread

prod_cons:
	gcc src/prod_cons.c -o lib/prod_cons -lpthread

writer_reader:
	gcc src/writer_reader.c -o lib/writer_reader -lpthread

# gcc -Iheaders -Wall -Werror -g -o error.o -c error.c
# gcc -Iheaders error.o -Wall -Werror -g -o philosophes philosophes.c -lpthread
# gcc -Iheaders error.o -Wall -Werror -g -o prod_cons prod_cons.c -lpthread
# gcc -Iheaders error.o -Wall -Werror -g -o writer_reader writer_reader.c -lpthread


clean:
	rm -f data/*.csv
	rm -f *.o
	rm -f lib/philosophes
	rm -f lib/prod_cons
	rm -f lib/writer_reader


script:
	chmod u+x script.sh
	./script.sh
