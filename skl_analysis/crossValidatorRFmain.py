import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
from datetime import datetime
import sys
import pickle

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.metrics import make_scorer
from crossValidatorBDT import sensitivity_score, extract_data


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
    df = df.reset_index(drop=True)

    X_A, Y_A, w_A, post_w_A, train_w_A = extract_data(df, njets)

    sens_scorer = make_scorer(sensitivity_score, greater_is_better=True, needs_threshold=True)

    rf = RandomForestClassifier(n_estimators=10, max_features=3, min_samples_split=2)

    param_grid = {'n_estimators': np.arange(10, 501, 10),
                  'max_features': [3, 4, 5]}

    test_grid = {'n_estimators': [10],
                 'max_features': [3]}

    # With our SKL hack, enter fit_params as tuples of train and post fit weights.
    fit_params = {'sample_weight': zip(train_w_A, post_w_A)}

    gs = GridSearchCV(rf, test_grid, scoring=sens_scorer,
                      fit_params=fit_params, cv=2)


    print "Beginning GridSearch."
    t1 = time.time()
    gs.fit(X_A, Y_A)
    t2 = time.time()
    t = t2 - t1
    print "GridSearch completed in {}s.".format(str(t))

    try:
        now = datetime.now().strftime('%d%m%y_%H%S')
        pickle.dump(gs, open("gridsearch_RF_{:d}jets_{:d}folds_{}.p".format(njets, kfolds, now), 'wb'))
        print "GS pickle successfully dumped to file gridsearch_{:d}jets_{:d}folds_{}.p.".format(njets, kfolds, now)
    except IndexError:
        print "Terminal argument not entered. Backup dump and exit."
        pickle.dump(gs, open('backup.p', 'wb'))

    print "END"


if __name__ == "__main__":
    main()