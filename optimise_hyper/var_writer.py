import numpy as np

f = open('BDT_vars.txt', 'w')

trees = np.arange(100,410,10)
depth = np.arange(3,8,1)

for t in trees:
	for d in depth:
		f.write('BDT_'+str(t)+'ntrees'+str(d)+'maxdepth'+'\n')

f.close()