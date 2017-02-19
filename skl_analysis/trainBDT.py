import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random

from event_obj import *
from sensitivity import trafoD, calc_sensitivity
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier

# RANDOM STATE
random.seed(42)

# Read in 2 jet and 3 jet dataframes from csv.
df_2jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_2jet.csv', index_col=0)
df_3jet = pd.read_csv('/Volumes/THUMB/VHbb-data/CSV/VHbb_data_3jet.csv', index_col=0)
dfs = [df_2jet, df_3jet]
print "CSV read-in completed."

events = []
# Get those df attributes. Then drop.
for df, njets, i in zip(dfs, (2, 3), range(2)):
    df = df.reset_index(drop=True)  # Just to make sure.

    processes = df['sample'].as_matrix().tolist()
    indices = df.index.values.tolist()
    event_weights = df['EventWeight'].as_matrix().tolist()

    args_zipped = zip(processes, indices, event_weights)

    this_events = [Event(a, njets, b, c) for a, b, c in args_zipped]
    events += this_events

    df = df.drop(['sample', 'EventWeight', 'EventNumber', 'Class'], axis=1)  # Drop some cols.

    dfs[i] = df
print "Event list populated."

# REVERT TO 2 JET. Generalise later.
df = dfs[0]
events_sig = [a for a in events if a.n_jets == 2 and a.classification == 1]
events_back = [a for a in events if a.n_jets == 2 and a.classification == 0]

# Shuffle each into two packs.
random.shuffle(events_sig)
random.shuffle(events_back)

events_sigA = events_sig[:(len(events_sig) // 2)]
events_sigB = events_sig[(len(events_sig) // 2):]
events_backA = events_back[:(len(events_back) // 2)]
events_backB = events_back[(len(events_back) // 2):]

events_A = events_sigA + events_backA
events_B = events_sigB + events_backB

# Get indices, weights and classes for each of these splits.
# w and Y need to be numpy arrays to work with skl.
indices_A = [a.index for a in events_A]
indices_B = [a.index for a in events_B]
w_A = np.array([a.event_weight for a in events_A])
w_B = np.array([a.event_weight for a in events_B])
Y_A = np.array([a.classification for a in events_A])
Y_B = np.array([a.classification for a in events_B])

# Index our X training sets by row; convert to ndarrays.
X_A = df.ix[indices_A].as_matrix()
X_B = df.ix[indices_B].as_matrix()

print "Commencing BDT training..."

# Set up BDTs and fit
bdt_A = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_split=0.05),
                           learning_rate=0.15,
                           algorithm="SAMME",
                           n_estimators=200
                           )

bdt_B = AdaBoostClassifier(DecisionTreeClassifier(max_depth=3, min_samples_split=0.05),
                           learning_rate=0.15,
                           algorithm="SAMME",
                           n_estimators=200
                           )

bdt_A.fit(X_A, Y_A, sample_weight=w_A)
bdt_B.fit(X_B, Y_B, sample_weight=w_B)
print "BDT training completed."

# Get scores of X_A for BDT_B and vice-versa.
scores_A = bdt_B.decision_function(X_A).tolist()
scores_B = bdt_A.decision_function(X_B).tolist()
print "Non-normalised decision function scores processed."

# Normalise decision scores between -1 and 1.
max_score = max([a for a in scores_A + scores_B])
min_score = min([a for a in scores_A + scores_B])
score_range = max_score - min_score
score_midpoint = min_score + score_range / 2
# Translate and shrink.
scores_A = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.001), scores_A)  # 1e-8 added for bounding.
scores_B = map(lambda a: (a - score_midpoint) / (score_range / 2 + 0.001), scores_B)


print "Updating event objects with decision scores..."

# Process scores to Event objects.
for e, s in zip(events_A,scores_A):
    e.set_decision_value(s)
for e, s in zip(events_B,scores_B):
    e.set_decision_value(s)
events = events_A + events_B
print "Event objects updated."

# Call TrafoD on Event list.
print "Implementing TrafoD histogram bin transform."
bins = trafoD(events)

sens = calc_sensitivity(events, bins)
print "Sensitivity " + str(sens)

print "Plotting results..."

# Initialise plot stuff
plot_range = (-1, 1)
class_names = Event.processes
plot_data = []
plot_weights = []
plot_colors = []

# Get list of hists.
for c in class_names:
    plot_data.append([a.decision_value for a in events if a.process == c])
    plot_weights.append([a.event_weight * a.scale_factor for a in events if a.process == c])
    plot_colors.append(Event.process_color_map[c])

# Plot.
plt.hist(plot_data,
         bins=bins,
         weights=plot_weights,
         range=plot_range,
         rwidth=1,
         color=plot_colors,
         label=class_names,
         stacked=True,
         edgecolor='None')
x1, x2, y1, y2 = plt.axis()
plt.axis((x1, x2, y1, y2 * 1.2))
plt.legend(loc='upper right', ncol=2, prop={'size': 12})
plt.ylabel('Samples')
plt.xlabel('Score')
plt.title('Decision Scores')

plt.show()