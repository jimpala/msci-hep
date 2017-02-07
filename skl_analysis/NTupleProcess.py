import numpy as np
import pandas as pd

from root_numpy import root2array


sample_map = {
    'qqZvvH125': 1,
    'qqWlvH125': 2,
    'Wbb': 3,
    'Wbc': 4,
    'Wcc': 5,
    'Wbl': 6,
    'Wcl': 7,
    'Wl': 8,
    'Zbb': 9,
    'Zbc': 10,
    'Zcc': 11,
    'Zbl': 12,
    'Zcl': 13,
    'Zl': 14,
    'ttbar': 15,
    'stopt': 16,
    'stops': 17,
    'stopWt': 18,
    'WW': 19,
    'ZZ': 20,
    'WZ': 21
}


branch_names = ["sample", "EventWeight", "EventNumber", "nJ", "nBJ", "mBB",
                "dRBB", "dPhiBB", "dEtaBB",
                "dPhiVBB", "dPhiLBmin", "Mtop", "dYWH", "dEtaWH",
                "dPhiLMET", "pTV", "pTB1", "pTB2", "pTL", "etaL",
                "mTW", "MET", "pTJ3", "mBBJ", "BDT"]

# Read in NTuples.
# Output S&B as pseudo 2D ndarrays (array of tuple rows).
signal_direct = root2array("/Volumes/HEPDRIVE/VHbb-data/Direct_Signal.root",
                           treename="Nominal",
                           branches=branch_names)

signal_truth = root2array("/Volumes/HEPDRIVE/VHbb-data/Truth_Signal.root",
                          treename="Nominal",
                          branches=branch_names)

background = root2array("/Volumes/HEPDRIVE/VHbb-data/background_Normal.root",
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

# Map sample names to ints in sample_map.
df['sample'] = df['sample'].map(lambda x: sample_map[x])


# Cutflow.
df = df[df['nBJ'] == 2]
df = df[df['pTV'] > 150000]

df.to_csv(path_or_buf='/Volumes/HEPDRIVE/VHbb-data/CSV/VHbb_data_preselect.csv')

print "NTuple processed to CSV file."