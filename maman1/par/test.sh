#!/bin/bash


for ((i = 0; i < 13; i++))
do
	echo "This is test number $i"
	echo -e "\n"
	./"par" < "test$i"
	echo -e "\n"
done
