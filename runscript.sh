#!/bin/bash
# get todays date
OUTPUT="$(date)"
touch /tmp/testscript.txt
echo $OUTPUT $1 $2 > /tmp/testscript.txt
