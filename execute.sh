#!/bin/bash

file_name=$(cat "script_execution/info.text")
find . -iname $file_name

if [ $? -ne 0 ]; then
	echo -n "Digite o nome do seu arquivo: "
	read file_name
fi
log_file="script_execution/erro.log"
g++ "$file_name" -o temp 2> $log_file
./temp 2>> $log_file
rm temp
