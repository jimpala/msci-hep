import re
import fnmatch
import os
import pandas as pd
import numpy as np

def main():
    filename = 'sensitivities_fullscan.txt'
    f = open(filename, 'r')

    entries = list()

    while True:
        # Read new line into buffer.
        line = f.readline()
        if not line:
            break

        entry = dict()

        settings_line = f.readline()
        settings = re.findall('\d+[\.]*\d*', settings_line)
        entry['trees'] = int(settings.pop(0))
        entry['depth'] = int(settings.pop(0))
        entry['min_node'] = float(settings.pop(0))
        f.readline()
        
        sens2_line = f.readline()
        sens2 = re.findall('\d+\.\d+', sens2_line)
        entry['sens2'] = float(sens2.pop(0))
        entry['err2'] = float(sens2.pop(0))
        sens3_line = f.readline()
        sens3 = re.findall('\d+\.\d+', sens3_line)
        entry['sens3'] = float(sens3.pop(0))
        entry['err3'] = float(sens3.pop(0))
        sens_line = f.readline()
        sens = re.findall('\d+\.\d+', sens_line)
        entry['sens'] = float(sens.pop(0))
        entry['err'] = float(sens.pop(0))
        f.readline()

        entries.append(entry)
        print entry

    entries_by_depth = dict()
    for r in range(2, 6):
        entries_by_depth[r] = [a for a in entries if a['depth'] == r]

    for depth_x, entries_x in entries_by_depth.items():

        data_array = list()
        node_sizes = np.linspace(0.01,0.1,10)

        for size in node_sizes:
            node_entries = [a for a in entries_x if a['node_size'] == size]
            node_entries = sorted(node_entries, key=lambda x: x['trees'])
            node_entries = [a['']]

    print entries_by_depth

    f.close()

if __name__ == '__main__':
    main()
