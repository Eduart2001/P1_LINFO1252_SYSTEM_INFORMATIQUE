all:
	gcc -Iheaders -Wall -Werror -g -o lib/error.o -c src/error.c
	gcc -Iheaders -Wall -Werror -g -o lib/my_mutex.o -c src/my_mutex.c
	gcc -Iheaders -Wall -Werror -g -o lib/my_semaphore.o -c src/my_semaphore.c

	gcc -Iheaders lib/error.o lib/my_mutex.o -Wall -Werror -g -o lib/philosophes src/philosophes.c -lpthread
	gcc -Iheaders lib/error.o lib/my_mutex.o lib/my_semaphore.o -Wall -Werror -g -o lib/prod_cons src/prod_cons.c -lpthread
	gcc -Iheaders lib/error.o lib/my_mutex.o lib/my_semaphore.o -Wall -Werror -g -o lib/writer_reader src/writer_reader.c -lpthread

tests:
	gcc -Iheaders -Wall -Werror -g -o lib/error.o -c src/error.c
	gcc -Iheaders -Wall -Werror -g -o lib/my_mutex.o -c src/my_mutex.c
	gcc -Iheaders lib/error.o lib/my_mutex.o -Wall -Werror -g -o lib/test_my_mutex test/test_my_mutex.c -lpthread


clean:
	rm -f data/*.csv
	rm -f data/*.pdf
	rm -f lib/*.o
	rm -f lib/philosophes
	rm -f lib/prod_cons
	rm -f lib/writer_reader
	rm -f lib/test_my_mutex

script:
	chmod u+x experiments.sh
	./experiments.sh

.PHONY: clean tests