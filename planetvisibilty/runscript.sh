#!/bin/bash
echo \<font color=\"Red\"\>Observer Location $1 $2\<br\>\</font\> > location.html
./orbitcalcs $1 $2 $3
