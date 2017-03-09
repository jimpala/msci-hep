from keras.models import Sequential
from keras.layers import Dense
import pickle

X = pickle.load(open('correct_x.p', 'rb'))
Y = pickle.load(open('correct_y.p', 'rb'))

model = Sequential()
model.add(Dense(12, input_dim=11, init='uniform', activation='relu'))
model.add(Dense(8, init='uniform', activation='relu'))
model.add(Dense(1, init='uniform', activation='sigmoid'))

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

model.fit(X, Y, nb_epoch=150, batch_size=10)