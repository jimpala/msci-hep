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
from sklearn.ensemble import GradientBoostingClassifier
from keras.models import Sequential
from keras.layers import Dense
import sys
import json
from datetime import datetime
from copy import deepcopy
from xgboost import XGBClassifier



def main():
    try:
        if sys.argv[1] == 'gpu':
            # Process the data.
            df_2jet_k1 = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_k1 = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_k2 = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_k2 = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

        else:
            # Process the data.
            df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

    except IndexError:
        print "No command line args passed. Running in local mode."

        # Process the data.
        df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
        df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
        df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
        df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
        print "CSV read-in complete."

    print "Beginning 2 jet analysis."
    # Reset indices just to make sure that nothing untoward happens.
    df_2jet_k1 = df_2jet_k1.reset_index(drop=True)
    df_2jet_k2 = df_2jet_k2.reset_index(drop=True)

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_2jet_k1, 2)
    events_k2 = populate_events(df_2jet_k2, 2)
    divider = len(events_k1)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_2jet_k1_untouched = ready_df_for_training(df_2jet_k1, events_k1, drop=False)
    df_2jet_k2_untouched = ready_df_for_training(df_2jet_k2, events_k2, drop=False)
    df_2jet_k1 = ready_df_for_training(df_2jet_k1, events_k1)
    df_2jet_k2 = ready_df_for_training(df_2jet_k2, events_k2)

    # Get events folds for both stack model components.
    events_k1_nn = deepcopy(events_k1)
    events_k2_nn = deepcopy(events_k2)
    events_k1_xgb = deepcopy(events_k1)
    events_k2_xgb = deepcopy(events_k2)

    # Set up NN models.
    model_k1 = Sequential()
    model_k1.add(Dense(14, init='uniform', activation='relu', input_dim=11))
    model_k1.add(Dense(1, init='uniform', activation='sigmoid'))
    model_k1.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy', ])

    model_k2 = Sequential()
    model_k2.add(Dense(14, init='uniform', activation='relu', input_dim=11))
    model_k2.add(Dense(1, init='uniform', activation='sigmoid'))
    model_k2.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy', ])

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2_nn = fold_score_keras(events_k1_nn, events_k2_nn, model_k1, df_2jet_k1, df_2jet_k2)
    events_k1_nn = fold_score_keras(events_k2_nn, events_k1_nn, model_k2, df_2jet_k2, df_2jet_k1)
    events_nn = events_k1_nn + events_k2_nn
    print "Done!"

    # Normalise the scores before TrafoD.
    events_nn = normalise_scores(events_nn)
    events_k1_nn = events_nn[:divider]
    events_k2_nn = events_nn[divider:]
    print "Decision scores normalised."

    xgb_k1 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)
    xgb_k2 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2_xgb = fold_score_xgb(events_k1_xgb, events_k2_xgb, xgb_k1, df_2jet_k1, df_2jet_k2)
    events_k1_xgb = fold_score_xgb(events_k2_xgb, events_k1_xgb, xgb_k2, df_2jet_k2, df_2jet_k1)
    events_xgb = events_k1_xgb + events_k2_xgb
    print "Done!"

    # Normalise the scores before TrafoD.
    events_xgb = normalise_scores(events_xgb)
    events_k1_xgb = events_xgb[:divider]
    events_k2_xgb = events_xgb[divider:]
    print "Decision scores normalised."

    df_2jet_k1['xgb'] = [a.decision_value for a in events_k1_xgb]
    df_2jet_k2['xgb'] = [a.decision_value for a in events_k2_xgb]
    df_2jet_k1['nn'] = [a.decision_value for a in events_k1_nn]
    df_2jet_k2['nn'] = [a.decision_value for a in events_k2_nn]

    print "BEGIN LEVEL 1."

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_2jet_k1_untouched, 2)
    events_k2 = populate_events(df_2jet_k2_untouched, 2)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_2jet_k1 = ready_df_for_training(df_2jet_k1_untouched, events_k1)
    df_2jet_k2 = ready_df_for_training(df_2jet_k2_untouched, events_k2)

    xgb_k1 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)
    xgb_k2 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)

    # Fit and train the events using the events lists, BDTs and DFs.
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
    
    # ---------------------------------------------------------------------------------------
    # ---------------------------------------------------------------------------------------

    print "Beginning 3 jet analysis."
    # Reset indices just to make sure that nothing untoward happens.
    df_3jet_k1 = df_3jet_k1.reset_index(drop=True)
    df_3jet_k2 = df_3jet_k2.reset_index(drop=True)

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_3jet_k1, 3)
    events_k2 = populate_events(df_3jet_k2, 3)
    divider = len(events_k1)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_3jet_k1_untouched = ready_df_for_training(df_3jet_k1, events_k1, drop=False)
    df_3jet_k2_untouched = ready_df_for_training(df_3jet_k2, events_k2, drop=False)
    df_3jet_k1 = ready_df_for_training(df_3jet_k1, events_k1)
    df_3jet_k2 = ready_df_for_training(df_3jet_k2, events_k2)

    # Get events folds for both stack model components.
    events_k1_nn = deepcopy(events_k1)
    events_k2_nn = deepcopy(events_k2)
    events_k1_xgb = deepcopy(events_k1)
    events_k2_xgb = deepcopy(events_k2)

    # Set up NN models.
    model_k1 = Sequential()
    model_k1.add(Dense(14, init='uniform', activation='relu', input_dim=13))
    model_k1.add(Dense(1, init='uniform', activation='sigmoid'))
    model_k1.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy', ])

    model_k2 = Sequential()
    model_k2.add(Dense(14, init='uniform', activation='relu', input_dim=13))
    model_k2.add(Dense(1, init='uniform', activation='sigmoid'))
    model_k2.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy', ])

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2_nn = fold_score_keras(events_k1_nn, events_k2_nn, model_k1, df_3jet_k1, df_3jet_k2)
    events_k1_nn = fold_score_keras(events_k2_nn, events_k1_nn, model_k2, df_3jet_k2, df_3jet_k1)
    events_nn = events_k1_nn + events_k2_nn
    print "Done!"

    # Normalise the scores before TrafoD.
    events_nn = normalise_scores(events_nn)
    events_k1_nn = events_nn[:divider]
    events_k2_nn = events_nn[divider:]
    print "Decision scores normalised."

    xgb_k1 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)
    xgb_k2 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2_xgb = fold_score_xgb(events_k1_xgb, events_k2_xgb, xgb_k1, df_3jet_k1, df_3jet_k2)
    events_k1_xgb = fold_score_xgb(events_k2_xgb, events_k1_xgb, xgb_k2, df_3jet_k2, df_3jet_k1)
    events_xgb = events_k1_xgb + events_k2_xgb
    print "Done!"

    # Normalise the scores before TrafoD.
    events_xgb = normalise_scores(events_xgb)
    events_k1_xgb = events_xgb[:divider]
    events_k2_xgb = events_xgb[divider:]
    print "Decision scores normalised."

    df_3jet_k1['xgb'] = [a.decision_value for a in events_k1_xgb]
    df_3jet_k2['xgb'] = [a.decision_value for a in events_k2_xgb]
    df_3jet_k1['nn'] = [a.decision_value for a in events_k1_nn]
    df_3jet_k2['nn'] = [a.decision_value for a in events_k2_nn]

    print "BEGIN LEVEL 1."

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_3jet_k1_untouched, 3)
    events_k2 = populate_events(df_3jet_k2_untouched, 3)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_3jet_k1 = ready_df_for_training(df_3jet_k1_untouched, events_k1)
    df_3jet_k2 = ready_df_for_training(df_3jet_k2_untouched, events_k2)

    xgb_k1 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)
    xgb_k2 = XGBClassifier(n_estimators=201, max_depth=2, learning_rate=0.1, subsample=0.45, gamma=28.75,
                           reg_lambda=0.6)

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

if __name__ == '__main__':
    main()