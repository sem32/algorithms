#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./gamsrv ./in/$file
done

