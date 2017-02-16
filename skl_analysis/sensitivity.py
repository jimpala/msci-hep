import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


data = np.loadtxt('VHbb_data_truncated.csv', delimiter=',')


def trafoD(signal_dict, background_dict, initial_bins=100):
    """
    Routine to calculate perform TrafoD transform on S/B data.
    Make sure it's PREWEIGHTED.
    """

    # Get signal type name strings.
    signal_types = signal_dict.keys()
    background_types = background_dict.keys()

    # Plot range should be between -1 and 1.
    plot_range = (-1, 1)

    # Get baseline hist counts and bins.
    signal_hists, signal_bins = plt.hist(signal_dict.values(),
                                        bins=initial_bins,
                                        range=plot_range)[:2]

    background_hists, background_bins = plt.hist(background_dict.values(),
                                                bins=initial_bins,
                                                range=plot_range)[:2]


    signal_hists = hist_count_separate(signal_hists)
    background_hists = hist_count_separate(background_hists)

    zs = 10
    zb = 10

    while True:
        


def hist_count_separate(cum_array_of_hists):

    array_of_hists = []
    while len(array_of_hists) > 1:
        this_hist = array_of_hists[-1] - array_of_hists[-2]
        array_of_hists.append(this_hist)
        array_of_hists.pop()

    return array_of_hists[::-1] # Reverse the array of hists.