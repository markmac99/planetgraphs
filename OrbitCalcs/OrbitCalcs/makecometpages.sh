#!/bin/bash
cd /home/bitnami/data/src
cat VisCometEls.txt  | cut -c103-140 | sort | uniq| while read i 
do 
  k=`echo $i | sed 's/\//-/g' | sed 's/ /-/g' | sed 's/(//g' | sed 's/)//g'`
  if [ ! -z $k ]
  then 
    echo $k
    ./create_page.php $k 2>/dev/null
  fi
done
./charter.sh
