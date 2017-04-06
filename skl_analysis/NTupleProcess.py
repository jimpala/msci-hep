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


branch_names = ["sample", "EventWeight", "EventNumber", "ChannelNumber", "isOdd", "weight",
                "nJ", "nTags", "nSigJet", "nForwardJet", "mBB",
                "dRBB", "dPhiBB", "dEtaBB", "sumPt", "pTB1", "pTB2", "pTBB",
                "pTBBoverMET", "etaB1", "etaB2", "MET", "MPT", "HT", "METHT", "MV1cB1",
                "MV1cB2", "MV1cJ3", "pTJ3", "etaJ3", "dRB1J3", "dRB2J3", "mBBJ",
                "dPhiVBB", "dPhiMETMPT", "dPhiMETdijet", "mindPhi", "BDT",
                "dPhiLBmin", "Mtop", "dYWH", "dEtaWH", "dPhiLMET", "pTL", "etaL", "mTW", "pTV"]

# Read in NTuples.
# Output S&B as pseudo 2D ndarrays (array of tuple rows).
signal_direct = root2array("/Volumes/THUMB/VHbb-data/Direct_Signal.root",
                           treename="Nominal",
                           branches=branch_names)

signal_truth = root2array("/Volumes/THUMB/VHbb-data/Truth_Signal.root",
                          treename="Nominal",
                          branches=branch_names)

background = root2array("/Volumes/THUMB/VHbb-data/background_Normal.root",
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
#df['sample'] = df['sample'].map(lambda x: sample_map[x])


# Cutflow.
df = df[df['nTags'] == 2]

# Split into 2 jet and 3 jet trainings.
df_2jet = df[df['nJ'] == 2]
df_3jet = df[df['nJ'] == 3]

# Split these once again by even/odd event number.
df_2jet_even = df_2jet[df_2jet['EventNumber'] % 2 == 0]
df_2jet_odd = df_2jet[df_2jet['EventNumber'] % 2 == 1]
df_3jet_even = df_3jet[df_3jet['EventNumber'] % 2 == 0]
df_3jet_odd = df_3jet[df_3jet['EventNumber'] % 2 == 1]


# Drop unneeded columns for the training.
df_2jet_even_filtered = df_2jet_even.drop(['dEtaBB', 'dPhiBB', 'weight',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', "sumPt",
                                 "ChannelNumber", "isOdd", "nSigJet", "nForwardJet",
                                 "pTBB", "pTBBoverMET", "etaB1", "etaB2", "dEtaBB",
                                 "HT", "METHT", "MV1cB1", "MV1cB2", "MV1cJ3", "MPT",
                                 "etaJ3", "dRB1J3", "dRB2J3", "dPhiMETMPT", "dPhiMETdijet",
                                 "mindPhi", "dEtaWH", "dPhiLMET",
                                 'mBBJ', 'pTJ3'], axis=1)
df_3jet_even_filtered = df_3jet_even.drop(['dEtaBB', 'dPhiBB', 'weight',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', "sumPt",
                                 "ChannelNumber", "isOdd", "nSigJet", "nForwardJet",
                                 "pTBB", "pTBBoverMET", "etaB1", "etaB2", "dEtaBB",
                                 "HT", "METHT", "MV1cB1", "MV1cB2", "MV1cJ3", "MPT",
                                 "etaJ3", "dRB1J3", "dRB2J3", "dPhiMETMPT", "dPhiMETdijet",
                                 "mindPhi", "dEtaWH", "dPhiLMET"], axis=1)
df_2jet_odd_filtered = df_2jet_odd.drop(['dEtaBB', 'dPhiBB', 'weight',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', "sumPt",
                                 "ChannelNumber", "isOdd", "nSigJet", "nForwardJet",
                                 "pTBB", "pTBBoverMET", "etaB1", "etaB2", "dEtaBB",
                                 "HT", "METHT", "MV1cB1", "MV1cB2", "MV1cJ3", "MPT",
                                 "etaJ3", "dRB1J3", "dRB2J3", "dPhiMETMPT", "dPhiMETdijet",
                                 "mindPhi", "dEtaWH", "dPhiLMET",
                                 'mBBJ', 'pTJ3'], axis=1)
df_3jet_odd_filtered = df_3jet_odd.drop(['dEtaBB', 'dPhiBB', 'weight',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', "sumPt",
                                 "ChannelNumber", "isOdd", "nSigJet", "nForwardJet",
                                 "pTBB", "pTBBoverMET", "etaB1", "etaB2", "dEtaBB",
                                 "HT", "METHT", "MV1cB1", "MV1cB2", "MV1cJ3", "MPT",
                                 "etaJ3", "dRB1J3", "dRB2J3", "dPhiMETMPT", "dPhiMETdijet",
                                 "mindPhi", "dEtaWH", "dPhiLMET"], axis=1)


# df_3jet_even_filtered = df_3jet_even.drop(['dEtaBB', 'dPhiBB',
#                                  'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL'], axis=1)
# df_2jet_odd_filtered = df_2jet_odd.drop(['dEtaBB', 'dPhiBB',
#                                  'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL',
#                                  'mBBJ', 'pTJ3'], axis=1)
# df_3jet_odd_filtered = df_3jet_odd.drop(['dEtaBB', 'dPhiBB',
#                                  'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL'], axis=1)

df_2jet_even_filtered.to_csv(path_or_buf='/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv')
df_3jet_even_filtered.to_csv(path_or_buf='/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv')
df_2jet_odd_filtered.to_csv(path_or_buf='/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv')
df_3jet_odd_filtered.to_csv(path_or_buf='/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv')

print "NTuple processed to CSV file."

