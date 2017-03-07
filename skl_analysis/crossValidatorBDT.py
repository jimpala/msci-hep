

from event_obj import *
from trainSwapFold import *
from sensitivity import trafoD_tuples, calc_sensitivity_tuples, normalise_decision_scores


def extract_data(df, njets):
    df = df.reset_index(drop=True)  # Just to make sure.

    # Get the df attributes. Then drop.
    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    # Number of jets here is ambiguous.
    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]

    # Set train weights.
    this_events = renormalise_train_weights(this_events)

    # Drop some cols.
    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class', 'nJ', 'nBJ'], axis=1)

    # Get indices, weights and classes for each of these splits.
    # w and Y need to be numpy arrays to work with skl.
    w = np.array([a.event_weight for a in this_events])
    post_w = np.array([a.post_fit_weight for a in this_events])
    train_w = np.array([a.train_weight for a in this_events])
    Y = np.array([a.classification for a in this_events])
    X = df.as_matrix()


    return X, Y, w, post_w, train_w


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