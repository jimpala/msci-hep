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


def populate_events(df, njets):
    df = df.reset_index(drop=True)  # Just to make sure.

    # Get the df attributes. Then drop.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    events = [Event(a, njets, b, c) for a, b, c in args_zipped]
    events = renormalise_weights(events)

    return events


def ready_df_for_training(df):
    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)  # Drop some cols.

    return df


def fold_score(events_A, events_B, bdt_A, df_A, df_B):

    # Get indices, weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w_A = np.array([a.event_weight for a in events_A])
    Y_A = np.array([a.classification for a in events_A])

    # Index our X training sets by row; convert to ndarrays.
    X_A = df_A.as_matrix()
    X_B = df_B.as_matrix()

    bdt_A.fit(X_A, Y_A, sample_weight=w_A)

    # Get scores of X_A for BDT_B and vice-versa.
    scores = bdt_A.decision_function(X_B).tolist()

    for e, s in zip(events_B, scores):
        e.set_decision_value(s)

    return events_B


def normalise_scores(events):

    scores = [a.decision_value for a in events]

    # Normalise decision scores between -1 and 1.
    max_score = max(scores)
    min_score = min(scores)
    score_range = max_score - min_score
    score_midpoint = min_score + score_range / 2
    # Translate and shrink. .001 added for bounding
    scores = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.000001), scores)

    for e, s in zip(events, scores):
        e.set_decision_value(s)
    return events


def trafo_sensitivity(events, error=True):

    # PUT EVENT OBJECTS IN TEST MODE.
    for e in events:
        e.set_test_mode()

    # Call TrafoD on Event list.
    print "Implementing TrafoD histogram bin transform."
    bins, delta_bins_s, delta_bins_b = trafoD_with_error(events)

    # Calculate sensitivity.
    sens, error = calc_sensitivity_with_error(events, bins, delta_bins_s, delta_bins_b)

    return sens, error


def decision_plot(events):

    # PUT EVENT OBJECTS IN TEST MODE.
    for e in events:
        e.set_test_mode()

    # Call TrafoD on Event list.
    print "Implementing TrafoD histogram bin transform."
    bins, delta_bins_s, delta_bins_b = trafoD_with_error(events)

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
        plot_data.append([a.decision_value for a in events if a.process == c])
        plot_weights.append([a.post_fit_weight for a in events if a.process == c])
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

    plt.show(block=True)
