#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

pushd ~/tools/ephemeris-compute-de430/
./dataFetch.py
popd

cd $here

mkdir $here/input>/dev/null 2>&1
wget -O $here/input/CometEls.txt http://www.minorplanetcenter.net/iau/MPCORB/CometEls.txt
wget -O /tmp/tmpfile.gz http://www.minorplanetcenter.net/iau/MPCORB/MPCORB.DAT.gz

EXITVAL=$?
if [ $EXITVAL -eq 0 ]
then
  gunzip /tmp/tmpfile.gz
  mv $here/input/MPCORB.DAT $here/input/MPCORB.DAT.prev
  mv /tmp/tmpfile $here/input/MPCORB.DAT
  exit 0
else
  exit $EXITVAL
fi
