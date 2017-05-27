#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./lngpok ./in/$file
done

