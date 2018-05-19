#!/usr/bin/env bash
BIN='./lab1'
#N1=$(./find-size.sh -b ./bin/lab1-seq -s 100 -t 10)
N1=900
echo "N1 = ${N1}"
#N2=$(./find-size.sh -b ./bin/lab1-seq -s 100 -t 2000)
N2=5400
echo "N2 = ${N2}"
DELTA=$(expr $(expr ${N2} - ${N1}) / 10)
echo "DELTA = ${DELTA}"

# from, to, delta, bin, csv, thread
function write_csv() {
    I=$1
    echo "N,TIME,X">./csv/$5
    while([ ${I} -le $2 ])
    do
        echo "./bin/$4 ${I} ${6}"
        RESULT=`./bin/$4 ${I} ${6}`
        echo "${RESULT}"
        TIME=`echo ${RESULT} | awk '{print $3}'`
        X=`echo ${RESULT} | awk '{print $10}'`
        echo "$I,$TIME,$X" >> ./csv/$5
        I=$(expr ${I} + $3)
    done;
}

#write_csv ${N1} ${N2} ${DELTA} lab1-seq seq.csv
#write_csv ${N1} ${N2} ${DELTA} lab1-par-2 lab1-par-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab1-par-4 lab1-par-4.csv
#write_csv ${N1} ${N2} ${DELTA} lab1-par-5 lab1-par-5.csv
#write_csv ${N1} ${N2} ${DELTA} lab1-par-8 lab1-par-8.csv
#
#write_csv ${N1} ${N2} ${DELTA} lab2-par lab2-par-2.csv 1
#write_csv ${N1} ${N2} ${DELTA} lab2-par lab2-par-2.csv 2
#write_csv ${N1} ${N2} ${DELTA} lab2-par lab2-par-4.csv 4
#write_csv ${N1} ${N2} ${DELTA} lab2-par lab2-par-5.csv 5
#
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-1-1 lab3-par-static-1-1.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-1-2 lab3-par-static-1-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-1-4 lab3-par-static-1-4.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-100-1 lab3-par-static-100-1.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-100-2 lab3-par-static-100-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-static-100-4 lab3-par-static-100-4.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-1-1 lab3-par-dynamic-1-1.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-1-2 lab3-par-dynamic-1-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-1-4 lab3-par-dynamic-1-4.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-100-1 lab3-par-dynamic-100-1.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-100-2 lab3-par-dynamic-100-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-dynamic-100-4 lab3-par-dynamic-100-4.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-guided-1-1 lab3-par-guided-1-1.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-guided-1-2 lab3-par-guided-1-2.csv
#write_csv ${N1} ${N2} ${DELTA} lab3-par-guided-1-4 lab3-par-guided-1-4.csv

write_csv ${N1} ${N2} ${DELTA} lab4-seq lab4-seq.csv
write_csv ${N1} ${N2} ${DELTA} lab4-par-guided lab4-par-guided-2.csv 2
write_csv ${N1} ${N2} ${DELTA} lab4-par-guided lab4-par-guided-4.csv 4
#write_csv ${N1} ${N2} ${DELTA} lab4-par-static lab4-par-static-2.csv 2
#write_csv ${N1} ${N2} ${DELTA} lab4-par-static lab4-par-static-4.csv 4
#write_csv ${N1} ${N2} ${DELTA} lab4-par-dynamic lab4-par-dynamic-2.csv 2
#write_csv ${N1} ${N2} ${DELTA} lab4-par-dynamic lab4-par-dynamic-4.csv 4
