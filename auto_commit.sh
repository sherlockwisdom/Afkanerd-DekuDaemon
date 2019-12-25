#!/bin/bash


COMMIT_MESSAGE=$1

git add .
git commit -m "$COMMIT_MESSAGE"

if make
then
	git push origin master
	echo "Pushed successfully"
else
	echo "Not pushing this code men...."
fi
