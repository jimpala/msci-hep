import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from event_obj import *
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.model_selection import GridSearchCV
from sklearn.model_selection import train_test_split

# Read in 2 jet and 3 jet dataframes from csv.
df_2jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet.csv', index_col=0)
df_3jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet.csv', index_col=0)
dfs = [df_2jet, df_3jet]
print "CSV read-in completed."

events = []
# Get those df attributes. Then drop.
for df, njets, i in zip(dfs, (2,3), range(2)):
    processes = df['sample'].as_matrix().tolist()
    event_nos = df['EventNumber'].as_matrix().tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, event_nos, event_weights)

    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]
    events += this_events

    df = df.drop(['sample', 'EventWeight', 'Class'], axis=1) # Drop some cols.
    df = df.set_index('EventNumber') # Set dfs to index by EventNumber. It's easier.

    dfs[i] = df
print "Event list populated."

# REVERT TO 2 JET. Generalise later.
df = dfs[0]
events = [a for a in events if a.n_jets == 2]

# Get event numbers and classes for 2 jet.
event_nos = np.array([[a.event_no for a in events]]).transpose() # Make this a column vector.
classes = np.array([a.classification for a in events])

# Get splitting event numbers.
event_nos_1, event_nos_2, Y_1, Y_2 = train_test_split(event_nos,
                                                      classes,
                                                      test_size=0.5,
                                                      random_state=42)
# Reshape as a plain old list.
event_nos_1 = event_nos_1.transpose().tolist()[0]
event_nos_2 = event_nos_2.transpose().tolist()[0]
print "ScikitLearn train-test-split executed."

### Apply splitting to event object list.
# events_1 = [a for a in events if a.event_no in event_nos_1]
# events_1 = map(lambda a: a.splitting(1), events_1)
# events_2 = [a for a in events if a.event_no in event_nos_2]
# events_2 = map(lambda a: a.splitting(2), events_2)
# events = events_1 + events_2
# print "Splitting applied to events."

# Index our X training sets by row; convert to ndarrays.
X_1 = df.ix[event_nos_1].as_matrix()
X_2 = df.ix[event_nos_2].as_matrix()
# Get weights with a list comp.
w_1 = [a.event_weight for a in events if a.event_no in event_nos_1]
w_2 = [a.event_weight for a in events if a.event_no in event_nos_2]
print "Commencing BDT training..."

# Set up BDTs and fit
bdt_1 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_split=0.05),
                           learning_rate=0.15,
                           algorithm="SAMME",
                           n_estimators=200
                           )

bdt_2 = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_split=0.05),
                           learning_rate=0.15,
                           algorithm="SAMME",
                           n_estimators=200
                           )

bdt_1.fit(X_1, Y_1, sample_weight=w_1)
bdt_2.fit(X_2, Y_2, sample_weight=w_2)
print "BDT training completed."

# Get scores of X1 for BDT 2 and vice-versa.
scores_1 = bdt_2.decision_function(X_1)
scores_2 = bdt_1.decision_function(X_2)
print "Decision function scores processed. Saving to Event objects..."


