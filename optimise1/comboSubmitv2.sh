#!/bin/bash

 # Set njets.


masswindow=0
for (($masswindow; masswindow<=1; masswindow++))
do
	alternative=0
	for (($alternative; alternative<=1; alternative++))
	do
		diboson=0
		for (($diboson; diboson<=1; diboson++))
		do
			truthall=0
			for (($truthall; truthall<=1; truthall++))
			do
				sherpa=0
				for (($sherpa; sherpa<=1; sherpa++))
				do
					njet=2
					for (($njet; njet<=3; njet++))
					do
						kfold=1 # Reset kFold.
						for (($kfold; kfold<=2; kfold++))
						do
							echo "Submitting training for $sherpa sherpa, $njet jets, $kfold iteration, $truthall truthall, $diboson MVA diboson, $alternative alternative MC, $masswindow mass window cut."
							tmvaTrain $njet $kfold $alternative $sherpa $truthall $diboson $masswindow
						done
					done
				done
			done
		done
	done
done

exit 0