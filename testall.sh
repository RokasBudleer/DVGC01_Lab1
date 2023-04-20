#!/bin/sh

for i in mytests/*.pas; do
	echo "testing $i"
	./parser <  $i
done
