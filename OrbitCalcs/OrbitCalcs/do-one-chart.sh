#!/bin/bash
pushd /home/bitnami/data/charts
/mnt/mjmm-data/star-charter/bin/starchart.bin input/$1.sch
cp output/$1*.png /home/bitnami/data
popd
