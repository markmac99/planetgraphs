#!/bin/bash
cd /opt/bitnami/apps/wordpress/htdocs/data/src
git pull
make
./cometpp ./CometEls.txt ./VisCometEls.txt /opt/bitnami/apps/wordpress/htdocs/data/ 
./pgsummary ./CometEls.txt /home/bitnami/data/planetpositions.js /opt/bitnami/apps/wordpress/htdocs/data/ ./ChartData.txt
cd ..
src/conjunctioncalcs 51.88 -1.31 3 2
cd src
./orbitcalcs 51.88 -1.31 `date +%Y%m%d`  /home/bitnami/data/
./mooncalcs 5.188 -1.31 2
mv mooncalcs.js /home/bitnami/data/
./makecometpages.sh
