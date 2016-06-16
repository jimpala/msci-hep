import flavtag
import histmaker
from histmaker import histContainer
from ROOT import TFile, TCanvas


def main():

    myFile = TFile('group.perf-flavtag.8324358.Akt4EMTo._000501.root')

    myjets = flavtag.GetJetProperties('bTag_AntiKt4EMTopoJets')

    myhistcontainers = flavtag.BandPlot(myjets)

    mycanvasses = []
    for i in xrange(12):
        mycanvasses.append(TCanvas('c%s' % i, 'Canvas %s' % i, 1))

    i = 0
    for band in histContainer.pt_bands:
        for stat in histContainer.sv0_stats:
            plot1 = [x.hist for x in myhistcontainers if x.GetPtBand()
                     == '%s' % band and x.GetSv0Stat() == '%s' % stat]
            mycanvasses[i].cd()
            plot1 = sorted(plot1, key=lambda x : histContainer(x).nentries, reverse=True)
            for hist in plot1:
                hist.Print()
                hist.Draw('same')
            i += 1





    raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()