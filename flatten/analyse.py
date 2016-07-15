from ROOT import TFile, TChain, gDirectory, gSystem, TH1D, TColor, TCanvas, TLegend
import ROOT
import os
import sys
import fnmatch

# Import test.C macro's shared library.
gSystem.Load('./test_C.so')
from ROOT import test

truthflavs = [0, 4, 5]

truthflav_lookup = {0: 'light', 4: 'charm',
                    5: 'bottom'}

jet_colour = {0: ROOT.kBlue, 4: ROOT.kGreen, 5: ROOT.kRed}

pt_bands = ['20-40GeV', '40-80GeV', '80-140GeV', '140GeV+']

pt_band_arg = {'20-40GeV': 'jet_pt > 20000 && jet_pt < 40000',
               '40-80GeV': 'jet_pt > 40000 && jet_pt < 80000',
               '80-140GeV': 'jet_pt > 80000 && jet_pt < 1400000',
               '140GeV+': 'jet_pt > 140000'}

# sv0 statistics to study with, given as name:hist_params
sv0_stats = {'sv0_mass': (50, 0, 4000), 'sv0_ntracks_v': (10, 0, 10),
             'sv0_normdist': (50, 0, 120)}

stats = ["jet_sv0_m", "jet_sv0_normdist", "jet_sv0_ntrkv"]


# -----------------------------------------------

def GetFilenames(directory):
    root_files_in_directory = [directory + f for f in os.listdir(directory)
                               if fnmatch.fnmatch(f, "*.root") and "proc" not in f
                               and "Output" not in f]
    return root_files_in_directory


# def MultipageToken(i):
#     if i==0: return "("
#     if i==11: return ")"
#     else: return ""

def Plot(root_filenames):
    """"Creates an array of jet property dicts for all jets in a given list of .root files."""

    mychain = TChain()

    print root_filenames

    for filename in root_filenames:
        current_file = TFile(filename)
        tree_name = current_file.GetListOfKeys().At(0).GetName()
        current_tree = current_file.Get(tree_name)
        print current_tree

        root_processor = test(current_tree)
        root_processor.Loop()

        processed_filename = filename[:-5] + "_proc.root"
        print processed_filename
        current_file = TFile(processed_filename)
        tree_name = current_file.GetListOfKeys().At(0).GetName()

        mychain.AddFile(current_file.GetName(), 100000, tree_name)

        current_file.Close()

    print "Read-in complete."

    create_file = TFile("Output.root", "RECREATE")
    create_file.Close()

    print mychain.GetEntries()

    # Counter
    i_hist = 0

    for band in pt_bands:
        for stat in stats:
            for truthflav in truthflavs:
                # Open up Output.root for writing
                write_file = TFile("Output.root", "UPDATE")

                # Filter entries for current hist, plot. Write to file.
                filter_string = "{0:s} && jet_truthflav == {1:s} && jet_sv0_sig3d > -40" \
                    .format(str(pt_band_arg[band]), str(truthflav))
                mychain.Draw("%s>>hist%s" % (stat, i_hist), filter_string)

                write_file.Write()

                hist_name = write_file.GetListOfKeys().At(i_hist).GetName()
                hist = write_file.Get(hist_name)

                hist.SetTitle("%s %s %s" % (truthflav_lookup[truthflav], band, stat))
                HistFormat(hist, truthflav, band, stat)

                # Flush object memory by closing.
                write_file.Close()

                i_hist += 1

    write_file.ls()


def HistFormat(hist, truthflav, band, stat):
    title = band + ' ' + stat
    hist.SetMarkerColor(jet_colour[truthflav])
    hist.SetLineColor(jet_colour[truthflav])
    hist.SetTitle(title)


def main():
    jim_directory = "./"

    filenames = GetFilenames(jim_directory)

    Plot(filenames)

    raw_input("Press Enter to close program.")


if __name__ == "__main__":
    main()
