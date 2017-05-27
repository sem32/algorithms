#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./wchain ./in/$file
done

