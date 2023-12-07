#!/bin/bash


echo "philosophs_number ,threads_number,try_number i ,time" &> data/pthread_philosophes.csv
for ((thread = 1; thread <= 64; thread *= 2)); do
    make philosophes    
    for i in {1..5}; do
        /usr/bin/time -f "$thread,$thread,$i,%e" ./lib/philosophes -N $thread >> data/pthread_philosophes.csv 2>&1
    done
    make clean -f ./lib/philosophes
done


echo "cons_number ,prod_number ,threads_number,try_number i ,time" &> data/pthrerad_prod_cons.csv

for ((thread = 1; thread <= 64; thread *= 2)); do
    if [ "$thread" != 1 ]; then
      c=$((thread/2))
      p=$((thread/2))
      for i in {1..5}; do
            /usr/bin/time -f "$c,$p,$thread,$i,%e" ./lib/prod_cons -P $p  -C $c >>data/pthrerad_prod_cons.csv 2>&1 
      done
    fi
done

echo "reader_number ,writer_number ,threads_number,try_number i ,time" &> data/pthread_writer_reader.csv

for ((thread = 1; thread <= 64; thread *= 2)); do
    make writer_reader
    if [ "$thread" != 1 ]; then 
      r=$((thread/2))
      w=$((thread/2))
      for i in {1..5}; do
            /usr/bin/time -f "$r,$w,$thread,$i,%e" ./lib/writer_reader -R $r -W $w >>data/pthread_writer_reader.csv 2>&1
      done
    fi
    make clean -f ./lib/writer_reader
done




echo Philosophes :
cat data/pthread_philosophes.csv

echo Producer Consumer :
cat data/pthrerad_prod_cons.csv

echo Writer reader :
cat data/pthread_writer_reader.csv