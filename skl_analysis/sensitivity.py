import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import json




def trafoD(signal_dict, background_dict, initial_bins=200):
    """
    Routine to calculate perform TrafoD transform on S/B data.
    Make sure it's PREWEIGHTED.
    """

    # Get signal type name strings.
    signal_types = signal_dict.keys()
    background_types = background_dict.keys()

    # Plot range should be between -1 and 1.
    plot_range = (-1, 1)

    # Data for initial plot.
    signal_data = signal_dict.values()
    background_data = background_dict.values()

    # Get baseline hist counts and bins.
    signal_hists, signal_bins = plt.hist(signal_data,
                            bins=initial_bins,
                            range=plot_range)[:2]

    # HACK! Make signal_hists into list of lists.
    signal_hists = [np.array(signal_hists)]

    background_hists, background_bins = plt.hist(background_data,
                                bins=initial_bins,
                                range=plot_range)[:2]

    # Get array of bin centres.
    bin_width = signal_bins[1] - signal_bins[0]
    bin_mins = signal_bins[:-1]

    #signal_hists = hist_count_separate([signal_hists])
    #background_hists = hist_count_separate(background_hists)

    # Get total S/B events
    N_s = sum(signal_hists[0])
    N_b = reduce(lambda a, b: np.sum(a) + np.sum(b), background_hists)
    
    signal_freqs = dict(zip(signal_types, signal_hists)) # Must be array of arrays!
    background_freqs = dict(zip(background_types, background_hists))

    # Create dict structure for new signal freqs.
    # Essentially malloc.
    new_signal_freqs = dict(zip(signal_types, [[0] * 100 for _ in range(len(signal_types))]))
    new_background_freqs = dict(zip(background_types, [[0] * 100 for _ in range(len(background_types))]))

    zs = 10
    zb = 10
    z = 0 # Initialise z.
    bin_i = 1 # Initialise new_bin.

    for bin_min in bin_mins[::-1]:
        sig_tot = 0
        back_tot = 0
        for s, hist in zip(signal_types, signal_hists):
            this_count = len([a for a in hist if a >= bin_min and a < bin_min + bin_width])
            new_signal_freqs[s][-bin_i] += this_count
            sig_tot += this_count

        for b, hist in zip(background_types, background_hists):
            this_count = len([a for a in hist if a >= bin_min and a < bin_min + bin_width])
            new_background_freqs[b][-bin_i] += this_count
            back_tot += this_count

        z += zs * sig_tot / N_s + zb * back_tot / N_b
        print z

        if z > 1:
            z = 0
            bin_i +=1

        


def hist_count_separate(cum_array_of_hists):

    hists_out = []
    while len(cum_array_of_hists) > 1:
        this_hist = map(lambda a, b: a - b, cum_array_of_hists[-1], cum_array_of_hists[-2])
        hists_out.append(this_hist)
        cum_array_of_hists.pop()

    hists_out.append(cum_array_of_hists.pop())

    return hists_out[::-1] # Reverse the array of hists.

def main():
    with open('signal_dict.json') as fp:
        test_signal = json.load(fp)

    with open('background_dict.json') as fp:
        test_background = json.load(fp)

    trafoD(test_signal,test_background)

if __name__ == '__main__':
    main()