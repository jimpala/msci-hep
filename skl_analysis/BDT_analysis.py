import numpy as np
import pandas as pd
#import matplotlib.pyplot as plt



from root_numpy import root2array, rec2array

branch_names = ["EventWeight", "EventNumber", "nJ", "nBJ", "mBB",
                "dRBB", "dPhiBB", "dEtaBB",
                "dPhiVBB", "dPhiLBmin", "Mtop", "dYWH", "dEtaWH",
                "dPhiLMET", "pTV", "pTB1", "pTB2", "pTL", "etaL",
                "mTW", "MET", "pTJ3", "mBBJ", "BDT"]

# Output S&B as pseudo 2D ndarrays (array of tuple rows).
signal = root2array("WH125_Direct.root",
                    treename="Nominal",
                    branches=branch_names)

background = root2array("singletop_Wt_Direct.root",
                    treename="Nominal",
                    branches=branch_names)

# Configure as DataFrames.
signal_df = pd.DataFrame(signal, columns=branch_names)
signal_df['Class'] = pd.Series(np.ones(len(signal_df)))

background_df = pd.DataFrame(background, columns=branch_names)
background_df['Class'] = pd.Series(np.zeros(len(background_df)))

# Concatenate S&B dfs.
df = pd.concat([signal_df,background_df])

print df