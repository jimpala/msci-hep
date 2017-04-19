from keras.models import Sequential
from keras.layers import Dense
from keras.callbacks import EarlyStopping
from sklearn.preprocessing import scale

import numpy as np
import pandas as pd
import json
import sys
from datetime import datetime

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
        'qqWlvH125': 1.0,
        'qqZllH125': 1.0
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
        'qqWlvH125': 1.0,
        'qqZllH125': 1.0
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
        df.loc[sig_mask, 'EventWeight'] = sig_entries[
            'EventWeight'].apply(lambda a: a * scale_sig)

        back_mask = (df['Class'] == 0)
        back_entries = df[back_mask]
        df.loc[back_mask, 'EventWeight'] = back_entries[
            'EventWeight'].apply(lambda a: a * scale_back)

    # Configure weights test mode (post-fit).
    elif test:
        weights = df.loc[:, 'EventWeight']
        indices = df.index.values
        samples = df.ix[:, 'sample'].as_matrix().tolist()
        samples = map(lambda a, b: scale_factor_map[
                      njets][a] * b, samples, weights)
        df['EventWeight'] = pd.Series(data=samples, index=indices)

    # Get the weights.
    w = df['EventWeight'].as_matrix()
    w = w.flatten()
    df = df.drop(['sample', 'EventWeight',
                  'EventNumber', 'nJ', 'nTags'], axis=1)

    # Get the classes.
    y = df['Class'].as_matrix().astype(int)
    y = np.reshape(y, (-1, 1))
    df = df.drop(['Class'], axis=1)

    # Get the feature vectors.
    x = df.as_matrix()

    return x, y, w


def main():

    sys.argv = [5,6,7]

    try:
        assert len(sys.argv) > 1
    except AssertionError:
        raise

    hidden_layers = [int(a) for a in sys.argv[1:]]

    # 2 JET NN
    ##########
    model_2 = Sequential()
    first = True
    for l in hidden_layers:
        if first:
            model_2.add(Dense(l, init='uniform',
                        activation='relu', input_dim=11))
            first = False
            continue
        model_2.add(Dense(l, init='uniform', activation='relu'))
    model_2.add(Dense(1, init='uniform', activation='sigmoid'))
    model_2.compile(loss='binary_crossentropy',
                    optimizer='adam', metrics=['accuracy', ])

    # 3 JET NN
    ##########
    model_3 = Sequential()
    first = True
    for l in hidden_layers:
        if first:
            model_3.add(Dense(l, init='uniform',
                              activation='relu', input_dim=13))
            first = False
            continue
        model_3.add(Dense(l, init='uniform', activation='relu'))
    model_3.add(Dense(1, init='uniform', activation='sigmoid'))
    model_3.compile(loss='binary_crossentropy',
                    optimizer='adam', metrics=['accuracy', ])

    # Load in CSVs.
    df_2jet_even = pd.read_csv(
        '/home/jpyne/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv(
        '/home/jpyne/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv(
        '/home/jpyne/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv(
        '/home/jpyne/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    # Concatenate odd and even into the same DF.
    df_2jet = pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True)
    df_2jet = df_2jet.sample(frac=1, random_state=42)
    df_3jet = pd.concat([df_3jet_even, df_3jet_odd], axis=0, ignore_index=True)
    df_3jet = df_3jet.sample(frac=1, random_state=42)

    # Process df data into np feature arrays. Scale features.
    X_2jet, Y_2jet, w_2jet = df_process(df_2jet, 2, train=True)
    X_2jet = scale(X_2jet)
    X_3jet, Y_3jet, w_3jet = df_process(df_3jet, 3, train=True)
    X_3jet = scale(X_3jet)

    # Fit 2 jet.
    print "Fitting..."
    hist = model_2.fit(X_2jet, Y_2jet, nb_epoch=1000, batch_size=32,
                       validation_split=0.25, callbacks=[EarlyStopping(patience=15)])
    print "Fit completed."

    # Dump results to JSON and HDF5.
    now = datetime.now().strftime('%d%m%y_%H%S')
    filename = 'trainedNN_{}_2jet'.format(now)
    json.dump({'params': hist.params, 'results': hist.history, 'low_val_loss': min(hist.history['val_loss'])},
              open(filename + '.json', 'w'))
    print "Results dumped to {}.".format(filename)
    model_2.save(filename + '.h5')
    print "Model saved to {}".format(filename)

    # Fit 3 jet.
    print "Fitting..."
    hist = model_3.fit(X_3jet, Y_3jet, nb_epoch=1000, batch_size=32,
                       validation_split=0.25, callbacks=[EarlyStopping(patience=15)])
    print "Fit completed."

    # Dump results to JSON and HDF5.
    now = datetime.now().strftime('%d%m%y_%H%S')
    filename = 'trainedNN_{}_3jet'.format(now)
    json.dump({'params': hist.params, 'results': hist.history, 'low_val_loss': min(hist.history['val_loss'])},
              open(filename + '.json', 'w'))
    print "Results dumped to {}.".format(filename)
    model_3.save(filename + '.h5')
    print "Model saved to {}".format(filename)

    print "Script completed."

if __name__ == '__main__':
    main()
