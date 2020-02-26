#! /bin/bash
if [ $# -gt 2 ]
then
    echo Can only accept a single argument must be string
fi

if [ $# -eq 0 ]
then
   echo Must pass a single string argument
fi

if [ $# -eq 1 ]
then
    git add .
    git commit -m $1
    git push
fi
