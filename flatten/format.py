from ROOT import TFile, TH1
import ROOT


# LOOKUP DICTS
flav_lookup = {"truthflav == 0": "light",
               "truthflav == 4": "charm",
               "truthflav == 5": "bottom"}

band_lookup = {"pt > 20000": "20GeV-40GeV",
               "pt > 40000": "40GeV-80GeV",
               "pt > 80000": "80GeV-140GeV",
               "pt > 140000": "140GeV+"}

stat_lookup =

flav_colour_lookup = {"light": ROOT.kBlue,
               "charm": ROOT.kGreen,
               "bottom": ROOT.kRed}


def HistFormat(hist):
    title = band + ' ' + stat
    hist.SetMarkerColor(flav_colour[truthflav])
    hist.SetLineColor(flav_colour[truthflav])
    hist.SetTitle(title)


def main():
    # Open up out hist .root file.
    hist_file = TFile("Output.root", "READ")

    # Get the keys to access the disk hists.
    nkeys = hist_file.GetNkeys()
    hist_keys = [n for n in range(nkeys)]

    # Generate array of hists from output file.
    hist_array = []
    for i in hist_keys:
        hist_name = hist_file.GetListOfKeys().At(i).GetName()
        hist_array.append(hist_file.Get(hist_name))

    # Close file handle.
    hist_file.Close()


if __name__ == "__main__":
    main()
