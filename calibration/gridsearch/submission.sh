#!/bin/bash

# Test environment variables.
# export NTREES="100"
# export NCUTS="100"

# Run tmvaTrain executable with four arguments and output to text file.
# Working cc file -> tmvaTrainLocalTune.cc
../train/tmvaTrain 0 2 $NTREES $NCUTS > text/EGStraintrees${NTREES}cuts${NCUTS}.txt