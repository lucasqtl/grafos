#!/bin/bash

# Binários
floyd=./floyd.bin
agm=./agm

echo "Testando algoritmo de Floyd-Warshall"
for i in inputs/*.mtx
do
    echo -e "\e[33mInstância $i\e[0m"
    $floyd -f $i > temp

    j=$(basename $i)
    diff -w temp ./outputs/$j > /dev/null
    if [ $? -eq 0 ]; then
        echo -e "\e[32mOK\e[0m"
    else
        echo -e "\e[31mErro\e[0m"
    fi
    rm temp
done