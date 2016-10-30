#!/usr/bin/sh


./main $@

echo "{\"seg\":[" > output/img.json

i="0"

while [ "$i" != "24" ]
do
    if [ "$i" != "" ]
    then
        echo "," >> output/img.json
    fi
    echo "test $i start:"
    ./pon $1 $(cat "output/user$i.out")
    echo "test $i done."
    i=$(($i+1))
    echo ""
done

echo "]}" >> output/img.json
