#!/usr/bin/sh


if [ "$1" == "rm" ]
then
    rm $(ls -l data | awk '$5 == 302{print $9}')
else

    page="$1"

    while [ "$page" != "$2" ]
    do
        test -e "data/20161028-page$page.json"
        if [ $? -ne 0 ]
        then
            echo "$page"
            curl -H "Authorization: token a04fdfceb19325e627514bbcb551ef1f78fdbb18" "https://meichu.tagtoo.com.tw/pageviews/?page=$page&date=20161028" > "data/20161028-page$page.json"
        fi
        page="$(($page+100))"
    done
fi
