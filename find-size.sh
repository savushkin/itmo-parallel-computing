#!/usr/bin/env bash
# USAGE: ./find-size.sh -b PATH_TO_BINARY_FILE -s STEP -t TIME

while getopts b:t:s: option
do
case "${option}" in
    b) BINARY=${OPTARG};;
    t) TIME=${OPTARG};;
    s) STEP=${OPTARG};;
esac
done

COUNT=0;

while(:)
do
    COUNT=$(expr ${COUNT} + ${STEP})
    CURRENT_TIME=`${BINARY} ${COUNT} | awk '{print $3}'`

    if [ ${CURRENT_TIME} -ge ${TIME} ]
    then
    echo ${COUNT}
    break
    fi

done;
