import os
import sys
import subprocess
import numpy as np

# Environment setup.
current_env = os.environ.copy()

# qsub submission string.
submission = 'qsub -N \"EGStrain_trees\"${NTREES}\"_cuts\"${NCUTS} -q short -v submission.sh'

# BDT hyperparameter space.
ntrees = np.arange(100,120,10)
ncuts = np.arange(100,120,10)

# Constants.
# (blank)
maxdepth = 4


print repr(submission)
print 'No. of jobs: ' + str(len(ntrees) * len(ncuts))

sys.exit()

# Submission loop iterating thru parameters..
for t in ntrees:
	current_env['NTREES'] = str(t)
	for c in ncuts:
		current_env['NCUTS'] = str(c)

		# qsub the bash script in current environment
		subprocess.Popen('./submission.sh', env=current_env)



if __name__ == "__main__":
    main()