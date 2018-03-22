#!/usr/bin/env bash
BIN='./lab2'
N1=950
echo "N1 = ${N1}"
N2=5000
echo "N2 = ${N2}"
DELTA=$(expr $(expr ${N2} - ${N1}) / 10)
echo "DELTA = ${DELTA}"

I=${N1}
:>./seq.csv
while([ ${I} -le ${N2} ])
do
    TIME=`${BIN}-seq ${I} | awk '{print $3}'`
    echo "$I,$TIME" >> ./seq.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-2.csv
while([ ${I} -le ${N2} ])
do
    TIME=`${BIN}-par-2 ${I} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-2.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-4.csv
while([ ${I} -le ${N2} ])
do
    TIME=`${BIN}-par-4 ${I} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-4.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-5.csv
while([ ${I} -le ${N2} ])
do
    TIME=`${BIN}-par-5 ${I} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-5.csv
    I=$(expr ${I} + ${DELTA})
done;

I=${N1}
:>./par-8.csv
while([ ${I} -le ${N2} ])
do
    TIME=`${BIN}-par-8 ${I} | awk '{print $3}'`
    echo "$I,$TIME" >> ./par-8.csv
    I=$(expr ${I} + ${DELTA})
done;
