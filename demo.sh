#!/usr/bin/sh


#./main $@

i="0"

while [ "$i" != "24" ]
do
    echo "test $i start:"
    ./pon $1 $(cat "output/user$i.out")
    echo "test $i done."
    i=$(($i+1))
    echo ""
done
