from keras.models import Sequential
from keras.layers import Dense
import numpy as np
import pandas as pd
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
    assert not train and test

    # Reset index of df.
    df.reset_index(drop=True)

    # Configure weights for train mode (renormalised).
    if train:
        sum_sig = df[df.Class == 1]['EventWeight'].sum()
        sum_back = df[df.Class == 0]['EventWeight'].sum()
        size_sig = df[df.Class == 1]['EventWeight'].sum()
        size_back = df[df.Class == 0]['EventWeight'].sum()

        scale_sig = size_sig / sum_sig
        scale_back = size_back / sum_back

        df[df.Class == 1]['EventWeight'] = df[df.Class == 1]['EventWeight'].apply(lambda a: a * scale_sig)
        df[df.Class == 0]['EventWeight'] = df[df.Class == 0]['EventWeight'].apply(lambda a: a * scale_back)

    # Configure weights test mode (post-fit).
    elif test:
        weights = df['EventWeight']
        scalings = df['sample'].apply(lambda a: scale_factor_map[njets][a])
        df['EventWeight'] = weights * scalings

    # Get the weights.
    w = df['EventWeight'].as_matrix()
    w = np.reshape(w, (1,-1))
    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'nJ', 'nBJ'], axis=1)



    # Get the classes.
    y = df['Class'].as_matrix()
    df = df.drop(['Class'])

    # Get the feature vectors.
    x = df.as_matrix()

    return x, y, w




def main():

    # Process the data.
    df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    x_A, y_A, z_A


if __name__ == '__main__':
    main()