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
                               and "Output" not in f and "base" not in f]
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


    # Make hists (fine grained)

    write_file = TFile("Output.root", "UPDATE")

    # Note that jet pt cuts off below 20MeV
    truth_hist = TH1D('truth_hist', 'B jets (TRUTH)', 2000, 20000, 50000)
    tagged_hist = TH1D('tagged_hist', 'B jets (TAGGED)', 2000, 20000, 50000)
    metric_hist = TH1D('metric_hist', 'B jets (TRUTH) / B jets (TAGGED)', 2000, 20000, 50000)

    truth_filter = "jet_truthflav == 5"
    tagged_filter = "jet_mv2c20 > 0"

    mychain.Draw("jet_pt>>+truth_hist", truth_filter)
    mychain.Draw("jet_pt>>+tagged_hist", tagged_filter)

    metric_hist.Divide(truth_hist,tagged_hist,1,1,'B')

    write_file.Write()
    write_file.Close()



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
