CC=gcc
CFALGS=-Wall -Werror -gcc
LIBS=-lpthread
INCLUDE_HEADERS_DIRECTORY=-Iheaders

all:
	make philosophes
	make prod_cons
	make writer_reader

philosophes: src/philosophes.c lib/my_semaphore.o lib/my_mutex.o lib/error.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) src/philosophes.c -o lib/philosophes $(LIBS)
%.o: %.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

prod_cons: src/prod_cons.c lib/my_semaphore.o lib/my_mutex.o lib/error.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) src/prod_cons.c -o lib/prod_cons $(LIBS)
%.o: %.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

writer_reader: src/writer_reader.c lib/my_semaphore.o lib/my_mutex.o lib/error.o
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) src/writer_reader.c -o lib/writer_reader $(LIBS)
%.o: %.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o $@ -c $<

# gcc -Iheaders -Wall -Werror -g -o error.o -c error.c
# gcc -Iheaders error.o -Wall -Werror -g -o my_mutex.o -c my_mutex.c
# gcc -Iheaders error.o -Wall -Werror -g -o philosophes philosophes.c -lpthread
# gcc -Iheaders error.o -Wall -Werror -g -o prod_cons prod_cons.c -lpthread
# gcc -Iheaders error.o -Wall -Werror -g -o writer_reader writer_reader.c -lpthread

tests:
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) $(CFLAGS) -o lib/error.o -c src/error.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) lib/error.o $(CFLAGS) -o lib/my_mutex.o -c src/my_mutex.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) lib/error.o lib/my_mutex.o $(CFLAGS) -o lib/my_semaphore.o -c src/my_semaphore.c
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) lib/error.o lib/my_mutex.o $(CFLAGS) -o lib/test_my_mutex test/test_my_mutex.c $(LIBS)
	$(CC) $(INCLUDE_HEADERS_DIRECTORY) lib/error.o lib/my_mutex.o lib/my_semaphore.o $(CFLAGS) -o lib/test_my_semaphore test/test_my_semaphore.c $(LIBS)


clean:
	rm -f data/*.csv
	rm -f *.o
	rm -f lib/philosophes
	rm -f lib/prod_cons
	rm -f lib/writer_reader


script:
	chmod u+x experiments.sh
	./experiments.sh

# a .PHONY target forces make to execute the command even if the target already exists
.PHONY: clean tests