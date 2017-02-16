import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('VHbb_data_truncated.csv')


def calculate_sensitivity(signal_counts, background_counts, signal_bins, background_bins):
    """Routine to calculate sensitivity from a given S/B histogram"""

    signal_counts.remove


