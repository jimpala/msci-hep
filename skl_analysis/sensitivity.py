import numpy as np
from event_obj import *


# NOTE:
# This routine does not yet incorporate event weights for each of BDT values involved.
#
# Supposedly the solution would be to add two more arguments of one-to-one S&B
# weight dicts.

def trafoD(signal_dict, background_dict, initial_bins=200):
    """
    Routine to calculate perform TrafoD transform on S/B data.
    Returns dicts of new signal ang background freqs, with associated bin edges.
    """

    # Get signal type name strings.
    signal_types = signal_dict.keys()
    background_types = background_dict.keys()

    # Data for initial plot.
    signal_data = signal_dict.values()
    background_data = background_dict.values()


    # Work out the bin edges
    bin_edges = np.linspace(1,2,initial_bins).tolist()

    # Get array of bin lower edges.
    bin_width = bin_edges[1] - bin_edges[0]
    bin_mins = bin_edges[:-1]

    # Get total S/B events
    N_s = len(signal_data[0])
    N_b = reduce(lambda a, b: len(a) + len(b), background_data)

    # Create dict structure for new signal freqs.
    # Essentially a malloc procedure.
    trafo_signal_freqs = dict(zip(signal_types, [[0] * 100 for _ in range(len(signal_types))]))
    trafo_background_freqs = dict(zip(background_types, [[0] * 100 for _ in range(len(background_types))]))

    zs = 10 # Zs constant
    zb = 10 # Zb constant
    z = 0 # Z value
    bin_i = 1 # Current binning (from the right, one as first).
    new_bin_edges = [1] # Array of new bin centres.

    for bin_min in bin_mins[::-1]: # Iterate through bin lower edges from right to left.

        # Re-initialise signal and background counts for this bin.
        sig_bin = 0
        back_bin = 0

        # Get frequency of S&B in the current bin.
        for s, data in zip(signal_types, signal_data):
            this_count = len([a for a in data if a >= bin_min and a < bin_min + bin_width])
            trafo_signal_freqs[s][-bin_i] += this_count
            sig_bin += this_count
        for b, data in zip(background_types, background_data):
            this_count = len([a for a in data if a >= bin_min and a < bin_min + bin_width])
            trafo_background_freqs[b][-bin_i] += this_count
            back_bin += this_count

        # Update z for the current bin.
        z += zs * sig_bin / N_s + zb * back_bin / N_b
        print z

        # If z is now more than one, or we've reached the final bin edge,
        # update the new bin edges.
        if z > 1 or bin_min <= -1:
            new_bin_edges.insert(0,bin_min) # Insert new bin edge at front of list.

            # Reset x and increase the bin index.
            z = 0
            bin_i +=1

    # Filter out the excess zeroes allocated to trafo_freq S/B arrays.
    for sig in signal_types:
        trafo_signal_freqs[sig] = [a for a in trafo_signal_freqs[sig] if a != 0]
    for back in background_types:
        trafo_background_freqs[back] = [a for a in trafo_background_freqs[back] if a != 0]

    return trafo_signal_freqs, trafo_background_freqs, new_bin_edges


def trafoD_new(event_list, initial_bins=200, z_s=10, z_b=10):
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
        try:
            sig_bin = sum([a.is_in_bin(p, p+interval) for a in signal_events])
        except TypeError:
            sig_bin = 0
        try:
            back_bin = sum([a.is_in_bin(p, p+interval) for a in back_events])
        except TypeError:
            back_bin = 0

        # Update z for the current bin.
        z += z_s * sig_bin / N_s + z_b * back_bin / N_b

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

    print trafoD_new(test_events)

if __name__ == '__main__':
    main()