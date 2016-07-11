import os
from ROOT import TFile, TTree, TChain

myChain = TChain()
current_file = TFile()

filename = os.environ['FLATTEN_FILE']
print os.environ['FLATTEN_FILE']

current_file = TFile.Open(filename, "READ")
tree_name = current_file.GetListOfKeys().At(0).GetName()

myChain.AddFile(current_file.GetName(), 1000000, tree_name)

myChain.Process('flattener.C')

print filename + " processed."