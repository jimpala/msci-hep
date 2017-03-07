#!/bin/python
"""
An executable main script to use the trainSwapFold methods to perform a standard
BDT analysis on the k1 and k2 2-jet DFs.
"""
# Authors: James Pyne
# License: MIT

from trainSwapFold import *
import pandas as pd
import numpy as np
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier

def main():
    # Load in the NTuple CSVs as DataFrames.
    df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "DFs loaded."

    # Reset indices just to make sure that nothing untoward happens.
    df_2jet_k1 = df_2jet_k1.reset_index(drop=True)
    df_2jet_k2 = df_2jet_k2.reset_index(drop=True)

    # Put the DFs through populate_events to get lists of workable events.
    events_k1 = populate_events(df_2jet_k1, 2)
    events_k2 = populate_events(df_2jet_k2, 2)

    # Set the train weights for these events.
    events_k1 = renormalise_train_weights(events_k1)
    events_k2 = renormalise_train_weights(events_k2)
    print "Event list ready."

    # Ready the DFs for training and scoring.
    df_2jet_k1 = ready_df_for_training(df_2jet_k1)
    df_2jet_k2 = ready_df_for_training(df_2jet_k2)

    bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                learning_rate=0.15,
                                algorithm="SAMME",
                                n_estimators=200
                                )

    bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                learning_rate=0.15,
                                algorithm="SAMME",
                                n_estimators=200
                                )

    # Fit and train the events using the events lists, BDTs and DFs.
    # Notice the ordering!
    print "Training and decision scoring..."
    events_k2 = fold_score(events_k1, events_k2, bdt_k1, df_2jet_k1, df_2jet_k2)
    events_k1 = fold_score(events_k2, events_k1, bdt_k2, df_2jet_k2, df_2jet_k1)
    events = events_k1 + events_k2
    print "Done!"

    # Normalise the scores before TrafoD.
    events = normalise_scores(events)
    print "Decision scores normalised."

    # TrafoD and score sensitivity.
    print "TrafoD and sensitivity scoring..."
    sens, error = trafo_sensitivity(events)
    print "Done!"

    print "Sensitivity is: {:f}".format(sens)
    print "Error is: {:f}".format(error)

    # Plot BDT.
    print "Plotting BDT..."
    decision_plot(events)

    print "Script finished."


if __name__ == '__main__':
    main()