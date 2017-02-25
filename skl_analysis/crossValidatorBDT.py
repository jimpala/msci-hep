import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
import sys

from event_obj import *
from sensitivity import trafoD, calc_sensitivity
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import GridSearchCV
from sklearn import preprocessing
from root_numpy import array2root




def extractData(df, njets):
    df = df.reset_index(drop=True)  # Just to make sure.

    # Get the df attributes. Then drop.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]

    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)  # Drop some cols.

    # Get indices, weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w = np.array([a.event_weight for a in this_events])
    Y = np.array([a.classification for a in this_events])
    X = df.as_matrix()

    return X, Y, w

    # Set up BDTs and fit





def main():
    # Read in 2 jet and 3 jet dataframes from csv.
    df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."


    X_A, Y_A, w_A = extractData(df_2jet_even, 2)
    X_B, Y_B, w_B = extractData(df_2jet_odd, 2)

    bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                             learning_rate=0.15,
                             algorithm="SAMME",
                             n_estimators=200
                             )

    param_grid = {'n_estimators': [200, 300, 400],
                  'base_estimator__max_depth': [2, 3, 4]}

    gs = GridSearchCV(bdt, param_grid, scoring='roc_auc',
                      fit_params={'sample_weight': w_A}, cv=3)


    print "Beginning GridSearch."
    t1 = time.time()
    gs.fit(X_A, Y_A)
    t2 = time.time()
    t = t2 - t1
    print "GridSearch completed in {}".format(str(t))


if __name__ == "__main__":
    main()