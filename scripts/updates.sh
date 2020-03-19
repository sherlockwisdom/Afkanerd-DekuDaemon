#!/bin/bash


# Please do not update code while it's running in production to avoid breaking git clones
# Update should be persistent once triggered, just in case of internet connection issues
# Maybe suppress output from this programs or run them in secured environments

# pull latest version from the main repo
git pull origin master

# build all the keys
make -C ../../ -B start   
