#!/bin/bash

make

echo "2,4,8,16,32,64" &> data/pthread_philosophes.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        TEXT=$(/usr/bin/time -f "%e" ./lib/philosophes -N $thread 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/pthread_philosophes.csv
done

# my_philosophes
echo "2,4,8,16,32,64" &> data/my_philosophes.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        TEXT=$(/usr/bin/time -f "%e" ./lib/philosophes -N $thread -m 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/my_philosophes.csv
done

echo "2,4,8,16,32,64" &> data/pthread_prod_cons.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        c=$thread
        p=$thread
        TEXT=$(/usr/bin/time -f "%e" ./lib/prod_cons -P $p -C $c 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/pthread_prod_cons.csv
done

# my_cons_prod
echo "2,4,8,16,32,64" &> data/my_prod_cons.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        c=$thread
        p=$thread
        TEXT=$(/usr/bin/time -f "%e" ./lib/prod_cons -P $p -C $c -m 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/my_prod_cons.csv
done

echo "2,4,8,16,32,64" &> data/pthread_writer_reader.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        r=$thread
        w=$thread
        TEXT=$(/usr/bin/time -f "%e" ./lib/writer_reader -R $r -W $w 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/pthread_writer_reader.csv
done

# my_writer_reader
echo "2,4,8,16,32,64" &> data/my_writer_reader.csv
for i in {1..5}; do
    TIMES=""
    for ((thread = 2; thread <= 64; thread *= 2)); do
        r=$thread
        w=$thread
        TEXT=$(/usr/bin/time -f "%e" ./lib/writer_reader -R $r -W $w -m 2>&1)
        TIME=$(echo "$TEXT" | tail -n 1)
        [ -z "$TIMES" ] && TIMES="$TIME" || TIMES="$TIMES,$TIME"
    done
    echo "$TIMES" >> data/my_writer_reader.csv
done


# echo Philosophes :
# cat data/pthread_philosophes.csv

# echo Producer Consumer :
# cat data/pthrerad_prod_cons.csv

# echo Writer reader :
# cat data/pthread_writer_reader.csv