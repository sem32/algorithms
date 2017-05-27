#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./hamstr ./in/$file
done

