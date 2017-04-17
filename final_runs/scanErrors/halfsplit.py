#!/usr/local/bin/python

import pandas as pd
from glob import glob

fps = glob('*.csv')

for fp in fps:
    df = pd.read_csv(fp, index_col=0)
    df_A = df.ix[:,:11]
    df_B = df.ix[:,11:]

    df_A = df_A.round(2)
    df_B = df_B.round(2)

    df_A.to_csv('A_' + fp)
    df_B.to_csv('B_' + fp)