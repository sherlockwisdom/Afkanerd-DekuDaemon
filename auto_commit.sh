#!/bin/bash


COMMIT_MESSAGE=$1
if make
then
	git add .
	git commit -m "$COMMIT_MESSAGE"
	git push origin master
else
	echo "Failed to build... think about branching"
fi
