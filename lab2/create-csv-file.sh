#!/usr/bin/env bash
BIN='./lab2-par'
N1=1000
echo "N1 = ${N1}"
N2=5700
echo "N2 = ${N2}"
DELTA=$(expr $(expr ${N2} - ${N1}) / 10)
echo "DELTA = ${DELTA}"

I=${N1}
:>./par-1.csv
while([ ${I} -le ${N2} ])
do
    echo "${BIN} ${I} 1"
    RESULT=`${BIN} ${I} 1`
    echo "${RESULT}"
    TIME=`echo ${RESULT} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-1.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-2.csv
while([ ${I} -le ${N2} ])
do
    echo "${BIN} ${I} 2"
    RESULT=`${BIN} ${I} 2`
    echo "${RESULT}"
    TIME=`echo ${RESULT} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-2.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-4.csv
while([ ${I} -le ${N2} ])
do
    echo "${BIN} ${I} 4"
    RESULT=`${BIN} ${I} 4`
    echo "${RESULT}"
    TIME=`echo ${RESULT} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-4.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-5.csv
while([ ${I} -le ${N2} ])
do
    echo "${BIN} ${I} 5"
    RESULT=`${BIN} ${I} 5`
    echo "${RESULT}"
    TIME=`echo ${RESULT} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-5.csv
    I=$(expr ${I} + ${DELTA})
done;

