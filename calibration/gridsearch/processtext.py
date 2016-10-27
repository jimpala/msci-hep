import re
import fnmatch
import os
import pandas as pd

def main():
    directory = '/Volumes/HEPDRIVE/gridsearch-text/lowpt_2jet_text/'

    directory_files = [f for f in os.listdir(directory)
                       if fnmatch.fnmatch(f, "*.txt")]

    database = {
        'nTrees': [],
        'nCuts': [],
        'kIndex': [],
        'ROCint': [],
    }


    for this_file in directory_files:
        # Open file for reading.
        f = open(directory + this_file, 'r')

        file_info = re.findall('\d+', this_file)

        # Add entry to database.
        # Important to cast stuff here as we're dealing
        # with strings.
        database['nTrees'].append(int(file_info[0]))
        database['nCuts'].append(int(file_info[1]))
        database['kIndex'].append(int(file_info[2]))


        while True:
            # New line into buffer. Exit if empty.
            line = f.readline()
            if not line:
                break
            # line.strip('\n')

            if 'ROC-integ' in line:
                f.readline()  # Skip intermediate line.
                results_line = f.readline()
                decimals = re.findall('\d+\.\d+', results_line)
                database['ROCint'].append(float(decimals[3]))  # 4th decimal is ROC-integral

        # Close yon file.
        f.close()


    df = pd.DataFrame(database)

    grouped = df.groupby(['nCuts', 'nTrees']).mean()
    grouped_df = pd.DataFrame(grouped)

    grouped_df.reset_index(inplace=True)
    grouped_df.drop('kIndex', axis=1, inplace=True)

    grouped_df.to_csv(path_or_buf='lowpt_2jet.csv')

if __name__ == "__main__":
    main()
