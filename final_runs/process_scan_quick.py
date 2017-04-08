import re
import fnmatch
import os
import pandas as pd
import numpy as np


def main():
    filename = 'sensitivities_fullscan.txt'
    f = open(filename, 'r')

    sens2_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err2_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens3_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err3_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    err_df_2 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                          columns=np.linspace(100, 500, 21).astype(int))

    sens2_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err2_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens3_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err3_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    err_df_3 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                          columns=np.linspace(100, 500, 21).astype(int))

    sens2_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err2_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens3_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err3_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    err_df_4 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                          columns=np.linspace(100, 500, 21).astype(int))

    sens2_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err2_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens3_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                            columns=np.linspace(100, 500, 21).astype(int))
    err3_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    sens_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                           columns=np.linspace(100, 500, 21).astype(int))
    err_df_5 = pd.DataFrame(index=np.linspace(0.01, 0.1, 10),
                          columns=np.linspace(100, 500, 21).astype(int))

    for size in np.linspace(0.01, 0.1, 10):

        # 2 DEPTH
        sens2_2 = list()
        err2_2 = list()
        sens3_2 = list()
        err3_2 = list()
        sens_2 = list()
        err_2 = list()

        for _ in range(21):

            f.readline()
            f.readline()
            f.readline()
            sens2_line = f.readline()
            sens2 = re.findall('\d+\.\d+', sens2_line)
            sens2_2.append(float(sens2.pop(0)))
            err2_2.append(float(sens2.pop(0)))
            sens3_line = f.readline()
            sens3 = re.findall('\d+\.\d+', sens3_line)
            sens3_2.append(float(sens3.pop(0)))
            err3_2.append(float(sens3.pop(0)))
            sens_line = f.readline()
            sens = re.findall('\d+\.\d+', sens_line)
            sens_2.append(float(sens.pop(0)))
            err_2.append(float(sens.pop(0)))
            f.readline()

        sens2_df_2.ix[size,:] = sens2_2
        err2_df_2.ix[size,:] = err2_2
        sens3_df_2.ix[size,:] = sens3_2
        err3_df_2.ix[size,:] = err3_2
        sens_df_2.ix[size,:] = sens_2
        err_df_2.ix[size,:] = err_2

        # 3 DEPTH
        sens2_3 = list()
        err2_3 = list()
        sens3_3 = list()
        err3_3 = list()
        sens_3 = list()
        err_3 = list()

        for _ in range(21):

            f.readline()
            f.readline()
            f.readline()
            sens2_line = f.readline()
            sens2 = re.findall('\d+\.\d+', sens2_line)
            sens2_3.append(float(sens2.pop(0)))
            err2_3.append(float(sens2.pop(0)))
            sens3_line = f.readline()
            sens3 = re.findall('\d+\.\d+', sens3_line)
            sens3_3.append(float(sens3.pop(0)))
            err3_3.append(float(sens3.pop(0)))
            sens_line = f.readline()
            sens = re.findall('\d+\.\d+', sens_line)
            sens_3.append(float(sens.pop(0)))
            err_3.append(float(sens.pop(0)))
            f.readline()

        sens2_df_3.ix[size,:] = sens2_3
        err2_df_3.ix[size,:] = err2_3
        sens3_df_3.ix[size,:] = sens3_3
        err3_df_3.ix[size,:] = err3_3
        sens_df_3.ix[size,:] = sens_3
        err_df_3.ix[size,:] = err_3


        # 4 DEPTH
        sens2_4 = list()
        err2_4 = list()
        sens3_4 = list()
        err3_4 = list()
        sens_4 = list()
        err_4 = list()

        for _ in range(21):

            f.readline()
            f.readline()
            f.readline()
            sens2_line = f.readline()
            sens2 = re.findall('\d+\.\d+', sens2_line)
            sens2_4.append(float(sens2.pop(0)))
            err2_4.append(float(sens2.pop(0)))
            sens3_line = f.readline()
            sens3 = re.findall('\d+\.\d+', sens3_line)
            sens3_4.append(float(sens3.pop(0)))
            err3_4.append(float(sens3.pop(0)))
            sens_line = f.readline()
            sens = re.findall('\d+\.\d+', sens_line)
            sens_4.append(float(sens.pop(0)))
            err_4.append(float(sens.pop(0)))
            f.readline()

        sens2_df_4.ix[size,:] = sens2_4
        err2_df_4.ix[size,:] = err2_4
        sens3_df_4.ix[size,:] = sens3_4
        err3_df_4.ix[size,:] = err3_4
        sens_df_4.ix[size,:] = sens_4
        err_df_4.ix[size,:] = err_4


        # 5 DEPTH
        sens2_5 = list()
        err2_5 = list()
        sens3_5 = list()
        err3_5 = list()
        sens_5 = list()
        err_5 = list()

        for _ in range(21):

            f.readline()
            f.readline()
            f.readline()
            sens2_line = f.readline()
            sens2 = re.findall('\d+\.\d+', sens2_line)
            sens2_5.append(float(sens2.pop(0)))
            err2_5.append(float(sens2.pop(0)))
            sens3_line = f.readline()
            sens3 = re.findall('\d+\.\d+', sens3_line)
            sens3_5.append(float(sens3.pop(0)))
            err3_5.append(float(sens3.pop(0)))
            sens_line = f.readline()
            sens = re.findall('\d+\.\d+', sens_line)
            sens_5.append(float(sens.pop(0)))
            err_5.append(float(sens.pop(0)))
            f.readline()

        sens2_df_5.ix[size,:] = sens2_5
        err2_df_5.ix[size,:] = err2_5
        sens3_df_5.ix[size,:] = sens3_5
        err3_df_5.ix[size,:] = err3_5
        sens_df_5.ix[size,:] = sens_5
        err_df_5.ix[size,:] = err_5


    sens2_df_2.to_csv('sens2_df_2.csv')
    err2_df_2.to_csv('err2_df_2.csv')
    sens3_df_2.to_csv('sens3_df_2.csv')
    err3_df_2.to_csv('err3_df_2.csv')
    sens_df_2.to_csv('sens_df_2.csv')
    err_df_2.to_csv('err_df_2.csv')

    sens2_df_3.to_csv('sens2_df_3.csv')
    err2_df_3.to_csv('err2_df_3.csv')
    sens3_df_3.to_csv('sens3_df_3.csv')
    err3_df_3.to_csv('err3_df_3.csv')
    sens_df_3.to_csv('sens_df_3.csv')
    err_df_3.to_csv('err_df_3.csv')

    sens2_df_4.to_csv('sens2_df_4.csv')
    err2_df_4.to_csv('err2_df_4.csv')
    sens3_df_4.to_csv('sens3_df_4.csv')
    err3_df_4.to_csv('err3_df_4.csv')
    sens_df_4.to_csv('sens_df_4.csv')
    err_df_4.to_csv('err_df_4.csv')

    sens2_df_5.to_csv('sens2_df_5.csv')
    err2_df_5.to_csv('err2_df_5.csv')
    sens3_df_5.to_csv('sens3_df_5.csv')
    err3_df_5.to_csv('err3_df_5.csv')
    sens_df_5.to_csv('sens_df_5.csv')
    err_df_5.to_csv('err_df_5.csv')

    f.close()



if __name__ == '__main__':
    main()
