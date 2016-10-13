#!/bin/bash

KCURRENT=1

cd /unix/atlasvhbb/abell/tmvaTraining/gridsearch
# Run tmvaTrain executable with four arguments and output to text file.
# Working cc file -> tmvaTrainLocalTune.cc

while (test $KCURRENT -le $KFOLD); do
	/unix/atlasvhbb/abell/tmvaTraining/train/tmvaTrain 0 2 $KCURRENT $NTREES $NCUTS\ > /unix/atlasvhbb/abell/tmvaTraining/gridsearch/text/EGStraintrees${NTREES}cuts${NCUTS}k${KCURRENT}.txt

 	let KCURRENT=$KCURRENT+1

done
