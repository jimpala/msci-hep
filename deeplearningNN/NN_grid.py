from keras.models import Sequential
from keras.layers import Dense
from keras.callbacks import EarlyStopping
from sklearn.preprocessing import scale

import numpy as np
import pandas as pd
import json
import sys


scale_factor_map = {
    2: {
        'Zl': 1.0,
        'Zcl': 1.41,
        'Zcc': 1.15,
        'Zbl': 1.15,
        'Zbc': 1.15,
        'Zbb': 1.15,
        'Wl': 1.00,
        'Wcl': 1.22,
        'Wcc': 1.65,
        'Wbl': 1.65,
        'Wbc': 1.65,
        'Wbb': 1.65,
        'stopWt': 0.92,
        'stopt': 0.92,
        'stops': 0.92,
        'ttbar': 0.80,
        'multijet': 0.63,
        'WW': 1.13,
        'ZZ': 1.13,
        'WZ': 1.13,
        'qqZvvH125': 1.0,
        'qqWlvH125': 1.0
    }, 3: {
        'Zl': 1.0,
        'Zcl': 1.0,
        'Zcc': 1.1,
        'Zbl': 1.1,
        'Zbc': 1.1,
        'Zbb': 1.1,
        'Wl': 1.00,
        'Wcl': 1.27,
        'Wcc': 1.59,
        'Wbl': 1.59,
        'Wbc': 1.59,
        'Wbb': 1.59,
        'stopWt': 0.99,
        'stopt': 0.99,
        'stops': 0.99,
        'ttbar': 0.87,
        'multijet': 0.6,
        'WW': 1.12,
        'ZZ': 1.12,
        'WZ': 1.12,
        'qqZvvH125': 1.0,
        'qqWlvH125': 1.0
    }
}


def df_process(df, njets, train=False, test=False):
    # train and test can't both be true.
    assert not (train and test)

    # Reset index of df.
    df.reset_index(drop=True)

    # Configure weights for train mode (renormalised).
    if train:
        sum_sig = df[df.Class == 1]['EventWeight'].sum()
        sum_back = df[df.Class == 0]['EventWeight'].sum()
        size_sig = df[df.Class == 1]['EventWeight'].size
        size_back = df[df.Class == 0]['EventWeight'].size

        scale_sig = size_sig / sum_sig
        scale_back = size_back / sum_back

        sig_mask = (df['Class'] == 1)
        sig_entries = df[sig_mask]
        df.loc[sig_mask, 'EventWeight'] = sig_entries['EventWeight'].apply(lambda a: a * scale_sig)

        back_mask = (df['Class'] == 0)
        back_entries = df[back_mask]
        df.loc[back_mask, 'EventWeight'] = back_entries['EventWeight'].apply(lambda a: a * scale_back)


    # Configure weights test mode (post-fit).
    elif test:
        weights = df.loc[:, 'EventWeight']
        indices = df.index.values
        samples = df.ix[:, 'sample'].as_matrix().tolist()
        samples = map(lambda a, b: scale_factor_map[njets][a] * b, samples, weights)
        df['EventWeight'] = pd.Series(data=samples, index=indices)

    # Get the weights.
    w = df['EventWeight'].as_matrix()
    w = w.flatten()
    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'nJ', 'nBJ'], axis=1)

    # Get the classes.
    y = df['Class'].as_matrix().astype(int)
    y = np.reshape(y, (-1, 1))
    df = df.drop(['Class'], axis=1)

    # Get the feature vectors.
    x = df.as_matrix()

    return x, y, w


def main():

    try:
        if sys.argv[1] == 'gpu':
            # Process the data.
            df_2jet_even = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_even = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_odd = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_odd = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

        else:
            # Process the data.
            df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
            df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
            df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
            df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
            print "CSV read-in complete."

    except IndexError:
        print "No command line args passed. Running in local mode."

        # Process the data.
        df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
        df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
        df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
        df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
        print "CSV read-in complete."

    # Concatenate odd and even into the same DF.
    df_2jet = pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True)

    # Process df data into np feature arrays. Scale features.
    X_A, Y_A, w_A = df_process(df_2jet, 2, train=True)
    X_A = scale(X_A)

    # SET RANGE OF HIDDEN NODES.
    hidden_layers = range(2, 65)

    for nodes in hidden_layers:

        # Define Keras NN.
        model = Sequential()
        model.add(Dense(nodes, init='uniform', activation='relu', input_dim=11))
        model.add(Dense(1, init='uniform', activation='sigmoid'))

        # Compile.
        model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy', ])

        # Fit.
        print "Fitting..."
        hist = model.fit(X_A, Y_A, nb_epoch=1000, batch_size=32,
                            validation_split=0.25, callbacks=[EarlyStopping(patience=25)])
        print "Fit completed."

        # Dump results to JSON.
        filename = 'basicMLP_{:d}nodes.json'.format(nodes)
        json.dump({'params': hist.params, 'results': hist.history, 'best_val_acc': max(hist.history['val_acc'])},
                  open(filename, 'w'))
        print "Results dumped to {}.".format(filename)

    print "Script completed."


if __name__ == '__main__':
    main()