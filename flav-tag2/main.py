import flavtag
from ROOT import TFile, TCanvas


def main():

    myFile = TFile('group.perf-flavtag.8324358.Akt4EMTo._000501.root')

    myjets = flavtag.GetJetProperties('bTag_AntiKt4EMTopoJets')

    myhists = flavtag.BandPlot(myjets)

    myhists['charm 40-80GeV sv0_mass'].Draw()

    raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()