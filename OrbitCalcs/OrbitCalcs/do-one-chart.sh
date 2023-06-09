#!/bin/bash
pushd /home/bitnami/data/charts
$HOME/tools/star-charter/bin/starchart.bin input/$1.sch
cp output/$1*.png /home/bitnami/data
popd
