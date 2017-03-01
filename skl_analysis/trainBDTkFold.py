import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
import sys

from event_obj import *
from crossValidatorBDT import renormalise_weights
from sensitivity import trafoD, calc_sensitivity
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import preprocessing
from root_numpy import array2root

# Read in 2 jet and 3 jet dataframes from csv.
df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)

df_list = [(df_2jet_even, df_2jet_odd), (df_3jet_even, df_3jet_odd)]
print "CSV read-in completed."

# Collect scores to add to NTuple at the end.
scores_dict = dict()

# This for loop is for 2/3jet
for j, njets in zip(range(2), (2, 3)):
    # Select tuple of folds.
    dfs = df_list[j]

    # Initialise lists to collect stuff.
    bdt_list = []
    X_list = []
    events_list = []

    for df in dfs:
        df = df.reset_index(drop=True)  # Just to make sure.

        # Get the df attributes. Then drop.
        processes = df['sample'].as_matrix().tolist()
        indices = df.index.values.tolist()
        event_weights = df['EventWeight'].as_matrix().tolist()

        args_zipped = zip(processes, indices, event_weights)

        this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]
        this_events = renormalise_weights(this_events)

        df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)  # Drop some cols.

        print "Event list populated."

        # Get indices, weights and classes for each of these splits.
        # w and Y need to be numpy arrays to work with skl.
        indices = [a.index for a in this_events]
        w = np.array([a.event_weight for a in this_events])
        Y = np.array([a.classification for a in this_events])

        # Index our X training sets by row; convert to ndarrays.
        X = df.ix[indices].as_matrix()

        print "Commencing BDT training..."

        # Set up BDTs and fit
        bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                 learning_rate=0.15,
                                 algorithm="SAMME",
                                 n_estimators=200
                                 )

        bdt.fit(X, Y, sample_weight=w)

        X_list.insert(0, X)
        bdt_list.insert(0, bdt)
        events_list += this_events
        print "BDT training completed."

    # Get scores of X_A for BDT_B and vice-versa.
    scores_odd = bdt_list[0].decision_function(X_list[1]).tolist()
    scores_even = bdt_list[1].decision_function(X_list[0]).tolist()
    print "Non-normalised decision function scores processed."

    # Normalise decision scores between -1 and 1.
    max_score = max([a for a in scores_odd + scores_even])
    min_score = min([a for a in scores_odd + scores_even])
    score_range = max_score - min_score
    score_midpoint = min_score + score_range / 2
    # Translate and shrink.
    scores_odd = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.000001), scores_odd)  # .001 added for bounding
    scores_even = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.000001), scores_even)


    print "Updating event objects with decision scores..."

    # Process scores to Event objects.
    for e, s in zip(events_list, scores_even + scores_odd):
        e.set_decision_value(s)
    print "Event objects updated."

    # Call TrafoD on Event list.
    # TIME: 0.3562s
    print "Implementing TrafoD histogram bin transform."
    bins = trafoD(events_list)

    # Calculate sensitivity.
    # TIME: 0.7216s
    #
    # 2 JET SENSITIVITY: 2.799
    # 3 JET SENSITIVITY: 1.634
    # TOTAL: 3.241
    sens = calc_sensitivity(events_list, bins)
    print "Sensitivity calculation: {}".format(str(sens))

    print "Plotting results..."

    # Initialise plot stuff
    plt.ion()
    plt.clf()
    plot_range = (-1, 1)
    class_names = Event.processes
    plot_data = []
    plot_weights = []
    plot_colors = []

    # Get list of hists.
    for c in class_names:
        plot_data.append([a.decision_value for a in events_list if a.process == c])
        plot_weights.append([a.post_fit_weight for a in events_list if a.process == c])
        plot_colors.append(Event.process_color_map[c])

    # Plot.
    plt.hist(plot_data,
             bins=bins,
             weights=plot_weights,
             range=plot_range,
             rwidth=1,
             color=plot_colors,
             label=class_names,
             stacked=True,
             edgecolor='None')
    x1, x2, y1, y2 = plt.axis()
    plt.axis((x1, x2, y1, y2 * 1.2))
    plt.legend(loc='upper right', ncol=2, prop={'size': 12})
    plt.ylabel('Samples')
    plt.xlabel('Score')
    plt.title('Decision Scores')

    plt.savefig('fig_{}jet.png'.format(njets))
    plt.show(block=True)

    # Put scores in dict.
    scores_dict['{}_jet'.format(str(njets))] = scores_even + scores_odd

# Concat and prepare 2/3 jet arrays with BDT scores.
df_2jet = pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True)
df_2jet['BDT'] = scores_dict['2_jet']

df_3jet = pd.concat([df_3jet_even, df_3jet_odd], axis=0, ignore_index=True)
df_3jet['BDT'] = scores_dict['3_jet']

df_ntuple = pd.concat([df_2jet, df_3jet], axis=0, ignore_index=True)
df_ntuple = df_ntuple.fillna(0)

# Convert to structured array
ntuple_list = df_ntuple.as_matrix().tolist()
ntuple_list = [tuple(a) for a in ntuple_list]

name_type_ref = [('BDT', 'f4'),
                 ('Class', 'i4'),
                 ('EventNumber', 'i4'),
                 ('EventWeight', 'f4'),
                 ('MET', 'f4'),
                 ('Mtop', 'f4'),
                 ('dPhiLBmin', 'f4'),
                 ('dPhiVBB', 'f4'),
                 ('dRBB', 'f4'),
                 ('dYWH', 'f4'),
                 ('mBB', 'f4'),
                 ('mBBJ', 'f4'),
                 ('mTW', 'f4'),
                 ('nBJ', 'i4'),
                 ('nJ', 'i4'),
                 ('pTB1', 'f4'),
                 ('pTB2', 'f4'),
                 ('pTJ3', 'f4'),
                 ('pTV', 'f4'),
                 ('sample', 'S15')]

ntuple_array = np.array(ntuple_list, dtype=name_type_ref)

# Write to ROOT file.
array2root(ntuple_array, '/Volumes/THUMB/VHbb-data/write/skl_BDT_results.root',
           mode='recreate')