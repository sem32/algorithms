#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./ijones ./in/$file
done

