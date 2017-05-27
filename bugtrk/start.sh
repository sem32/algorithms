#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./bugtrk ./in/$file
done

