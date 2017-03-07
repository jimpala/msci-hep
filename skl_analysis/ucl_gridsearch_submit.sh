#!/bin/bash

# Activate venv.
source /home/jpyne/gridsearch-venv/bin/activate
cd /home/jpyne/msci-hep/skl_analysis

# Iterate through gridsearch args.
kfolds=2
for (($kfolds; kfolds<=2; kfolds++))
do
	njets=2
	for (($njets; njets<=3; njets++))
	do
		python crossValidatorBDTmain_GPU.py $njets $kfolds
	done
done

exit 0
