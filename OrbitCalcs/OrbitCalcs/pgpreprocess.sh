#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $here
source config

mkdir $here/output >/dev/null 2>&1
./cometpp $here/input/CometEls.txt $here/input/VisCometEls.txt $here/output
./pgsummary $here/input/CometEls.txt $here/output/planetpositions.js $here/output $here/output/ChartData.txt $here/output/comets.js
cd output
../conjunctioncalcs 51.88 -1.31 3 2 $here/output

cd ..
./orbitcalcs 51.88 -1.31 `date +%Y%m%d`  $here/output $here/input
./mooncalcs 51.88 -1.31 2 $here/output > $here/output/mooncalcs.csv

# create the comet pages and charts
./makecometpages.sh

# sync everything to the webserver
rsync -avz -e "ssh -i $WEBKEY" output/* $WEBUSER@$WEBHOST:$DESTDIR
