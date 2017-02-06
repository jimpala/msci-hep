import numpy as np
import pandas as pd
#import matplotlib.pyplot as plt

from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier

from root_numpy import root2array, rec2array

branch_names = ["EventWeight", "EventNumber", "nJ", "nBJ", "mBB",
                "dRBB", "dPhiBB", "dEtaBB",
                "dPhiVBB", "dPhiLBmin", "Mtop", "dYWH", "dEtaWH",
                "dPhiLMET", "pTV", "pTB1", "pTB2", "pTL", "etaL",
                "mTW", "MET", "pTJ3", "mBBJ", "BDT"]

# Output S&B as pseudo 2D ndarrays (array of tuple rows).
signal_direct = root2array("/Volumes/HEPDRIVE/VHbb-data/Results/Direct_Signal.root",
                    treename="Nominal",
                    branches=branch_names)

signal_truth = root2array("/Volumes/HEPDRIVE/VHbb-data/Results/Truth_Signal.root",
                    treename="Nominal",
                    branches=branch_names)

background = root2array("/Volumes/HEPDRIVE/VHbb-data/Results/background_Normal.root",
                    treename="Nominal",
                    branches=branch_names)

print "NTuple read-in complete."

# Configure as DataFrames.
signal_direct_df = pd.DataFrame(signal_direct, columns=branch_names)
signal_direct_df['Class'] = pd.Series(np.ones(len(signal_direct_df)))

signal_truth_df = pd.DataFrame(signal_truth, columns=branch_names)
signal_truth_df['Class'] = pd.Series(np.ones(len(signal_truth_df)))

signal_df = pd.concat([signal_direct_df,signal_truth_df])

background_df = pd.DataFrame(background, columns=branch_names)
background_df['Class'] = pd.Series(np.zeros(len(background_df)))

# Concatenate S&B dfs.
df = pd.concat([signal_direct_df, background_df])

dataset = df.as_matrix()

print dataset