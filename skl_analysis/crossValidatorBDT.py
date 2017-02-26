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
from sklearn.metrics import make_scorer
from sensitivity import trafoD_tuples, calc_sensitivity_tuples, normalise_decision_scores




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
    # Read in 2 jet and 3 jet dataframes from csv.
    df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    df_2jet = pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True)

    # Shuffle the DF.
    df_2jet = df_2jet.sample(frac=1, random_state=42)


    X_A, Y_A, w_A = extractData(df_2jet, 2)

    sens_scorer = make_scorer(sensitivity_score, greater_is_better=True, needs_threshold=True)

    bdt = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                             learning_rate=0.15,
                             algorithm="SAMME",
                             n_estimators=200
                             )

    param_grid = {'n_estimators': [200, 300],
                  'base_estimator__max_depth': [2, 3]}

    fit_params = {'sample_weight': w_A}

    gs = GridSearchCV(bdt, param_grid, scoring=sens_scorer,
                      fit_params=fit_params, cv=2)


    print "Beginning GridSearch."
    t1 = time.time()
    gs.fit(X_A, Y_A)
    t2 = time.time()
    t = t2 - t1
    print "GridSearch completed in {}".format(str(t))

    print "END"


if __name__ == "__main__":
    main()