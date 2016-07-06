import flavtag
import histmaker
from histmaker import histContainer, flav_colours
from ROOT import TFile, TCanvas, TLegend


def main():

    # Import TFile/TChain object via ROOT for analysis.
    # NOTE: Even though the IDE produces a warning that this variable
    # is never used, it is nonetheless required.
    myFile = TFile('group.perf-flavtag.8324358.Akt4EMTo._000501.root')

    myjets, underflow = flavtag.GetJetProperties('bTag_AntiKt4EMTopoJets')


    myhistcontainers = flavtag.BandPlot(myjets, underflow)

    # Initialise canvas/legend arrays.
    mycanvasses = []
    mylegends = []

    # Create array of 12 TCanvasses and TLegends.
    for i in xrange(12):
        mycanvasses.append(TCanvas('c%s' % i, 'Canvas %s' % i, 1))
        mylegends.append(TLegend(0.55,0.65,0.76,0.82))

    i = 0
    for band in histContainer.pt_bands:
        for stat in histContainer.sv0_stats:
            containers_for_stat = [x for x in myhistcontainers if x.GetPtBand()
                     == '%s' % band and x.GetSv0Stat() == '%s' % stat]
            plot = map(lambda x: x.hist, containers_for_stat)
            title = stat + " @ " + "p_{T}=" + band
            mycanvasses[i].cd()

            # Sort plot order in terms of most entries.
            plot = sorted(plot, key=lambda x : histContainer(x).nentries, reverse=True)

            # Normalise hists.
            for hist in plot:
                integral = hist.Integral()
                norm_factor = 1 / integral
                hist.Scale(norm_factor)

            freqlim = 0 # Set max frequency for y axis limits.
            for hist in plot:
                maxbin = hist.GetMaximumBin()
                maxfreq = hist.GetBinContent(maxbin)
                print maxfreq
                if maxfreq > freqlim:
                    freqlim = maxfreq

            for hist in plot:


                # Draw hist.
                hist.Print()
                hist.Draw('same hist')
                mylegends[i].AddEntry(hist, "%s Jets" % hist.GetTitle()[0], "l")

                hist.SetAxisRange(0, freqlim, "Y")
                hist.Rebin()

                hist.SetTitle(title)
                hist.SetXTitle(stat)


            mylegends[i].Draw()
            i += 1





    raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()