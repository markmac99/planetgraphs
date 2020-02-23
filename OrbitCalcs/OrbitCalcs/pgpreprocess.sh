#!/bin/bash
cd /opt/bitnami/apps/wordpress/htdocs/data/src
make
./cometpp ./CometEls.txt ./VisCometEls.txt /opt/bitnami/apps/wordpress/htdocs/data/
./pgsummary ./CometEls.txt ../planetpositions.js /opt/bitnami/apps/wordpress/htdocs/data/
cd ..
src/conjunctioncalcs 51.88 -1.31 3 2
src/mooncalcs 5.188 -1.31 2
cd src
./orbitcalcs 51.88 -1.31 `date +%Y%m%d`  ..
