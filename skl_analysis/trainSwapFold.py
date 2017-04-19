import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from event_obj import *
from sensitivity import trafoD_with_error, calc_sensitivity_with_error
from keras.callbacks import EarlyStopping
from sklearn.preprocessing import scale
from datetime import datetime


def populate_events(df, njets, train_weights=False):
    """Returns a list of n-jet events corresponding to the entries in passed DF."""

    # Get the df attributes. Zip them up. Then enter them via a list iteration.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    events = [Event(a, njets, b, c) for a, b, c in args_zipped]

    if train_weights:
        events = renormalise_train_weights(events)

    return events


def renormalise_train_weights(event_list):
    """Takes a list of events and sets their renormalised training weights."""

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
        s.set_train_weight(sig_scale)
    for b in back_events:
        b.set_train_weight(back_scale)

    # Concatenate the arrays and return.
    scaled_event_list = sig_events + back_events
    return scaled_event_list


def ready_df_for_training(df, events, drop=True):
    """Returns df with non-training variables dropped and ordered by event list."""

    if drop:
        # Drop unneeded cols.
        df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nTags'], axis=1)

    # Order by event list.
    df = df.ix[[a.index for a in events]]

    return df


def ready_df_for_cascade_training(df, events):
    """Returns df with non-training variables dropped and ordered by event list."""
    # Drop unneeded cols.
    df = df.drop(['sample', 'category', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nTags'], axis=1)

    # Order by event list.
    df = df.ix[[a.index for a in events]]

    return df


def fold_score(events_A, events_B, bdt_A, df_A, df_B):
    """Returns scored events_B for a BDT_A."""

    # Get indices, train weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w_A = np.array([a.train_weight for a in events_A])
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


def fold_score_xgb(events_A, events_B, xgb_A, df_A, df_B):
    """Returns scored events_B for a BDT_A."""

    # Get indices, train weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w_A = np.array([a.train_weight for a in events_A])
    Y_A = np.array([a.classification for a in events_A])

    # Index our X training sets by row; convert to ndarrays.
    X_A = df_A.as_matrix()
    X_B = df_B.as_matrix()


    xgb_A.fit(X_A, Y_A, sample_weight=w_A)

    # Get scores of X_A for BDT_B and vice-versa.
    prob_tuples = xgb_A.predict_proba(X_B).tolist()
    scores = [a[1] for a in prob_tuples]

    for e, s in zip(events_B, scores):
        e.set_decision_value(s)

    return events_B


def fold_score_proba(events_A, events_B, bdt_A, df_A, df_B):
    """Returns scored events_B for a BDT_A."""

    # Get indices, train weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w_A = np.array([a.train_weight for a in events_A])
    Y_A = np.array([a.classification for a in events_A])

    # Index our X training sets by row; convert to ndarrays.
    X_A = df_A.as_matrix()
    X_B = df_B.as_matrix()

    bdt_A.fit(X_A, Y_A, sample_weight=w_A)

    # Get scores of X_A for BDT_B and vice-versa.
    prob_tuples = bdt_A.predict_proba(X_B).tolist()
    # Only want the second element of the prob tuple (prob of signal).
    scores = [a[1] for a in prob_tuples]

    for e, s in zip(events_B, scores):
        e.set_decision_value(s)

    return events_B


def fold_score_proba_knn(events_A, events_B, knn_A, df_A, df_B):
    """Returns scored events_B for a BDT_A."""

    # Get indices, train weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    Y_A = np.array([a.classification for a in events_A])

    # Index our X training sets by row; convert to ndarrays.
    X_A = df_A.as_matrix()
    X_B = df_B.as_matrix()

    knn_A.fit(X_A, Y_A)

    # Get scores of X_A for BDT_B and vice-versa.
    scores = knn_A.predict_proba(X_B).tolist()
    # Only want the second element of the prob tuple (prob of signal).
    scores = map(lambda a: a[1], scores)

    for e, s in zip(events_B, scores):
        e.set_decision_value(s)

    return events_B

def fold_score_keras(events_A, events_B, model_A, df_A, df_B):
    """Returns scored events_B for a BDT_A."""

    # Get indices, train weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w_A = np.array([a.train_weight for a in events_A])
    w_B = np.array([a.train_weight for a in events_B])
    Y_A = np.array([a.classification for a in events_A])
    Y_B = np.array([a.classification for a in events_B])

    # Index our X training sets by row; convert to ndarrays.
    X_A = df_A.as_matrix()
    X_B = df_B.as_matrix()

    # Scale for the NN.
    X_A = scale(X_A)
    X_B = scale(X_B)

    # Fit model.
    model_A.fit(X_A, Y_A, sample_weight=w_A, validation_data=(X_B, Y_B, w_B),
                nb_epoch=1000, batch_size=32, callbacks=[EarlyStopping(patience=50)])
    model_A.save(datetime.now().strftime('%d%m%y_%H%S') + '_kerasmodel.h5')


    # Get scores of X_A for BDT_B and vice-versa.
    prob_tuples = model_A.predict_proba(X_B).tolist()
    # Only want the second element of the prob tuple (prob of signal).
    scores = [a[0] for a in prob_tuples]

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

    # Call TrafoD on Event list.
    bins, delta_bins_s, delta_bins_b = trafoD_with_error(events)

    # Calculate sensitivity.
    sens, error = calc_sensitivity_with_error(events, bins, delta_bins_s, delta_bins_b)

    return sens, error


def decision_plot(events, show=False, block=False):

    # Call TrafoD on Event list.
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

    plt.show(block=block)
