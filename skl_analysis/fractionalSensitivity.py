#!/bin/python
"""
An executable main script to use the trainSwapFold methods to perform a standard
BDT analysis on the k1 and k2 2-jet DFs.
"""
# Authors: James Pyne
# License: MIT
import matplotlib as mpl
mpl.use('Agg')

from trainSwapFold import *
import pandas as pd
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
import sys
import random
import json

random.seed(42)



def main():
    try:
        if sys.argv[1] == 'gpu':
            # Process the data.
            df_2jet_k1_orig = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_k1_orig = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_k2_orig = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_k2_orig = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

        else:
            # Process the data.
            df_2jet_k1_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_k1_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_k2_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_k2_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

    except IndexError:
        print "No command line args passed. Running in local mode."

        # Process the data.
        df_2jet_k1_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
        df_3jet_k1_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
        df_2jet_k2_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
        df_3jet_k2_orig = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
        print "CSV read-in complete."

    # Initialise JSON results.
    json_out = dict()
    json_out['nsamples_2jet'] = len(df_2jet_k1_orig) + len(df_2jet_k2_orig)
    json_out['nsamples_3jet'] = len(df_3jet_k1_orig) + len(df_3jet_k2_orig)
    sample_fracs = np.linspace(0.1, 1, 19)
    json_out['sample_fracs'] = sample_fracs.tolist()

    for variant in ['signal', 'background']:
        sensitivities = list()
        errors = list()
        
        for fraction in sample_fracs:
        
            print "Fractioning dfs"
            
            # Split into signal and background dfs.
            df_2jet_k1_sig = df_2jet_k1_orig[df_2jet_k1_orig.Class == 1]
            df_2jet_k2_sig = df_2jet_k2_orig[df_2jet_k2_orig.Class == 1]
            df_2jet_k1_back = df_2jet_k1_orig[df_2jet_k1_orig.Class == 0]
            df_2jet_k2_back = df_2jet_k2_orig[df_2jet_k2_orig.Class == 0]
            
            # Fraction S or B.
            if variant == 'signal':
                df_2jet_k1_sig = df_2jet_k1_sig.sample(frac=fraction)
                df_2jet_k2_sig = df_2jet_k2_sig.sample(frac=fraction)
            elif variant == 'background':
                df_2jet_k1_back = df_2jet_k1_back.sample(frac=fraction)
                df_2jet_k2_back = df_2jet_k2_back.sample(frac=fraction)
            
            # Concatenate them back together.
            df_2jet_k1_frac = pd.concat((df_2jet_k1_sig, df_2jet_k1_back), axis=0, ignore_index=True)
            df_2jet_k2_frac = pd.concat((df_2jet_k2_sig, df_2jet_k2_back), axis=0, ignore_index=True)

            print "Beginning 2 jet analysis."
            # Reset indices just to make sure that nothing untoward happens.
            df_2jet_k1_frac = df_2jet_k1_frac.reset_index(drop=True)
            df_2jet_k2_frac = df_2jet_k2_frac.reset_index(drop=True)
            df_2jet_k1 = df_2jet_k1_orig.reset_index(drop=True)
            df_2jet_k2 = df_2jet_k2_orig.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_2jet_k1, 2)
            events_k1_frac = populate_events(df_2jet_k1_frac, 2)
            events_k2 = populate_events(df_2jet_k2, 2)
            events_k2_frac = populate_events(df_2jet_k2_frac, 2)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k1_frac = renormalise_train_weights(events_k1_frac)
            events_k2 = renormalise_train_weights(events_k2)
            events_k2_frac = renormalise_train_weights(events_k2_frac)
            print "Event list ready."
        
            # Ready the DFs for training and scoring.
            df_2jet_k1 = ready_df_for_training(df_2jet_k1, events_k1)
            df_2jet_k1_frac = ready_df_for_training(df_2jet_k1_frac, events_k1_frac)
            df_2jet_k2 = ready_df_for_training(df_2jet_k2, events_k2)
            df_2jet_k2_frac = ready_df_for_training(df_2jet_k2_frac, events_k2_frac)

            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )
        
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )
        
            # Fit and train the events using the events lists, BDTs and DFs.
            # Notice the ordering!
            print "Training and decision scoring..."
            events_k2 = fold_score(events_k1_frac, events_k2, bdt_k1, df_2jet_k1_frac, df_2jet_k2)
            events_k1 = fold_score(events_k2_frac, events_k1, bdt_k2, df_2jet_k2_frac, df_2jet_k1)
            events = events_k1 + events_k2
            print "Done!"
        
            # Normalise the scores before TrafoD.
            events = normalise_scores(events)
            print "Decision scores normalised."
        
            # TrafoD and score sensitivity.
            print "TrafoD and sensitivity scoring..."
            sens_2jet, error_2jet = trafo_sensitivity(events)
            print "Done!"
        
            print "2 jet sensitivity is: {:f}".format(sens_2jet)
            print "2 jet error is: {:f}".format(error_2jet)

            sensitivities.append(sens_2jet)
            errors.append(error_2jet)

        # Append sensitivities/errors to JSON.
        json_out['sens_2jet_{}_varied'.format(variant)] = sensitivities
        json_out['err_2jet_{}_varied'.format(variant)] = errors

    # Dump JSON.
    with open('fractional_sensitivity.json', 'w') as fp:
        json.dump(json_out, fp)




    # --------------------------------------------------------
    # --------------------------------------------------------


    for variant in ['signal', 'background']:
        sensitivities = list()
        errors = list()

        for fraction in sample_fracs:

            print "Fractioning dfs"

            # Split into signal and background dfs.
            df_3jet_k1_sig = df_3jet_k1_orig[df_3jet_k1_orig.Class == 1]
            df_3jet_k2_sig = df_3jet_k2_orig[df_3jet_k2_orig.Class == 1]
            df_3jet_k1_back = df_3jet_k1_orig[df_3jet_k1_orig.Class == 0]
            df_3jet_k2_back = df_3jet_k2_orig[df_3jet_k2_orig.Class == 0]

            # Fraction S or B.
            if variant == 'signal':
                df_3jet_k1_sig = df_3jet_k1_sig.sample(frac=fraction)
                df_3jet_k2_sig = df_3jet_k2_sig.sample(frac=fraction)
            elif variant == 'background':
                df_3jet_k1_back = df_3jet_k1_back.sample(frac=fraction)
                df_3jet_k2_back = df_3jet_k2_back.sample(frac=fraction)

            # Concatenate them back together.
            df_3jet_k1_frac = pd.concat((df_3jet_k1_sig, df_3jet_k1_back), axis=0, ignore_index=True)
            df_3jet_k2_frac = pd.concat((df_3jet_k2_sig, df_3jet_k2_back), axis=0, ignore_index=True)

            print "Beginning 3 jet analysis."
            # Reset indices just to make sure that nothing untoward happens.
            df_3jet_k1_frac = df_3jet_k1_frac.reset_index(drop=True)
            df_3jet_k2_frac = df_3jet_k2_frac.reset_index(drop=True)
            df_3jet_k1 = df_3jet_k1_orig.reset_index(drop=True)
            df_3jet_k2 = df_3jet_k2_orig.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_3jet_k1, 2)
            events_k1_frac = populate_events(df_3jet_k1_frac, 3)
            events_k2 = populate_events(df_3jet_k2, 2)
            events_k2_frac = populate_events(df_3jet_k2_frac, 3)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k1_frac = renormalise_train_weights(events_k1_frac)
            events_k2 = renormalise_train_weights(events_k2)
            events_k2_frac = renormalise_train_weights(events_k2_frac)
            print "Event list ready."

            # Ready the DFs for training and scoring.
            df_3jet_k1 = ready_df_for_training(df_3jet_k1, events_k1)
            df_3jet_k1_frac = ready_df_for_training(df_3jet_k1_frac, events_k1_frac)
            df_3jet_k2 = ready_df_for_training(df_3jet_k2, events_k2)
            df_3jet_k2_frac = ready_df_for_training(df_3jet_k2_frac, events_k2_frac)

            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )

            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=200
                                        )

            # Fit and train the events using the events lists, BDTs and DFs.
            # Notice the ordering!
            print "Training and decision scoring..."
            events_k2 = fold_score(events_k1_frac, events_k2, bdt_k1, df_3jet_k1_frac, df_3jet_k2)
            events_k1 = fold_score(events_k2_frac, events_k1, bdt_k2, df_3jet_k2_frac, df_3jet_k1)
            events = events_k1 + events_k2
            print "Done!"

            # Normalise the scores before TrafoD.
            events = normalise_scores(events)
            print "Decision scores normalised."

            # TrafoD and score sensitivity.
            print "TrafoD and sensitivity scoring..."
            sens_3jet, error_3jet = trafo_sensitivity(events)
            print "Done!"

            print "3 jet sensitivity is: {:f}".format(sens_3jet)
            print "3 jet error is: {:f}".format(error_3jet)

            sensitivities.append(sens_3jet)
            errors.append(error_3jet)

        # Append sensitivities/errors to JSON.
        json_out['sens_3jet_{}_varied'.format(variant)] = sensitivities
        json_out['err_3jet_{}_varied'.format(variant)] = errors

    # Dump JSON.
    with open('fractional_sensitivity.json', 'w') as fp:
        json.dump(json_out, fp)

    print "Results dumped to fractional_sensitivity.json."
    print "Script finished."


if __name__ == '__main__':
    main()
