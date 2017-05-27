#!/bin/bash

for file in $( ls ./in); do
	 echo $file
	./discnt ./in/$file
done

