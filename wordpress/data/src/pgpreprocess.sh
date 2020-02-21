#!/bin/bash
cd /opt/bitnami/apps/wordpress/htdocs/data/src
./cometpp ./CometEls.txt ./VisCometEls.txt /opt/bitnami/apps/wordpress/htdocs/data/
./pgsummary ./CometEls.txt ../planetpositions.js /opt/bitnami/apps/wordpress/htdocs/data/
cd ..
src/conjunctioncalcs 51.88 -1.31 3 2
