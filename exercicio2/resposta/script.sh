#!/bin/bash

# Configurações
EXECUTAVEL=./primos
NUMEROS=1000000         # Quantidade fixa de números
RODADAS=1000
CSV="resultados.csv"

# Cabeçalho do CSV
echo "threads,execucao,tempo" > $CSV

# Loop para potências de 2: 2^0 até 2^3 (ou seja: 1, 2, 4, 8)
for ((exp=0; exp<=3; exp++)); do
  threads=$((2**exp))
  echo "Executando para $threads threads..."
  
  for ((i=1; i<=RODADAS; i++)); do
    TEMPO=$($EXECUTAVEL $NUMEROS $threads)
    echo "$threads,$i,$TEMPO" >> $CSV
  done
done

echo "Execuções completas. Resultados salvos em $CSV."

