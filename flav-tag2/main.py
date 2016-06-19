import flavtag
import histmaker
from histmaker import histContainer, flav_colours
from ROOT import TFile, TCanvas, TLegend


def main():

    myFile = TFile('group.perf-flavtag.8324358.Akt4EMTo._000501.root')

    myjets, underflow = flavtag.GetJetProperties('bTag_AntiKt4EMTopoJets')

    myhistcontainers = flavtag.BandPlot(myjets, underflow)

    for histcont in myhistcontainers:
        histcont.hist.SetFillColorAlpha(flav_colours[histcont.GetFlavour()], 0.5)

    mycanvasses = []
    mylegends = []
    for i in xrange(12):
        mycanvasses.append(TCanvas('c%s' % i, 'Canvas %s' % i, 1))
        mylegends.append(TLegend(0.55,0.65,0.76,0.82))

    i = 0
    for band in histContainer.pt_bands:
        for stat in histContainer.sv0_stats:
            plot = [x.hist for x in myhistcontainers if x.GetPtBand()
                     == '%s' % band and x.GetSv0Stat() == '%s' % stat]
            mycanvasses[i].cd()
            plot = sorted(plot, key=lambda x : histContainer(x).nentries, reverse=True)
            for hist in plot:
                hist.Print()
                hist.Rebin(2)
                hist.Draw('same hist')
                mylegends[i].AddEntry(hist, "%s Jets" % hist.GetTitle()[0], "l")

            mylegends[i].Draw()
            i += 1





    raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()