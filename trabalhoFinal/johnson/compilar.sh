#!/bin/bash

printf "compilando codigo sequencial!\n\n"
g++ -o sequencial sequencial.cpp
printf "codigo sequencial compilado! para executar rode:\n\t ./sequencial ../testes/<numero>.in\n"
printf "\n!compilando codigo concorrente!\n\n"
g++ -o conc concorrente.cpp
printf "codigo concorrente compilado! para executar rode:\n\t ./conc <numero_de_threads> ../testes/<numero>.in\n"