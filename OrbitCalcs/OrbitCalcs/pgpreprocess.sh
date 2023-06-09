#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $here
source config

mkdir $here/output >/dev/null 2>&1
./cometpp $here/input/CometEls.txt $here/input/VisCometEls.txt $here/output
./pgsummary $here/input/CometEls.txt $here/output/planetpositions.js $here/output $here/output/ChartData.txt $here/output/comets.js
./conjunctioncalcs 51.88 -1.31 3 2 $here/output
./orbitcalcs 51.88 -1.31 `date +%Y%m%d`  $here/output $here/input
./mooncalcs 51.88 -1.31 2 $here/output > $here/output/mooncalcs.csv

# create the comet pages and charts
./makecometpages.sh
find  output -maxdepth 1 -mtime +10 -name "*.png" -exec rm -f {} \;
find  output -maxdepth 1 -mtime +10 -name "*.js" -exec rm -f {} \;
find  output -maxdepth 1 -mtime +10 -name "*.csv" -exec rm -f {} \;

pushd ~/data
find  . -maxdepth 1 -mtime +10 -name "*.png" -exec mv {} ./old \;
find  . -maxdepth 1 -mtime +10 -name "*.js" -exec mv {} ./old \;
find  . -maxdepth 1 -mtime +10 -name "*.csv" -exec mv {} ./old \;
popd

# sync everything to the webserver
rsync -avz  --exclude MPC* ./output/* ~/$DESTDIR
