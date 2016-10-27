import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm

df_import = pd.DataFrame.from_csv('lowpt_2jet.csv')

nCuts = df_import['nCuts'].tolist()
nTrees = df_import['nTrees'].tolist()
ROCint = df_import['ROCint'].tolist()

heat_colormap = cm.get_cmap('hot')

plt.hexbin(nCuts,nTrees,C=ROCint,cmap=heat_colormap, gridsize=(20,16))

plt.show()