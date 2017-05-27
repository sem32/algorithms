#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./govern ./in/$file
done

