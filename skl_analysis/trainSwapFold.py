import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import random
import time
import sys

from event_obj import *
from crossValidatorBDT import renormalise_weights
from sensitivity import trafoD, calc_sensitivity, trafoD_with_error, calc_sensitivity_with_error
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn import preprocessing
from root_numpy import array2root


def populate_events(df):
    pass


def swap_fold_and_score():
    pass


def trafo_sensitivity(error=True):
    pass


def decision_plot(event_list):
    pass
