#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $here
source ./config

echo "#!/bin/bash" > /tmp/createpages.sh
echo "cd /tmp" >> /tmp/createpages.sh
echo "chmod +x ./create_page.php" >> /tmp/createpages.sh

cat input/VisCometEls.txt  | cut -c103-140 | sort | uniq| while read i 
do 
  k=`echo $i | sed 's/\//-/g' | sed 's/ /-/g' | sed 's/(//g' | sed 's/)//g'`
  if [ ! -z $k ]
  then 
    echo $k
    echo "./create_page.php $k" >> /tmp/createpages.sh
  fi
done
chmod +x /tmp/createpages.sh

scp -i $WEBKEY /tmp/createpages.sh $WEBUSER@$WEBHOST:/tmp
scp -i $WEBKEY ./create_page.php $WEBUSER@$WEBHOST:/tmp
ssh -i $WEBKEY $WEBUSER@$WEBHOST /tmp/createpages.sh

pushd ~/tools/ephemeris-compute-de430/
mv -f data/*.cmt /tmp
./setup.sh
popd
./charter.sh
