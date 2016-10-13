import os
import sys
import subprocess
import numpy as np


def main():
	# Environment setup.
	current_env = os.environ.copy()

	# qsub submission string.
	submission= 'qsub -e /dev/null -o /dev/null -q short -V submission.sh'

	# BDT hyperparameter space.
	ntrees = np.arange(100,120,10)
	ncuts = np.arange(100,120,10)

	# Constants.
	# (blank)
	maxdepth = 4

	# kFold setting.
	# NOTE: This is done remotely at the moment via
	# running the 'kFolder' root macro on each of
	# TTrees with the specified k value.
	k = 10
	current_env['KFOLD'] = str(k)


	print repr(submission)
	print 'No. of jobs: ' + str(len(ntrees) * len(ncuts))

	# sys.exit()

	# Submission loop iterating thru parameters..
	for t in ntrees:
		current_env['NTREES'] = str(t)
		for c in ncuts:
			current_env['NCUTS'] = str(c)

			# qsub the bash script in current environment
			print 'hello'
			subprocess.call(submission, env=current_env, shell=True)



if __name__ == "__main__":
    main()