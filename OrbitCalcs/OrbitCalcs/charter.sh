#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $here
source ./config

templ=./template.sch

jd=`cat ./jd.txt`
jde=$(( $jd + 40 ))
echo Julian date is $jd to $jde

export SHAREDDIR 

cat output/ChartData.txt | while read i 
do 
	comid=$(echo $i | cut -d, -f1 | awk -F " " '{print $1}')
	comname=$(echo $i | cut -d, -f2)
	echo $comid $comname
	cat $templ | sed "s/FNAFNAFNA/$comname/g"|sed "s/IDIDID/$comid/g"|sed "s/STASTASTA/$jd/g"|sed "s/ENDENDEND/$jde/g" > /tmp/$comname.sch
	$HOME/tools/star-charter/bin/starchart.bin /tmp/$comname.sch 
	rm -f /tmp/$comname.sch
done
cat output/ChartPlanets.txt | while read i
do
	comid=$(echo $i | cut -d, -f1)
	comname=$(echo $i | cut -d, -f2)
	# pluto moves so slowly we need to chart over a longer period than 40 days
	[ "$comid" == "p9" ] && jde=$(( $jd + 1000 )) || jde=$(( jd + 40 ))
	echo $comid $comname
	cat $templ | sed "s/FNAFNAFNA/$comname/g"|sed "s/IDIDID/$comid/g"|sed "s/STASTASTA/$jd/g"|sed "s/ENDENDEND/$jde/g" > /tmp/$comname.sch
	$HOME/tools/star-charter/bin/starchart.bin /tmp/$comname.sch 
	rm -f /tmp/$comname.sch
done
