#!/bin/bash

 # Set njets.


njet=2
for (($njet; njet<=3; njet++))
do
	kfold=1 # Reset kFold.
	for (($kfold; kfold<=2; kfold++))
	do
		echo "Submitting training for $njet jets, $kfold iteration."
		./tmvaTrain $njet $kfold 0 0
	done
done

exit 0