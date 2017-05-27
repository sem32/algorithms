#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./sigkey ./in/$file
done

