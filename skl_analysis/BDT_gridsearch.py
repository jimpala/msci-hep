from sensitivity import *

from sklearn.metrics import make_scorer

def sensitivity_score(y, y_pred, weights=None):
    """Gets sensitivity score metric from 1D numpy arras of y,
    predicted y, and post-fit weights."""

    # Weights must be populated.
    assert weights

    # Get all args into 1D lists.
    y = y.reshaope

    # Perform TrafoD on predictions to get optimal bins.
    bins = trafoD_tuples(y, y_pred, weights)

    # Calculate sensitivity with set bins.
    sens = calc_sensitivity_tuples(y, y_pred, weights, bins)

    return sens



def main():
    pass

if __name__ == '__main__':
    main()