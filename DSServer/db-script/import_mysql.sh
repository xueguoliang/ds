#!/bin/sh
username=`cat ~/dsconfig | jq '.username' | awk -F\" '{print $2}'`
password=`cat ~/dsconfig | jq '.password' | awk -F\" '{print $2}'`

scripts=`find -name "*.sql"`
for script in $scripts
do
    mysql -u$username -p$password < $script
    echo $script done
done

echo init database done
