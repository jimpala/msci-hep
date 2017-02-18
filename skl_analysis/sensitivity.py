import numpy as np
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


def main():

    # Generate some random events.
    test_events = [Event('qqZvvH125',2,1,2,decision_value=0.8),
                   Event('qqZvvH125', 2, 1, 2, decision_value=0.8),
                   Event('qqZvvH125', 2, 1, 2, decision_value=0.8),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),
                   Event('Wbb', 2, 1, 2, decision_value=-0.5),]

    print trafoD(test_events)


def isclose(a, b, rel_tol=1e-09, abs_tol=0.0):
    return abs(a-b) <= max(rel_tol * max(abs(a), abs(b)), abs_tol)

if __name__ == '__main__':
    main()