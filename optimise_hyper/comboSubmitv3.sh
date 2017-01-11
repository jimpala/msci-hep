#!/bin/bash

 # Set njets.


maxdepth=3
for (($maxdepth; maxdepth<=7; maxdepth++))
do
	ntrees=100
	for (($ntrees; ntrees<=400; ntrees+=20))
	do
		njet=2
		for (($njet; njet<=3; njet++))
		do
			kfold=1 # Reset kFold.
			for (($kfold; kfold<=2; kfold++))
			do
				echo "Submitting training for $ntrees trees, $maxdepth max depth. $njet jets, $kfold iteration."
				./tmvaTrain $njet $kfold $ntrees $maxdepth
			done
		done
	done
done

exit 0