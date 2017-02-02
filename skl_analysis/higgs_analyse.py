import numpy as np
#import matplotlib.pyplot as plt



from root_numpy import root2array, rec2array

from sklearn import model_selection
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.model_selection import train_test_split

branch_names = """lepton pT, lepton eta, lepton phi, missing energy magnitude,
missing energy phi, jet 1 pt, jet 1 eta, jet 1 phi, jet 1 b-tag, jet 2 pt,
jet 2 eta, jet 2 phi, jet 2 b-tag, jet 3 pt, jet 3 eta, jet 3 phi, jet 3 b-tag,
jet 4 pt, jet 4 eta, jet 4 phi, jet 4 b-tag, m_jj, m_jjj, m_lv, m_jlv, m_bb, m_wbb,
m_wwbb""".split(",")
branch_names = [c.strip() for c in branch_names]
branch_names = (b.replace(" ", "_") for b in branch_names)
branch_names = list(b.replace("-", "_") for b in branch_names)

signal = root2array("HIGGSsignal.root",
                    treename="tree",
                    branches=branch_names)
signal = rec2array(signal)

backgr = root2array("HIGGSbackground.root",
                    treename="tree",
                    branches=branch_names)
backgr = rec2array(backgr)

# for sklearn data is usually organised
# into one 2D array of shape (n_samples x n_features)
# containing all the data and one array of categories
# of length n_samples
X = np.concatenate((signal, backgr))
y = np.concatenate((np.ones(signal.shape[0]), np.zeros(backgr.shape[0])))

# set aside an evaluation sample
# We artificially limit ourselves
# to a small subset of the available
# samples. This speeds things up and
# makes it easier to illustrate things
# like over-fitting. In real life you
# would not do this.
X_dev,X_eval, y_dev,y_eval = train_test_split(X, y,
                                              train_size=20000,
                                              test_size=10000,
                                              random_state=42)

gbt = GradientBoostingClassifier(n_estimators=200,
                                 max_depth=1,
                                 subsample=0.5,
                                 max_features=0.5,
                                 learning_rate=0.02)

scores = model_selection.cross_val_score(gbt,
                                          X_dev, y_dev,
                                          scoring="roc_auc",
                                          n_jobs=6,
                                          cv=3)

print "Accuracy: %0.5f (+/- %0.5f)"%(scores.mean(), scores.std())