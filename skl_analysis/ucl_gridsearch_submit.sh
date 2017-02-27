#!/bin/bash

# Activate venv.
source /home/jpyne/gridsearch-venv/bin/activate
cd /home/jpyne/msci-hep/skl_analysis

# Iterate through gridsearch args.
kfolds = 2
for (($kfolds; kfolds<=3; kfolds++))
do
	njets = 2
	for (($njets; njets<=3; njets++))
	do
		python crossValidatorBDT.py $njets $kfolds
	done
done

exit 0