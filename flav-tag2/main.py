import flavtag
import histmaker
from histmaker import histContainer, flav_colours
from ROOT import TFile, TCanvas, TLegend, TTree, TChain
import os
import sys
import fnmatch


def main():

    # Import TFile/TChain object via ROOT for analysis.
    # NOTE: Even though the IDE produces a warning that this variable
    # is never used, it is nonetheless required.

    root_files_in_directory = [f for f in os.listdir('.') if os.path.isfile(f) and
                               fnmatch.fnmatch(f, '*.root')]

    myjets, underflow = flavtag.GetJetProperties(root_files_in_directory)


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
                if maxfreq > freqlim:
                    freqlim = maxfreq

            for hist in plot:


                # Draw hist.
                hist.Draw('same hist')
                mylegends[i].AddEntry(hist, "%s Jets" % hist.GetTitle()[0], "l")

                hist.SetAxisRange(0, freqlim, "Y")
                hist.Rebin()

                hist.SetTitle(title)
                hist.SetXTitle(stat)

            # Draw legend on to canvas, output to pdf and increase counter
            multipage_token = ""
            if i==0:
                multipage_token = "("
            elif i==11:
                multipage_token = ")"

            mylegends[i].Draw()
            mycanvasses[i].Print('plots.pdf%s' % multipage_token, 'Title:%s' % title)

            i += 1





    #### Use when wanting to view graphs.
    # raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()