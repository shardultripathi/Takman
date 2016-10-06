#!/bin/bash

# This is to run the script

# if [ ! -e $1 ]; then
#     echo "Inputfile doesn't exist. Exitting..."
#     exit 1
# fi
if [ -e "dstak" ]; then
    ./dstak $1
else
    echo "First run compile.sh to compile the code"
fi
