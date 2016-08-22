#!/bin/sh
read -p "input ip first 1 byte:"  first
read -p "input ip first 2 byte:"  second
read -p "input ip first 3 byte:"  third
read -p "input start 4 byte:"  fourStart
read -p "input stop 4 byte:"  fourStop
while [ $fourStart -le $fourStop ]
do
	echo $first.$second.$third.$fourStart:8888
	curl $first.$second.$third.$fourStart:8888 --connect-timeout 0.5
	echo 
	fourStart=`expr $fourStart + 1`
done