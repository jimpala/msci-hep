import json
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from trainSwapFold import *


def main():
    # Read in 2 jet and 3 jet dataframes from csv.
    df_k1_2jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_k1_3jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_k2_2jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_k2_3jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)

    json_store = dict()

    # 2 JET VARIED SIGNAL
    ######################
    df = pd.concat([df_k1_2jet, df_k2_2jet], axis=0, ignore_index=True)  # Reset built-in.
    df_signal = df[df.Class == 1]
    df_background = df[df.Class == 0]
    json_store['nsamples_2jet'] = len(df)
    sample_fracs = np.linspace(0.1, 1, 19)
    json_store['sample_fracs'] = sample_fracs.tolist()

    df_gen = (pd.concat([df_background, df_signal.sample(frac=a)], axis=0) for a in sample_fracs)

    print "Beginning generator loop for 2 jet."

    try:
        sens_2jet = list()
        err_2jet = list()

        while True:
            print "Loading randomly sampled data..."
            # Generate the next df with a sample fraction and reset the index.
            this_df = df_gen.next()
            this_df = this_df.sample(frac=1)  # Need to mix up for lower/upper slicing to avoid zeros.
            this_df = this_df.reset_index(drop=True)

            # Segregate the upper and lower half for the swap fold.
            this_df_k1 = this_df.ix[:len(this_df) / 2]
            this_df_k2 = this_df.ix[len(this_df) / 2:]

            # Populate events lists and ready DFs for training.
            this_events_k1 = populate_events(this_df_k1, 2, train_weights=True)
            this_events_k2 = populate_events(this_df_k2, 2, train_weights=True)

            this_df_k1 = ready_df_for_training(this_df_k1, this_events_k1)
            this_df_k2 = ready_df_for_training(this_df_k2, this_events_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            # LOAD IN FULL SET FOR TESTING
            # Reset indices just to make sure that nothing untoward happens.
            df_k1 = df_k1_2jet.reset_index(drop=True)
            df_k2 = df_k2_2jet.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_k1, 2)
            events_k2 = populate_events(df_k2, 2)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k2 = renormalise_train_weights(events_k2)
            print "Event list ready."

            # Ready the DFs for training and scoring.
            df_k1 = ready_df_for_training(df_k1, events_k1)
            df_k2 = ready_df_for_training(df_k2, events_k2)

            # Get decision scores.
            print "Scoring folded events..."
            events_k2 = fold_score(this_events_k1, events_k2, bdt_k1, this_df_k1, df_k2)
            events_k1 = fold_score(this_events_k2, events_k1, bdt_k2, this_df_k2, df_k1)
            events = events_k1 + events_k2

            # Process sensitivity and add to the list.
            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_2jet.append(this_sens)
            err_2jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "2 jet iteration completed."

    except StopIteration:
        print "2 jet analysis fully completed."

    finally:
        json_store['sens_2jet_background_varied'] = sens_2jet
        json_store['err_2jet_background_varied'] = err_2jet

    # 3 JET VARIED SIGNAL
    #####################
    df = pd.concat([df_k1_3jet, df_k2_3jet], axis=0, ignore_index=True)
    df_signal = df[df.Class == 1]
    df_background = df[df.Class == 0]
    json_store['nsamples_3jet'] = len(df)
    sample_fracs = np.linspace(0.1, 1, 19)

    df_gen = (pd.concat([df_background, df_signal.sample(frac=a)], axis=0) for a in sample_fracs)

    print "Beginning generator loop for 3 jet."

    try:
        sens_3jet = list()
        err_3jet = list()

        while True:
            print "Loading randomly sampled data..."
            # Generate the next df with a sample fraction and reset the index.
            this_df = df_gen.next()
            this_df = this_df.sample(frac=1)  # Need to mix up for lower/upper slicing to avoid zeros.
            this_df = this_df.reset_index(drop=True)

            # Segregate the upper and lower half for the swap fold.
            this_df_k1 = this_df.ix[:len(this_df) / 2]
            this_df_k2 = this_df.ix[len(this_df) / 2:]

            # Populate events lists and ready DFs for training.
            this_events_k1 = populate_events(this_df_k1, 2, train_weights=True)
            this_events_k2 = populate_events(this_df_k2, 2, train_weights=True)

            this_df_k1 = ready_df_for_training(this_df_k1, this_events_k1)
            this_df_k2 = ready_df_for_training(this_df_k2, this_events_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            # LOAD IN FULL SET FOR TESTING
            # Reset indices just to make sure that nothing untoward happens.
            df_k1 = df_k1_3jet.reset_index(drop=True)
            df_k2 = df_k2_3jet.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_k1, 3)
            events_k2 = populate_events(df_k2, 3)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k2 = renormalise_train_weights(events_k2)
            print "Event list ready."

            # Ready the DFs for training and scoring.
            df_k1 = ready_df_for_training(df_k1, events_k1)
            df_k2 = ready_df_for_training(df_k2, events_k2)

            # Get decision scores.
            print "Scoring folded events..."
            events_k2 = fold_score(this_events_k1, events_k2, bdt_k1, this_df_k1, df_k2)
            events_k1 = fold_score(this_events_k2, events_k1, bdt_k2, this_df_k2, df_k1)
            events = events_k1 + events_k2

            # Process sensitivity and add to the list.
            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_3jet.append(this_sens)
            err_3jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "3 jet iteration completed."

    except StopIteration:
        print "3 jet analysis fully completed."

    finally:
        json_store['sens_3jet_background_varied'] = sens_3jet
        json_store['err_3jet'] = err_3jet

    # 2 JET VARIED BACKGROUND
    ##########################
    df = pd.concat([df_k1_2jet, df_k2_2jet], axis=0, ignore_index=True)  # Reset built-in.
    df_signal = df[df.Class == 1]
    df_background = df[df.Class == 0]
    sample_fracs = np.linspace(0.1, 1, 19)

    df_gen = (pd.concat([df_signal, df_background.sample(frac=a)], axis=0) for a in sample_fracs)

    print "Beginning generator loop for 2 jet."

    try:
        sens_2jet = list()
        err_2jet = list()

        while True:
            print "Loading randomly sampled data..."
            # Generate the next df with a sample fraction and reset the index.
            this_df = df_gen.next()
            this_df = this_df.sample(frac=1)  # Need to mix up for lower/upper slicing to avoid zeros.
            this_df = this_df.reset_index(drop=True)

            # Segregate the upper and lower half for the swap fold.
            this_df_k1 = this_df.ix[:len(this_df) / 2]
            this_df_k2 = this_df.ix[len(this_df) / 2:]

            # Populate events lists and ready DFs for training.
            this_events_k1 = populate_events(this_df_k1, 2, train_weights=True)
            this_events_k2 = populate_events(this_df_k2, 2, train_weights=True)

            this_df_k1 = ready_df_for_training(this_df_k1, this_events_k1)
            this_df_k2 = ready_df_for_training(this_df_k2, this_events_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            # LOAD IN FULL SET FOR TESTING
            # Reset indices just to make sure that nothing untoward happens.
            df_k1 = df_k1_2jet.reset_index(drop=True)
            df_k2 = df_k2_2jet.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_k1, 2)
            events_k2 = populate_events(df_k2, 2)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k2 = renormalise_train_weights(events_k2)
            print "Event list ready."

            # Ready the DFs for training and scoring.
            df_k1 = ready_df_for_training(df_k1, events_k1)
            df_k2 = ready_df_for_training(df_k2, events_k2)

            # Get decision scores.
            print "Scoring folded events..."
            events_k2 = fold_score(this_events_k1, events_k2, bdt_k1, this_df_k1, df_k2)
            events_k1 = fold_score(this_events_k2, events_k1, bdt_k2, this_df_k2, df_k1)
            events = events_k1 + events_k2

            # Process sensitivity and add to the list.
            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_2jet.append(this_sens)
            err_2jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "2 jet iteration completed."

    except StopIteration:
        print "2 jet analysis fully completed."

    finally:
        json_store['sens_2jet_signal_varied'] = sens_2jet
        json_store['err_2jet_signal_varied'] = err_2jet

    # 3 JET VARIED BACKGROUND
    ###########################
    df = pd.concat([df_k1_3jet, df_k2_3jet], axis=0, ignore_index=True)
    df_signal = df[df.Class == 1]
    df_background = df[df.Class == 0]
    sample_fracs = np.linspace(0.1, 1, 19)

    df_gen = (pd.concat([df_signal, df_background.sample(frac=a)], axis=0) for a in sample_fracs)

    print "Beginning generator loop for 3 jet."

    try:
        sens_3jet = list()
        err_3jet = list()

        while True:
            print "Loading randomly sampled data..."
            # Generate the next df with a sample fraction and reset the index.
            this_df = df_gen.next()
            this_df = this_df.sample(frac=1)  # Need to mix up for lower/upper slicing to avoid zeros.
            this_df = this_df.reset_index(drop=True)

            # Segregate the upper and lower half for the swap fold.
            this_df_k1 = this_df.ix[:len(this_df) / 2]
            this_df_k2 = this_df.ix[len(this_df) / 2:]

            # Populate events lists and ready DFs for training.
            this_events_k1 = populate_events(this_df_k1, 2, train_weights=True)
            this_events_k2 = populate_events(this_df_k2, 2, train_weights=True)

            this_df_k1 = ready_df_for_training(this_df_k1, this_events_k1)
            this_df_k2 = ready_df_for_training(this_df_k2, this_events_k2)

            print "Training first fold of BDT..."
            bdt_k1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            print "Training second fold of BDT..."
            bdt_k2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_leaf=0.01),
                                        learning_rate=0.15,
                                        algorithm="SAMME",
                                        n_estimators=310
                                        )

            # LOAD IN FULL SET FOR TESTING
            # Reset indices just to make sure that nothing untoward happens.
            df_k1 = df_k1_3jet.reset_index(drop=True)
            df_k2 = df_k2_3jet.reset_index(drop=True)

            # Put the DFs through populate_events to get lists of workable events.
            events_k1 = populate_events(df_k1, 3)
            events_k2 = populate_events(df_k2, 3)

            # Set the train weights for these events.
            events_k1 = renormalise_train_weights(events_k1)
            events_k2 = renormalise_train_weights(events_k2)
            print "Event list ready."

            # Ready the DFs for training and scoring.
            df_k1 = ready_df_for_training(df_k1, events_k1)
            df_k2 = ready_df_for_training(df_k2, events_k2)

            # Get decision scores.
            print "Scoring folded events..."
            events_k2 = fold_score(this_events_k1, events_k2, bdt_k1, this_df_k1, df_k2)
            events_k1 = fold_score(this_events_k2, events_k1, bdt_k2, this_df_k2, df_k1)
            events = events_k1 + events_k2

            # Process sensitivity and add to the list.
            print "Processing sensitivity and error..."
            events = normalise_scores(events)
            this_sens, this_error = trafo_sensitivity(events)
            sens_3jet.append(this_sens)
            err_3jet.append(this_error)
            print "Sens: {:f}".format(this_sens)
            print "3 jet iteration completed."

    except StopIteration:
        print "3 jet analysis fully completed."

    finally:
        json_store['sens_3jet_signal_varied'] = sens_3jet
        json_store['err_3jet_signal_varied'] = err_3jet

    # Dump JSON to file.
    with open('samplesVsSensitivity_SB.json', 'w') as fp:
        json.dump(json_store, fp)

    print "Dumped analysis data to samplesVsSensitivity_SB.json. Analysis complete."


if __name__ == "__main__":
    main()