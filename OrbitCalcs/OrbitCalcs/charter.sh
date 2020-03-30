#!/bin/bash
cd /home/bitnami/data/src
templ=/home/bitnami/data/charts/input/template.sch
jd=`cat jd.txt`
jde=$[$jd+40]
echo Julian date is $jd to $jde

cat ChartData.txt | while read i 
do 
	comid=`echo $i | cut -d, -f1`
	comname=`echo $i | cut -d, -f2`
	echo $comid and $comname
	targ=/home/bitnami/data/charts/input/$comname
	cat $templ | sed s/FNAFNAFNA/$comname/g|sed s/IDIDID/$comid/g|sed s/STASTASTA/$jd/g|sed s/ENDENDEND/$jde/g > $targ.sch
	/home/bitnami/data/charts/do-one.sh $comname
done
cat ChartPlanets.txt | while read i 
do
	comid=`echo $i | cut -d, -f1`
	comname=`echo $i | cut -d, -f2`
	echo $comid and $comname
	targ=/home/bitnami/data/charts/input/$comname
	cat $templ | sed s/FNAFNAFNA/$comname/g|sed s/IDIDID/$comid/g|sed s/STASTASTA/$jd/g|sed s/ENDENDEND/$jde/g > $targ.sch
	/home/bitnami/data/charts/do-one.sh $comname
done
