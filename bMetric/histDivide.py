from ROOT import TFile, TChain, gDirectory, gSystem, TH1D, TCanvas
import ROOT

outputted_file = TFile('Output.root', 'READ')

tru_hist_name = outputted_file.GetListOfKeys().At(0).GetName()
tag_hist_name = outputted_file.GetListOfKeys().At(1).GetName()

tru_hist = outputted_file.Get(tru_hist_name)
tag_hist = outputted_file.Get(tag_hist_name)

