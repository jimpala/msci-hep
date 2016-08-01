from ROOT import TFile, TChain, gDirectory, gSystem, TH1D, TCanvas
import ROOT

# Pull hists from Output.root
output_file = TFile('Output.root', 'UPDATE')

tru_hist_name = output_file.GetListOfKeys().At(0).GetName()
print tru_hist_name
tag_hist_name = output_file.GetListOfKeys().At(1).GetName()

tru_hist = output_file.Get(tru_hist_name)
tag_hist = output_file.Get(tag_hist_name)


tru_hist.SetAxisRange(0,500)
tag_hist.SetAxisRange(0,500)