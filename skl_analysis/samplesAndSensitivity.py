import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
import sys
import json

from event_obj import *
from crossValidatorBDT import renormalise_weights
from sensitivity import trafoD, calc_sensitivity, trafoD_with_error, calc_sensitivity_with_error
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import preprocessing
from trainSwapFold import *


def main():
    # Read in 2 jet and 3 jet dataframes from csv.
    df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)

    json_store = dict()

    # 2 JET
    #######
    df = pd.concat([df_2jet_k1, df_2jet_k2], axis=0, ignore_index=True)
    json_store['nsamples_2jet'] = len(df)
    sample_fracs = np.arange(0.1, 1.05, 0.05)
    json_store['sample_fracs'] = sample_fracs

    df_gen = (df.sample(frac=a, random_state=42) for a in sample_fracs)

    print "Beginning generator loop for 2 jet."

    try:
        sens_2jet = list()
        err_2jet = list()

        while True:
            print "Loading randomly sampled data..."
            this_df = df_gen.next()
            this_df = this_df.reset_index(drop=True)

            this_df_k1 = this_df.ix[:len(this_df)/2]
            this_df_k2 = this_df.ix[len(this_df)/2:]

            events_k1 = populate_events(this_df_k1, 2)
            events_k2 = populate_events(this_df_k2, 2)

            this_df_k1 = this_df_k1.ix[[a.index for a in events_k1]]
            this_df_k2 = this_df_k2.ix[[a.index for a in events_k2]]

            this_df_k1 = ready_df_for_training(this_df_k1)
            this_df_k2 = ready_df_for_training(this_df_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Scoring folded events..."
            events_k2 = fold_score(events_k1, events_k2, bdt_k1, this_df_k1, this_df_k2)
            events_k1 = fold_score(events_k2, events_k1, bdt_k2, this_df_k2, this_df_k1)
            events = events_k1 + events_k2

            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_2jet.append(this_sens)
            err_2jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "2 jet iteration completed."

    except StopIteration:
        print "2 jet analysis fully completed."

    finally:
        json_store['sens_2jet'] = sens_2jet
        json_store['err_2jet'] = err_2jet

    # 3 JET
    #######
    df = pd.concat([df_3jet_k1, df_3jet_k2], axis=0, ignore_index=True)
    json_store['nsamples_3jet'] = len(df)
    sample_fracs = np.arange(0.1, 1.05, 0.05)

    df_gen = (df.sample(frac=a, random_state=42) for a in sample_fracs)

    print "Beginning generator loop for 3 jet."

    try:
        sens_3jet = list()
        err_3jet = list()

        while True:
            print "Loading randomly sampled data..."
            this_df = df_gen.next()
            this_df = this_df.reset_index(drop=True)

            this_df_k1 = this_df.ix[:len(this_df)/2]
            this_df_k2 = this_df.ix[len(this_df)/2:]

            events_k1 = populate_events(this_df_k1, 2)
            events_k2 = populate_events(this_df_k2, 2)

            this_df_k1 = this_df_k1.ix[[a.index for a in events_k1]]
            this_df_k2 = this_df_k2.ix[[a.index for a in events_k2]]

            this_df_k1 = ready_df_for_training(this_df_k1)
            this_df_k2 = ready_df_for_training(this_df_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )

            print "Scoring folded events..."
            events_k2 = fold_score(events_k1, events_k2, bdt_k1, this_df_k1, this_df_k2)
            events_k1 = fold_score(events_k2, events_k1, bdt_k2, this_df_k2, this_df_k1)
            events = events_k1 + events_k2

            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_3jet.append(this_sens)
            err_3jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "3 jet iteration completed."

    except StopIteration:
        print "3 jet analysis fully completed."

    finally:
        json_store['sens_3jet'] = sens_3jet
        json_store['err_3jet'] = err_3jet

    # Dump JSON to file.
    with open('samplesVsSensitivity.json', 'w') as fp:
        json.dump(json_store, fp)

    print "Dumped analysis data to samplesVsSensitivity.json. Analysis complete."


if __name__ == "__main__":
    main()