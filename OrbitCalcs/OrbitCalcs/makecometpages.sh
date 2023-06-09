#!/bin/bash
here="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $here
source ./config

echo '#!/bin/bash' > /tmp/createpages.sh
echo "cd /tmp" >> /tmp/createpages.sh
echo "chmod +x $here/create_page.php" >> /tmp/createpages.sh

cat input/VisCometEls.txt  | cut -c103-140 | sort | uniq| while read i 
do 
  k=`echo $i | sed 's/\//-/g' | sed 's/ /-/g' | sed 's/(//g' | sed 's/)//g'`
  if [ ! -z $k ]
  then 
    echo $k
    echo "$here/create_page.php $k" >> /tmp/createpages.sh
  fi
done
chmod +x /tmp/createpages.sh
/tmp/createpages.sh

pushd ~/tools/ephemeris-compute-de430/
mv -f data/*.cmt /tmp
./setup.sh
popd
./charter.sh
