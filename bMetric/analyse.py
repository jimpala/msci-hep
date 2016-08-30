from ROOT import TFile, TChain, gDirectory, gSystem, TH1D, TCanvas
import ROOT
import os
import sys
import fnmatch

# Import test.C macro's shared library.
gSystem.Load('./tag_metric_C.so')
from ROOT import tag_metric


# -----------------------------------------------

def GetFilenames(directory):
    root_files_in_directory = [directory + f for f in os.listdir(directory)
                               if fnmatch.fnmatch(f, "*.root") and "proc" not in f
                               and "group" in f]
    return root_files_in_directory


def Plot(root_filenames):
    """"Creates an array of jet property dicts for all jets in a given list of .root files."""

    mychain = TChain()

    print root_filenames

    for filename in root_filenames:
        current_file = TFile(filename)

        # Make sure that the root file has a tree in it.
        if current_file.GetNkeys() != 0:
            tree_name = current_file.GetListOfKeys().At(0).GetName()
            current_tree = current_file.Get(tree_name)

            root_processor = tag_metric(current_tree)
            root_processor.Loop()

            processed_filename = filename[:-5] + "_proc.root"
            current_file = TFile(processed_filename)
            tree_name = current_file.GetListOfKeys().At(0).GetName()

            mychain.AddFile(current_file.GetName(), 100000, tree_name)

        else:
            print filename + " does not contain a tree."

        current_file.Close()

    print "Read-in complete."


    # Initialise output root file.
    create_file = TFile("Output.root", "RECREATE")
    create_file.Close()
    create_file2 = TFile("Output2.root", "RECREATE")
    create_file2.Close()


    # Make hists (fine grained)

    write_file = TFile("Output.root", "UPDATE")

    # Note that jet pt cuts off below 20MeV
    total_hist = TH1D('total_hist', 'All jets (mv2c20 > 0)', 200, 20000, 50000)
    bottom_hist = TH1D('bottom_hist', 'B jets (mv2c20 > 0)', 200, 20000, 50000)
    charm_hist = TH1D('charm_hist', 'C jets (mv2c20 > 0)', 200, 20000, 50000)
    light_hist = TH1D('light_hist', 'L jets (mv2c20 > 0)', 200, 20000, 50000)
    b_efficiency_hist = TH1D('b_efficiency_hist', 'B-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)
    c_efficiency_hist = TH1D('c_efficiency_hist', 'C-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)
    l_efficiency_hist = TH1D('l_efficiency_hist', 'L-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)

    mv2c20_cut = "jet_mv2c20 > 0 "
    total_filter = mv2c20_cut
    bottom_filter = mv2c20_cut + " && jet_truthflav == 5"
    charm_filter = mv2c20_cut + " && jet_truthflav == 4"
    light_filter = mv2c20_cut + " && jet_truthflav == 0"

    # '+' in front of the histogram name appends to existing hist.
    mychain.Draw("jet_pt>>+total_hist", total_filter)
    mychain.Draw("jet_pt>>+bottom_hist", bottom_filter)
    mychain.Draw("jet_pt>>+charm_hist", charm_filter)
    mychain.Draw("jet_pt>>+light_hist", light_filter)

    # Divide to get efficiency hists.
    b_efficiency_hist.Divide(bottom_hist, total_hist,1,1,'B')
    c_efficiency_hist.Divide(charm_hist, total_hist,1,1,'B')
    l_efficiency_hist.Divide(light_hist, total_hist,1,1,'B')

    write_file.Write()
    write_file.Close()

    write_file2 = TFile("Output2.root", "UPDATE")

    b_discrim_hist = TH1D('b_discrim_hist', 'mv2c20 discriminant variable', 100, -1, 1)
    c_discrim_hist = TH1D('c_discrim_hist', 'mv2c20 discriminant variable', 100, -1, 1)
    l_discrim_hist = TH1D('l_discrim_hist', 'mv2c20 discriminant variable', 100, -1, 1)

    # REMEMBER when using TBrowser to set y-axis to a log scale.
    mychain.Draw("jet_mv2c20>>+b_discrim_hist", "jet_truthflav == 5")
    mychain.Draw("jet_mv2c20>>+c_discrim_hist", "jet_truthflav == 4")
    mychain.Draw("jet_mv2c20>>+l_discrim_hist", "jet_truthflav == 0")

    normalise(b_discrim_hist)
    normalise(c_discrim_hist)
    normalise(l_discrim_hist)

    write_file2.Write()
    write_file2.Close()




def normalise(hist):
    integral = hist.Integral()
    norm_factor = 1 / integral
    hist.Scale(norm_factor)


def main():
    jim_directory = "./"
    tim_directory = "/unix/atlas2/scanlon/group.perf-flavtag.mc15_13TeV."\
    "410000.PowhegPythiaEvtGen_s2608_s2183_r7725_r7676."\
    "BTAGNTUP_V47_full_Akt4EMTo/"

    mode = jim_directory

    filenames = GetFilenames(mode)

    Plot(filenames)

    # Immediately terminate unless in jim_directory mode.
    if mode == jim_directory:
        raw_input("Press Enter to close program.")


if __name__ == "__main__":
    main()
