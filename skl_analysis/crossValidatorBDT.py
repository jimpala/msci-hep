import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
from datetime import datetime
import sys
import pickle

from event_obj import *
from sensitivity import trafoD, calc_sensitivity
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import make_scorer
from sensitivity import trafoD_tuples, calc_sensitivity_tuples, normalise_decision_scores




def extractData(df, njets):
    df = df.reset_index(drop=True)  # Just to make sure.

    # Get the df attributes. Then drop.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    # Number of jets here is ambiguous.
    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]

    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)  # Drop some cols.

    # Get indices, weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w = np.array([a.event_weight for a in this_events])
    Y = np.array([a.classification for a in this_events])
    X = df.as_matrix()

    return X, Y, w


def sensitivity_score(y, y_pred, sample_weight=None):
    """Gets sensitivity score metric from 1D numpy arras of y,
    predicted y, and post-fit weights."""

    # Weights must be populated.
    assert sample_weight.any()

    # Get all args into 1D lists.
    y = y.reshape(1, -1)

    # Normalise the prediction scores between -1 and 1.
    y_pred = normalise_decision_scores(y_pred)

    # Perform TrafoD on predictions to get optimal bins.
    bins = trafoD_tuples(y, y_pred, sample_weight)

    # Calculate sensitivity with set bins.
    sens = calc_sensitivity_tuples(y, y_pred, sample_weight, bins)

    return sens


def main():
    """This main takes one argv - the number of jets for the analysis."""
    # Read in 2 jet and 3 jet dataframes from csv.
    df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    dfs = list()
    dfs.append(pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True))
    dfs.append(pd.concat([df_3jet_even, df_3jet_odd], axis=0, ignore_index=True))

    try:
        df = dfs.pop(1) if sys.argv[1] == '3' else dfs.pop(0)
        njets = 3 if sys.argv[1] == '3' else 2
    except IndexError:
        print "No njets command line argument. Using default 2 jet analysis."
        df = dfs.pop(0)
        njets = 2
    try:
        kfolds = int(sys.argv[2])
    except IndexError:
        print "No kfolds command line argument. Using default 2 folds."
        kfolds = 2

    del dfs

    # Shuffle the DF.
    df = df.sample(frac=1, random_state=42)


    X_A, Y_A, w_A = extractData(df, njets)

    sens_scorer = make_scorer(sensitivity_score, greater_is_better=True, needs_threshold=True)

    bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                             learning_rate=0.15,
                             algorithm="SAMME",
                             n_estimators=200
                             )

    param_grid = {'n_estimators': np.arange(100, 401, 10),
                  'base_estimator__max_depth': [2, 3, 4]}

    test_grid = {'n_estimators': [100],
                  'base_estimator__max_depth': [2]}

    fit_params = {'sample_weight': w_A}

    gs = GridSearchCV(bdt, param_grid, scoring=sens_scorer,
                      fit_params=fit_params, cv=2)


    print "Beginning GridSearch."
    t1 = time.time()
    gs.fit(X_A, Y_A)
    t2 = time.time()
    t = t2 - t1
    print "GridSearch completed in {}".format(str(t))

    try:
        now = datetime.now().strftime('%d%m%y_%H%S')
        pickle.dump(gs, open("gridsearch_{:d}jets_{:d}folds_{}.p.".format(njets, kfolds, now), 'wb'))
        print "GS pickle successfully dumped to file gridsearch_{:d}jets_{:d}folds_{}.p.".format(njets, kfolds, now)
    except IndexError:
        print "Terminal argument not entered. Backup dump and exit."
        pickle.dump(gs, open('backup.p', 'wb'))

    print "END"


if __name__ == "__main__":
    main()