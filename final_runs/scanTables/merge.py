#!/usr/local/bin/python

import pandas as pd
from glob import glob

fps_sens = glob('*sens*.csv')
fps_err = glob('*err*.csv')

for fp_sens, fp_err in zip(fps_sens, fps_err):
    df_sens = pd.read_csv(fp_sens, index_col=0).astype(str)
    df_err = pd.read_csv(fp_err, index_col=0).astype(str)

    df_comp = pd.DataFrame(df_sens.values+'+'+df_err.values, columns=df_sens.columns, index=df_sens.index)

    df_comp.to_csv(fp_sens+fp_err)