#!/bin/bash


COMMIT_MESSAGE=$1

git add .
git commit -m "$COMMIT_MESSAGE"

if make
	git push origin master
	echo "Pushed successfully!"
fi
