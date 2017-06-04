#!/bin/bash

THREADS="2 4 8 16 32"
PROBLEMS=$(find testbed -name "*" -type f)
SERIAL="./knapsackDYN_serial"
PARALLEL="./knapsackDYN_hybrid"
THREADS_FILE="threads.csv"

if [ ! -d "output" ]; then
    mkdir output
fi
if [ ! -d "errors" ]; then
    mkdir errors
fi

echo $THREADS > $THREADS_FILE

for PROBLEM in $PROBLEMS; do
    THREAD=1
    PROBLEM_NAME=$(head -n 1 $PROBLEM | awk '{print $1 "_" $2}')
    qsub -N "P.$PROBLEM_NAME.$THREAD" -pe mpich $THREAD run-mpich-smp.sh $SERIAL $PROBLEM
    for THREAD in $THREADS; do
        qsub -N "P.$PROBLEM_NAME.$THREAD" -pe mpich $THREAD run-mpich-smp.sh $PARALLEL $PROBLEM
    done
done
