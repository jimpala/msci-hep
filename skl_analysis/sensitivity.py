import numpy as np
import matplotlib.pyplot as plt
import math
from event_obj import *
from copy import copy


def trafoD(event_list, initial_bins=1000, z_s=10, z_b=10):
    """Output optimised histogram bin widths from a list of events"""

    # Check that all event decision values are populated
    for e in event_list:
        assert e.decision_value

    # Sort event list by BDT value, descending.
    event_list = sorted(event_list, key=lambda a: a.decision_value)

    # Get total signal and background.
    N_tot = sum([a.post_fit_weight for a in event_list])
    N_s = sum([a.post_fit_weight for a in event_list if a.classification == 1])
    N_b = N_tot - N_s

    # Set up scan parameters.
    scan_points = np.linspace(-1, 1, num=initial_bins).tolist()[1:-1]
    scan_points = scan_points[::-1]

    # Initialise z and bin list.
    z = 0
    bins = [1.0]

    try:
        # Iterate over bin low edges in scan.
        for p in scan_points:
            # Initialise freq count for this bin
            sig_bin = 0
            back_bin = 0

            # Current bin loop.
            # Remember, events are in descending DV order.
            while True:
                # End algo if no events left - update z and then IndexError.
                if not event_list:
                    z += z_s * sig_bin / N_s + z_b * back_bin / N_b
                    if z > 1:
                        bins.insert(0, p)
                    raise IndexError

                # Break if DV not in bin.
                if event_list[-1].decision_value < p:
                    break

                # Pop the event.
                this_event = event_list.pop()

                # Add freq to S/B count.
                if this_event.classification == 1:
                    sig_bin += this_event.post_fit_weight
                else:
                    back_bin += this_event.post_fit_weight

            # Update z for current bin.
            z += z_s * sig_bin / N_s + z_b * back_bin / N_b

            # Reset z and update bin
            if z > 1:
                bins.insert(0, p)
                z = 0

    except IndexError:
        print "TrafoD: All events processed."

    finally:
        bins.insert(0,-1.0)
        return bins


def trafoD_with_error(event_list, initial_bins=1000, z_s=10, z_b=10):
    """Output optimised histogram bin widths from a list of events"""

    # Check that all event decision values are populated
    for e in event_list:
        assert e.decision_value

    # Sort event list by BDT value, descending.
    event_list = sorted(event_list, key=lambda a: a.decision_value)

    # Get total signal and background.
    N_tot = sum([a.post_fit_weight for a in event_list])
    N_s = sum([a.post_fit_weight for a in event_list if a.classification == 1])
    N_b = N_tot - N_s

    # Set up scan parameters.
    scan_points = np.linspace(-1, 1, num=initial_bins).tolist()[1:-1]
    scan_points = scan_points[::-1]

    # Initialise z and bin list.
    z = 0
    bins = [1.0]
    sum_w2_s = 0
    sum_w2_b = 0
    delta_bins_s = list()
    delta_bins_b = list()

    try:
        # Iterate over bin low edges in scan.
        for p in scan_points:
            # Initialise freq count for this bin
            sig_bin = 0
            back_bin = 0

            # Current bin loop.
            # Remember, events are in descending DV order.
            while True:
                # End algo if no events left - update z and then IndexError.
                if not event_list:
                    z += z_s * sig_bin / N_s + z_b * back_bin / N_b
                    if z > 1:
                        bins.insert(0, p)
                        delta_bins_s.insert(0, sum_w2_s)
                        delta_bins_b.insert(0, sum_w2_b)
                    raise IndexError

                # Break if DV not in bin.
                if event_list[-1].decision_value < p:
                    break

                # Pop the event.
                this_event = event_list.pop()

                # Add freq to S/B count, and the square to sums of w2.
                if this_event.classification == 1:
                    sig_bin += this_event.post_fit_weight
                    sum_w2_s += this_event.post_fit_weight ** 2
                else:
                    back_bin += this_event.post_fit_weight
                    sum_w2_b += this_event.post_fit_weight ** 2

            # Update z for current bin.
            z += z_s * sig_bin / N_s + z_b * back_bin / N_b

            # Reset z and update bin
            if z > 1:
                bins.insert(0, p)
                z = 0

                # Update sum_w2 for this bin.
                delta_bins_s.insert(0, sum_w2_s)
                delta_bins_b.insert(0, sum_w2_b)
                sum_w2_s = 0
                sum_w2_b = 0

    except IndexError:
        print "TrafoD: All events processed."

    finally:
        bins.insert(0,-1.0)
        delta_bins_s.insert(0, sum_w2_s)
        delta_bins_b.insert(0, sum_w2_b)
        return bins, delta_bins_s, delta_bins_b


