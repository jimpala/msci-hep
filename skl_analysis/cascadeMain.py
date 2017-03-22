#!/bin/python
"""
An executable main script to use the trainSwapFold methods to perform a standard
BDT analysis on the k1 and k2 2-jet DFs.
"""
# Authors: James Pyne
# License: MIT

from trainSwapFold import *
import math
import pandas as pd
from xgboost import XGBClassifier, DMatrix
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from event_obj import Event


def main():
    # Load in the NTuple CSVs as DataFrames.
    df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "DFs loaded."

    print "Beginning 2 jet analysis."
    # Reset indices just to make sure that nothing untoward happens.
    df_2jet_k1 = df_2jet_k1.reset_index(drop=True)
    df_2jet_k2 = df_2jet_k2.reset_index(drop=True)

    # Add a new category column to the DFs.
    df_2jet_k1['category'] = df_2jet_k1['sample'].apply(lambda x: Event.process_general_map[x])
    df_2jet_k2['category'] = df_2jet_k2['sample'].apply(lambda x: Event.process_general_map[x])

    # Separate these DFs into their backgorund category types - signal is kept in each!
    df_2jet_k1_vjets = df_2jet_k1.loc[(df_2jet_k1.category == 'VH') | (df_2jet_k1.category == 'V+jets')]
    df_2jet_k1_ttbar = df_2jet_k1.loc[(df_2jet_k1.category == 'VH') | (df_2jet_k1.category == 'ttbar')]
    df_2jet_k1_stop = df_2jet_k1.loc[(df_2jet_k1.category == 'VH') | (df_2jet_k1.category == 'stop')]
    df_2jet_k1_diboson = df_2jet_k1.loc[(df_2jet_k1.category == 'VH') | (df_2jet_k1.category == 'diboson')]

    df_2jet_k2_vjets = df_2jet_k2.loc[(df_2jet_k2.category == 'VH') | (df_2jet_k2.category == 'V+jets')]
    df_2jet_k2_ttbar = df_2jet_k2.loc[(df_2jet_k2.category == 'VH') | (df_2jet_k2.category == 'ttbar')]
    df_2jet_k2_stop = df_2jet_k2.loc[(df_2jet_k2.category == 'VH') | (df_2jet_k2.category == 'stop')]
    df_2jet_k2_diboson = df_2jet_k2.loc[(df_2jet_k2.category == 'VH') | (df_2jet_k2.category == 'diboson')]

    # Put the DFs through populate_events to get lists of workable events.
    events_k1_vjets = populate_events(df_2jet_k1_vjets, 2)
    events_k1_ttbar = populate_events(df_2jet_k1_ttbar, 2)
    events_k1_stop = populate_events(df_2jet_k1_stop, 2)
    events_k1_diboson = populate_events(df_2jet_k1_diboson, 2)
    events_k1 = populate_events(df_2jet_k1, 2)

    events_k2_vjets = populate_events(df_2jet_k2_vjets, 2)
    events_k2_ttbar = populate_events(df_2jet_k2_ttbar, 2)
    events_k2_stop = populate_events(df_2jet_k2_stop, 2)
    events_k2_diboson = populate_events(df_2jet_k2_diboson, 2)
    events_k2 = populate_events(df_2jet_k2, 2)

    # Set the train weights for these events.

    events_k1_vjets = renormalise_train_weights(events_k1_vjets)
    events_k1_ttbar = renormalise_train_weights(events_k1_ttbar)
    events_k1_stop = renormalise_train_weights(events_k1_stop)
    events_k1_diboson = renormalise_train_weights(events_k1_diboson)
    events_k1 = renormalise_train_weights(events_k1)
    
    events_k2_vjets = renormalise_train_weights(events_k2_vjets)
    events_k2_ttbar = renormalise_train_weights(events_k2_ttbar)
    events_k2_stop = renormalise_train_weights(events_k2_stop)
    events_k2_diboson = renormalise_train_weights(events_k2_diboson)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_2jet_k1_vjets = ready_df_for_cascade_training(df_2jet_k1_vjets, events_k1_vjets)
    df_2jet_k1_ttbar = ready_df_for_cascade_training(df_2jet_k1_ttbar, events_k1_ttbar)
    df_2jet_k1_stop = ready_df_for_cascade_training(df_2jet_k1_stop, events_k1_stop)
    df_2jet_k1_diboson = ready_df_for_cascade_training(df_2jet_k1_diboson, events_k1_diboson)
    df_2jet_k1 = ready_df_for_training(df_2jet_k1, events_k1)
    
    df_2jet_k2_vjets = ready_df_for_cascade_training(df_2jet_k2_vjets, events_k2_vjets)
    df_2jet_k2_ttbar = ready_df_for_cascade_training(df_2jet_k2_ttbar, events_k2_ttbar)
    df_2jet_k2_stop = ready_df_for_cascade_training(df_2jet_k2_stop, events_k2_stop)
    df_2jet_k2_diboson = ready_df_for_cascade_training(df_2jet_k2_diboson, events_k2_diboson)
    df_2jet_k2 = ready_df_for_training(df_2jet_k2, events_k2)
    
    xgb_k1_vjets = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k1_ttbar = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k1_stop = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k1_diboson = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k1 = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    
    xgb_k2_vjets = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k2_ttbar = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k2_stop = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k2_diboson = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)
    xgb_k2 = XGBClassifier(n_estimators=200, max_depth=2, learning_rate=0.1)

    # Cascade train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2 = fold_score_xgb(events_k1, events_k2, xgb_k1, df_2jet_k1, df_2jet_k2)
    events_k1 = fold_score_xgb(events_k2, events_k1, xgb_k2, df_2jet_k2, df_2jet_k1)
    events = events_k1 + events_k2
    print "Done!"

    # Normalise the scores before TrafoD.
    events = normalise_scores(events)
    print "Decision scores normalised."

    # TrafoD and score sensitivity.
    print "TrafoD and sensitivity scoring..."
    sens_2jet, error_2jet = trafo_sensitivity(events)
    print "Done!"

    print "2 jet sensitivity is: {:f}".format(sens_2jet)
    print "2 jet error is: {:f}".format(error_2jet)

    # Plot BDT.
    print "Plotting BDT..."
    decision_plot(events, block=False)

















    print "Beginning 3 jet analysis."
    # Reset indices just to make sure that nothing untoward happens.
    df_3jet_k1 = df_3jet_k1.reset_index(drop=True)
    df_3jet_k2 = df_3jet_k2.reset_index(drop=True)

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_3jet_k1, 3)
    events_k2 = populate_events(df_3jet_k2, 3)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_3jet_k1 = ready_df_for_training(df_3jet_k1, events_k1)
    df_3jet_k2 = ready_df_for_training(df_3jet_k2, events_k2)

    xgb_k1 = XGBClassifier(n_estimators=360, max_depth=2, learning_rate=0.1, subsample=0.5)
    xgb_k2 = XGBClassifier(n_estimators=360, max_depth=2, learning_rate=0.1, subsample=0.5)

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2 = fold_score_xgb(events_k1, events_k2, xgb_k1, df_3jet_k1, df_3jet_k2)
    events_k1 = fold_score_xgb(events_k2, events_k1, xgb_k2, df_3jet_k2, df_3jet_k1)
    events = events_k1 + events_k2
    print "Done!"

    # Normalise the scores before TrafoD.
    events = normalise_scores(events)
    print "Decision scores normalised."

    # TrafoD and score sensitivity.
    print "TrafoD and sensitivity scoring..."
    sens_3jet, error_3jet = trafo_sensitivity(events)
    print "Done!"

    print "3 jet sensitivity is: {:f}".format(sens_3jet)
    print "3 jet error is: {:f}".format(error_3jet)

    # Plot BDT.
    print "Plotting BDT..."
    decision_plot(events, block=False)

    combined_sens = math.sqrt(sens_2jet ** 2 + sens_3jet ** 2)
    combined_error = math.sqrt(error_2jet ** 2 + error_3jet ** 2)
    print "Combined sensitivity is: {:f}".format(combined_sens)
    print "Combined error is: {:f}".format(combined_error)

    print "Script finished."


if __name__ == '__main__':
    main()
