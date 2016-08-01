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


    # TRUTH HIST
    # ----------

    # Open up Output.root for writing
    write_file = TFile("Output.root", "UPDATE")

    # Filter entries for current hist, plot. Write to file.
    filter_string = "jet_truthflav == 5"
    mychain.Draw("jet_pt>>truth_hist", filter_string)

    write_file.Write()

    # Flush object memory by closing.
    write_file.Close()




    # TAGGED HIST
    # ----------

    # Open up Output.root for writing
    write_file = TFile("Output.root", "UPDATE")

    # Filter entries for current hist, plot. Write to file.
    filter_string = "jet_mv2c20 > 0"
    mychain.Draw("jet_pt>>tagged_hist", filter_string)

    write_file.Write()

    # Flush object memory by closing.
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
