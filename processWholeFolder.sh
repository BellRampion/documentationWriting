#!/bin/bash

#NOTE: This script fails if there aren't any files of the correct type in the folder it is run against. Otherwise, works great
for filename in /*.xojo_code; do
    ./findMethodsXojoBash "$filename" "test.txt"
done
