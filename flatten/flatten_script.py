import os
import fnmatch
import subprocess
from ROOT import TFile, TTree, TChain

tim_directory = "/unix/atlas2/scanlon/group.perf-flavtag.mc15_13TeV."\
    "410000.PowhegPythiaEvtGen_s2608_s2183_r7725_r7676."\
    "BTAGNTUP_V47_full_Akt4EMTo/"

jim_directory = "./"

root_files_in_directory = [jim_directory + f for f in os.listdir(jim_directory)
                           if fnmatch.fnmatch(f, "*.root") and "proc" not in f]

print root_files_in_directory


for filename in root_files_in_directory:
	
	os.environ['FLATTEN_FILE'] = filename
	subprocess.call('./flatten_ex.sh')


print "Processing completed."