from keras.models import Sequential
from keras.layers.core import Dense, Activation
from keras.optimizers import SGD, Adam
from sklearn import preprocessing
import numpy as np
import pandas as pd

np.random.seed(100)


def main():

    # Example XOR.
    model = Sequential()
    model.add(Dense(2, input_dim=2))
    model.add(Activation('sigmoid'))
    model.add(Dense(1, input_dim=2))
    model.add(Activation('sigmoid'))
    X = np.array([[0,0],[0,1],[1,0],[1,1]], "float32")
    y = np.array([[0],[1],[1],[0]], "float32")
    sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True)
    model.compile(loss='binary_crossentropy', optimizer=sgd)
    model.fit(X, y, nb_epoch=500, batch_size=1)

    # Process the data.
    df_2jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_even.csv', index_col=0)
    df_3jet_even = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_even.csv', index_col=0)
    df_2jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet_odd.csv', index_col=0)
    df_3jet_odd = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet_odd.csv', index_col=0)
    print "CSV read-in complete."

    # Shuffle the DFs.
    df_2jet_even = df_2jet_even.sample(frac=1)
    df_2jet_odd = df_2jet_odd.sample(frac=1)
    df_2jet = pd.concat([df_2jet_even, df_2jet_odd], axis=0, ignore_index=True)
    df_2jet = df_2jet.drop(['sample', 'EventWeight', 'EventNumber', 'nJ', 'nBJ'], axis=1)

    # Get the classes.
    y = df_2jet['Class'].as_matrix().astype(int)
    y = np.reshape(y, (-1, 1))
    df = df_2jet.drop(['Class'], axis=1)

    # Get the feature vectors.
    X = df.as_matrix().astype('float32')
    X = preprocessing.scale(X)

    # Test NN.
    model = Sequential()
    model.add(Dense(10, input_dim=11))
    model.add(Activation('sigmoid'))
    model.add(Dense(1, input_dim=10))
    model.add(Activation('sigmoid'))
    sgd = SGD(lr=0.1, decay=1e-6, momentum=0.9, nesterov=True)
    adam = Adam()
    model.compile(loss='binary_crossentropy', optimizer=adam)
    model.fit(X, y, nb_epoch=500)




if __name__ == '__main__':
    main()