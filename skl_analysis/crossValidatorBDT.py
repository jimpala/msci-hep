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


def renormalise_weights(event_list):

    # Get S&B event lists.
    sig_events = [a for a in event_list if a.classification == 1]
    back_events = [a for a in event_list if a.classification == 0]

    # Get S&B event weight sums and frequencies.
    sig_weight_sum = sum([a.event_weight for a in sig_events])
    back_weight_sum = sum([a.event_weight for a in back_events])
    sig_freq = len(sig_events)
    back_freq = len(back_events)

    # Get renormalisation scale factors. freq = weight_sum * scale.
    sig_scale = sig_freq / sig_weight_sum
    back_scale = back_freq / back_weight_sum

    # Scale them up and recalculate post fit weights.
    for s in sig_events:
        s.rescale_weights(sig_scale)
    for b in back_events:
        b.rescale_weights(back_scale)

    # Concatenate the arrays and return.
    scaled_event_list = sig_events + back_events
    return scaled_event_list


def extract_data(df, njets, renormalised=True):
    df = df.reset_index(drop=True)  # Just to make sure.

    # Get the df attributes. Then drop.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    # Number of jets here is ambiguous.
    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]

    # Optional renormalisation
    if renormalised:
        this_events = renormalise_weights(this_events)

    # Drop some cols.
    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)

    # Get indices, weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w = np.array([a.event_weight for a in this_events])
    post_w = np.array([a.post_fit_weight for a in this_events])
    Y = np.array([a.classification for a in this_events])
    X = df.as_matrix()


    return X, Y, w, post_w


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