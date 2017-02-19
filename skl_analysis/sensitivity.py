import numpy as np
import matplotlib.pyplot as plt
import math
from event_obj import *


def trafoD(event_list, initial_bins=200, z_s=10, z_b=10):
    """Output optimised histogram bin widths from a list of events"""

    # Check that all event decision values are populated
    for e in event_list:
        assert e.decision_value

    # Get S/B event lists.
    signal_events = [a for a in event_list if a.classification == 1]
    back_events = [a for a in event_list if a.classification == 0]

    # Get total signal and background.
    N_s = sum([a.scale_factor * a.event_weight for a in signal_events])
    N_b = sum([a.scale_factor * a.event_weight for a in back_events])

    # Set up scan parameters.
    scan_points = np.linspace(-1, 1, num=initial_bins).tolist()
    interval = scan_points[1] - scan_points[0]
    scan_points = scan_points[1:-1]
    scan_points = scan_points[::-1]

    # Intialise z and bin list.
    z = 0
    bins = [1.0]

    for p in scan_points:
        # Get S/B freqs for current bin in scan
        sig_bin = sum([a.is_in_bin(p, p+interval) for a in signal_events])
        back_bin = sum([a.is_in_bin(p, p+interval) for a in back_events])

        # Update z for the current bin.
        z += z_s * sig_bin / N_s + z_b * back_bin / N_b
        print z

        if z > 1:
            bins.insert(0, p)
            z = 0

    # Close up the bins.
    bins.insert(0, -1.0)

    return bins


def calc_sensitivity(events, bins):
    """Calculate sensitivity (note: turns matplotlib interactive off)."""

    # Turn off interactive MPL.
    plt.ioff()

    # Initialise sensitivity.
    sens = 0

    # Get S/B stuff to plot.
    events_sb = [[a.decision_value for a in events if a.classification == 1],
                 [a.decision_value for a in events if a.classification == 0]]

    weights_sb = [[a.event_weight * a.scale_factor for a in events if a.classification == 1],
                  [a.event_weight * a.scale_factor for a in events if a.classification == 0]]

    counts_sb = plt.hist(events_sb,
                         bins=bins,
                         weights=weights_sb)[0]

    # Calculate sensitivity iteratively bin-by-bin.
    # Initialise cumulative S/B counters.
    n_s = 0
    n_b = 0

    # Reverse the counts before calculating.
    for s, b in zip(counts_sb[0][::-1], counts_sb[1][::-1]):
        n_s += s
        n_b += b
        sens += 2 * ((n_s + n_b) * math.log(1 + n_s / n_b) - n_s)

    return math.sqrt(sens)


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