def trafoD_tuples(y, y_pred, w, initial_bins=200, z_s=10, z_b=10):
    """Output optimised histogram bin widths list of y, predicted y, and POSTFIT weights."""

    y = y.tolist()[0]
    y_pred = y_pred.tolist()
    w = w.tolist()

    # Zip them up for the list comp.
    y_data = zip(y, y_pred, w)

    # Sort by prediction value.
    y_data = sorted(y_data, key=lambda a: a[1])

    # Get total signal and background.
    N_tot = sum([a[2] for a in y_data])
    N_s = sum([a[2] for a in y_data if a[0] == 1])
    N_b = N_tot - N_s

    # Set up scan parameters.
    scan_points = np.linspace(-1, 1, num=initial_bins).tolist()[1:-1]
    scan_points = scan_points[::-1]

    # Initialise z and bin list.
    z = 0
    bins = [1.0]

    try:
        # Iterate over bin low edges in scan.
        for p in scan_points:
            # Initialise freq count for this bin
            sig_bin = 0
            back_bin = 0

            # Current bin loop.
            # Remember, events are in descending DV order.
            while True:
                # End algo if no events left - update z and then IndexError.
                if not y_data:
                    z += z_s * sig_bin / N_s + z_b * back_bin / N_b
                    if z > 1:
                        bins.insert(0, p)
                    raise IndexError

                # Break if DV not in bin.
                if y_data[-1][1] < p:
                    break

                # Pop the event.
                this_dv = y_data.pop()

                # Add freq to S/B count.
                if this_dv[0] == 1:
                    sig_bin += this_dv[2]
                else:
                    back_bin += this_dv[2]

            # Update z for current bin.
            z += z_s * sig_bin / N_s + z_b * back_bin / N_b

            # Reset z and update bin
            if z > 1:
                bins.insert(0, p)
                z = 0

    except IndexError:
        print "TrafoD: All events processed."

    finally:
        bins.insert(0,-1.0)
        return bins


def calc_sensitivity(events, bins):
    """Calculate sensitivity (note: turns matplotlib interactive off)."""

    # Turn off interactive MPL.
    plt.ioff()

    # Initialise sensitivity.
    sens_sq = 0

    # Get S/B stuff to plot.
    events_sb = [[a.decision_value for a in events if a.classification == 1],
                 [a.decision_value for a in events if a.classification == 0]]

    weights_sb = [[a.post_fit_weight for a in events if a.classification == 1],
                  [a.post_fit_weight for a in events if a.classification == 0]]

    counts_sb = plt.hist(events_sb,
                         bins=bins,
                         weights=weights_sb)[0]

    # Reverse the counts before calculating.
    # Zip up S counts with B counts per bin.
    for s, b in zip(counts_sb[0][::-1], counts_sb[1][::-1]):
        this_sens = 2 * ((s + b) * math.log(1 + s / b) - s)
        if not math.isnan(this_sens):
            sens_sq += this_sens

    sens = math.sqrt(sens_sq)

    return sens


def calc_sensitivity_with_error(events, bins, bin_sums_w2_s, bin_sums_w2_b):
    """Calculate sensitivity (note: turns matplotlib interactive off)."""

    # Turn off interactive MPL.
    plt.ioff()

    # Initialise sensitivity and error.
    sens_sq = 0
    error_sq = 0

    # Get S/B stuff to plot.
    events_sb = [[a.decision_value for a in events if a.classification == 1],
                 [a.decision_value for a in events if a.classification == 0]]

    weights_sb = [[a.post_fit_weight for a in events if a.classification == 1],
                  [a.post_fit_weight for a in events if a.classification == 0]]

    counts_sb = plt.hist(events_sb,
                         bins=bins,
                         weights=weights_sb)[0]

    # Reverse the counts before calculating.
    # Zip up S, B, DS and DB per bin.
    s_stack = counts_sb[0][::-1]
    b_stack = counts_sb[1][::-1]
    ds_stack = bin_sums_w2_s[::-1]
    db_stack = bin_sums_w2_b[::-1]

    for s, b, ds, db in zip(s_stack, b_stack, ds_stack, db_stack):
        this_sens = 2 * ((s + b) * math.log(1 + s / b) - s)
        this_dsens_ds = 2 * math.log(1 + s/b)
        this_dsens_db = 2 * (math.log(1 + s/b) - s/b)
        this_error = (this_dsens_ds * ds) ** 2 + (this_dsens_db * db) ** 2
        if not math.isnan(this_sens):
            sens_sq += this_sens
        if not math.isnan(this_error):
            error_sq += this_error

    # Sqrt operations and error equation balancing.
    sens = math.sqrt(sens_sq)
    error = math.sqrt(error_sq/sens_sq)

    return sens, error


def calc_sensitivity_tuples(y, y_pred, w, bins):
    """Calculate sensitivity (note: turns matplotlib interactive off)."""

    y = y.tolist()[0]
    y_pred = y_pred.tolist()
    w = w.tolist()

    # Zip them up for the list comp.
    y_data = zip(y, y_pred, w)

    # Turn off interactive MPL.
    plt.ioff()

    # Initialise sensitivity.
    sens_sq = 0

    # Get S/B stuff to plot.
    events_sb = [[a[1] for a in y_data if a[0] == 1], [a[1] for a in y_data if a[0] == 0]]

    weights_sb = [[a[2] for a in y_data if a[0] == 1], [a[2] for a in y_data if a[0] == 0]]

    counts_sb = plt.hist(events_sb,
                         bins=bins,
                         weights=weights_sb)[0]

    # Reverse the counts before calculating.
    # Zip up S counts with B counts per bin.
    for s, b in zip(counts_sb[0][::-1], counts_sb[1][::-1]):
        sens_sq += 2 * ((s + b) * math.log(1 + s / b) - s)

    sens = math.sqrt(sens_sq)

    return sens


def normalise_decision_scores(scores):

    scores = scores.tolist()

    # Normalise decision scores between -1 and 1.
    max_score = max([a for a in scores])
    min_score = min([a for a in scores])
    score_range = max_score - min_score
    score_midpoint = min_score + score_range / 2
    # Translate and shrink.
    norm_scores = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.000001), scores)  # .001 added for bounding

    return np.array(norm_scores)


def main():

    # Generate some random events.
    test_events = [Event('qqZvvH125',2,1,2,decision_value=0.8),
                   Event('qqZvvH125', 2, 1, 2, decision_value=0.8),
                   Event('qqZvvH125', 2, 1, 2, decision_value=0.8),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),]

    print trafoD(test_events)



if __name__ == '__main__':
    main()