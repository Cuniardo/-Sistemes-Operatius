#!/bin/bash

EXEC = programa
SRC = main.c

check_test (){
if [ "$SRC" -nt "$EXEC" ] 
then
      echo "Building program"
      make run
fi

RESULT = `./$EXEC`

if ["$RESULT" == "0"];
then
      echo "User found."
else 
      echo "User not found"
}

check_test $?

make clean
cd ..