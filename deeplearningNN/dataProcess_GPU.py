from keras.models import Sequential
from keras.layers import Dense
from keras.optimizers import Adam, Adadelta
from keras.utils import np_utils

import numpy as np
import pandas as pd
import json
# fix random seed for reproducibility
seed = 7
np.random.seed(seed)

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
        weights = df.loc[:,'EventWeight']
        indices = df.index.values
        samples = df.ix[:,'sample'].as_matrix().tolist()
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

    # Process the data.
    df_2jet_even = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/home/jpyne/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/home/jpyne/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    # Shuffle the DFs.
    df_2jet_even = df_2jet_even.sample(frac=1)
    df_2jet_odd = df_2jet_odd.sample(frac=1)

    X_A, Y_A, w_A = df_process(df_2jet_even, 2, train=True)
    X_B, Y_B, w_B = df_process(df_2jet_odd, 2, test=True)
    Y_A = np_utils.to_categorical(Y_A,2)
    Y_B = np_utils.to_categorical(Y_B,2)
    validation = (X_B, Y_B, w_B)

    # NN model
    model = Sequential()
    model.add(Dense(300, input_dim=11, init='uniform', activation='relu'))
    model.add(Dense(300, init='uniform', activation='relu'))
    model.add(Dense(2, init='uniform', activation='softmax'))

    opt = Adadelta()
    model.compile(loss='binary_crossentropy', optimizer=opt, metrics=['accuracy'])

    # Fit the model
    hist = model.fit(X_A, Y_A, nb_epoch=50, batch_size=32, sample_weight=w_A,
                     validation_data=validation)

    # Get decision scores.
    Ystar_B = model.predict(X_B)
    Ystar_B = np.reshape(Ystar_B, (1, -1))[0].tolist()
    Y_B = np.reshape(Y_B, (1, -1))[0].tolist()

    json.dump({'Ystar': Ystar_B, 'Y': Y_B}, open('keras_test_out.json', 'w'))
    json.dump(hist.history, open('keras_history.json', 'w'))

    # Ystar_B = [int(round(a)) for a in Ystar_B]
    #
    # scores = [1 if a == b else 0 for a, b in zip(Y_B, Ystar_B)]
    # truth_rate = scores.count(1) / len(scores)

    print "Script completed."


if __name__ == '__main__':
    main()