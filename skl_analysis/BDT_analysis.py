import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import train_test_split

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

# Split into 2 jet and 3 jet trainings.
df_2jet = df[df['nJ'] == 2]
df_3jet = df[df['nJ'] == 3]

# Classification array for these DFs.
classification_2jet = df_2jet['Class'].as_matrix()
classification_3jet = df_3jet['Class'].as_matrix()

# Drop unneeded columns for the training.
df_2jet_filtered = df_2jet.drop(['sample','EventWeight','nJ','nBJ', 'dEtaBB', 'dPhiBB',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', 'Class'])
df_3jet_filtered = df_3jet.drop(['sample','EventWeight','nJ','nBJ', 'dEtaBB', 'dPhiBB',
                                 'dEtaWH', 'dPhiLMET', 'BDT', 'pTL', 'etaL', 'Class'])

# Convert to ndarray.
dataset_2jet = df_2jet_filtered.as_matrix()
dataset_3jet = df_3jet_filtered.as_matrix()

# Split into training and test sets.
train_2jet, test_2jet, train_2jet_class, test_2jet_class = train_test_split(dataset_2jet,
                                                                            classification_2jet,
                                                                            test_size=100000,
                                                                            train_size=300000,
                                                                            random_state=42)

train_3jet, test_3jet, train_3jet_class, test_3jet_class = train_test_split(dataset_3jet,
                                                                            classification_3jet,
                                                                            test_size=100000,
                                                                            train_size=300000,
                                                                            random_state=42)

# Create BDT object.
bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3),
                         algorithm="SAMME",
                         n_estimators=200)

# Train BDT for 2 jet.
bdt.fit(train_2jet, train_2jet_class)

# Plot decision scores for test set.
twoclass_output = bdt.decision_function(test_2jet)
plot_range = (twoclass_output.min(), twoclass_output.max())
plt.subplot(122)

plot_colors = "br"
plot_step = 0.02
class_names = "SB"

for i, n, c in zip(range(2), class_names, plot_colors):
    plt.hist(twoclass_output[test_2jet_class == i],
             bins=10,
             range=plot_range,
             facecolor=c,
             label='Class %s' % n,
             alpha=.5)
x1, x2, y1, y2 = plt.axis()
plt.axis((x1, x2, y1, y2 * 1.2))
plt.legend(loc='upper right')
plt.ylabel('Samples')
plt.xlabel('Score')
plt.title('Decision Scores')

plt.tight_layout()
plt.subplots_adjust(wspace=0.35)
plt.show()