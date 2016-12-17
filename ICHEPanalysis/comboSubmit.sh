#!/bin/bash

njet=2 # Set njets.

for (($njet; njet<=3; njet++))
do
kfold=0 # Reset kFold.
	for (($kfold; kfold<=1; kfold++))
	do
		echo "Submitting training for $njet jets and iteration $kfold"
		tmvaTrain $njet $kfold 0 1 1
		mv weights "weights"$njet"jet"$kfold"iter"
	done
done

exit 0