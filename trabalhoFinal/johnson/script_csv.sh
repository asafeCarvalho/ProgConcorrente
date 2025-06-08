#!/bin/bash

EXECUTAVEL=./conc



# CSV header
echo "File, sequencial,Time_2,Time_4,Time_8" > results.csv

# Loop over input files 1.in to 23.in
for (( i=1; i<=23; i++ ))
do
  file="../testes/$i.in"
  echo "Processing file: $file"

  # Initialize a row with the file name
  row="$file, $(./sequencial $file)"

  # For each number of threads, measure the execution time
  for threads in 2 4 8
  do
    echo "  Threads: $threads"
    
    # Measure the real execution time (in seconds)

    TEMPO=$($EXECUTAVEL $threads $file)

    # Append the execution time to the row
    row="$row,$TEMPO"
  done

  # Append the row to the CSV file
  echo "$row" >> results.csv
  echo "$row"
done
