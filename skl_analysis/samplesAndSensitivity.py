import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
import sys

from event_obj import *
from crossValidatorBDT import renormalise_weights
from sensitivity import trafoD, calc_sensitivity, trafoD_with_error, calc_sensitivity_with_error
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import preprocessing
from root_numpy import array2root

from trainSwapFold import *

# DEFINE A GENERATOR FUNCTION THAT YIELDS DATAFRAMES
# WITH DIFFERENT SAMPLE FRACTIONS.

def df_generator():
    pass

def main():
    # Read in 2 jet and 3 jet dataframes from csv.
    df_2jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_k1 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_k2 = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)

    events_k1 = populate_events(df_2jet_k1, 2)
    events_k2 = populate_events(df_2jet_k2, 2)

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

    events_k1 = fold_score(events_k1, events_k2, bdt_k1, df_2jet_k1, df_2jet_k2)
    events_k2 = fold_score(events_k2, events_k1, bdt_k2, df_2jet_k2, df_2jet_k1)
    events = events_k1 + events_k2

    events = normalise_scores(events)

    sens, error = trafo_sensitivity(events)
    print sens
    print error

    decision_plot(events)

if __name__ == "__main__":
    main()