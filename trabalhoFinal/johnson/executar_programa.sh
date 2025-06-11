#!/bin/bash

bash compilar.sh

EXECUTAVEL=./concorrente



# CSV header
printf "File, sequencial,Time_2,Time_4,Time_8" > results.csv

# Loop over input files 1.in to 23.in
for (( i=1; i<=23; i++ ))
do
  printf "\n\n"
  file="../testes/$i.in"
  printf "Processing file: $file\n"
  printf "\texecutando o sequencial\n"
  SEQUENCIAL=$(./sequencial $file)
  # Initialize a row with the file name
  row="$file, $SEQUENCIAL"
  printf "\t\to sequencial levou $SEQUENCIAL segundos\n"

  # For each number of threads, measure the execution time
  for threads in 2 4 8
  do
    printf "\tThreads: $threads\n"
    
    # Measure the real execution time (in seconds)

    TEMPO=$($EXECUTAVEL $threads $file)
    printf "\t\tthreads = $threads, tempo = $TEMPO segundos \n"

    # Append the execution time to the row
    row="$row,$TEMPO"
  done

  # Append the row to the CSV file
  printf "$row" >> results.csv
  printf "resultado final:\n\t$row\n"
done
