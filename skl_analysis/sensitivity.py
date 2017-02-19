import numpy as np
import matplotlib.pyplot as plt
import math
from event_obj import *


def trafoD(event_list, initial_bins=200, z_s=10, z_b=10):
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

    print "TrafoD: Beginning loop."

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
        sens += 2 * ((s + b) * math.log(1 + s / b) - s)

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