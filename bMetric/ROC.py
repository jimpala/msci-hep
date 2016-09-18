from ROOT import TFile, TChain, gDirectory, gSystem, TH1D, TCanvas, TGraph
import ROOT
import os
import sys
import fnmatch
import array
import math

# Import test.C macro's shared library.
gSystem.Load('./tag_metric_C.so')
from ROOT import tag_metric

#--------------------------------------


def getFilenames(directory):
    """Finds all .root files we want in a specified directory."""
    root_files_in_directory = [directory + f for f in os.listdir(directory)
                               if fnmatch.fnmatch(f, "*.root") and "proc" not in f
                               and "group" in f]
    return root_files_in_directory


def processToChain(root_filenames):
    """"Processes list of homogenous .root files into a TChain object."""

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
    mychain.Print()

    return mychain



def createROC(chain, discrim_var, discrim_nbins, discrim_lo_bound, discrim_hi_bound):
    """Outputs a ROC curve for a given tagger."""
    # Initialise output root file.
    create_file = TFile("output_{}.root".format(discrim_var), "RECREATE")
    create_file.Close()

    write_file = TFile("output_{}.root".format(discrim_var), "UPDATE")

    b_discrim_hist = TH1D('b_discrim_hist', '', discrim_nbins, discrim_lo_bound, discrim_hi_bound)
    l_discrim_hist = TH1D('l_discrim_hist', '', discrim_nbins, discrim_lo_bound, discrim_hi_bound)
    c_discrim_hist = TH1D('c_discrim_hist', '', discrim_nbins, discrim_lo_bound, discrim_hi_bound)


    # REMEMBER when using TBrowser to set y-axis to a log scale.
    # Right click the canvas and select 'setlogy'.
    chain.Draw("{}>>+b_discrim_hist".format(discrim_var), "jet_truthflav == 5")
    chain.Draw("{}>>+c_discrim_hist".format(discrim_var), "jet_truthflav == 4")
    chain.Draw("{}>>+l_discrim_hist".format(discrim_var), "jet_truthflav == 0")

    # Normalise to obviate frequency distribution effects.
    normalise(b_discrim_hist)
    normalise(c_discrim_hist)
    normalise(l_discrim_hist)


    # Soft check that histograms are binned the same.
    assert(b_discrim_hist.GetNbinsX() == l_discrim_hist.GetNbinsX() ==
    c_discrim_hist.GetNbinsX() )

    # Initialise iteration parameters.
    end_bin = b_discrim_hist.GetNbinsX()
    b_eff_plot = []
    l_rej_plot = []
    c_rej_plot = []
    rejected = 0

    # Iterate through integrals between discrim variable cut and 1.
    for this_bin in range(0, end_bin + 1):

        # Integrate between current bin and end bin.
        b_int = b_discrim_hist.Integral(this_bin, end_bin)

        l_int = l_discrim_hist.Integral(this_bin, end_bin)
        c_int = c_discrim_hist.Integral(this_bin, end_bin)

        # Append to plot arrays. l and cl are rejection plots,
        # so need to be inverse.

        

        if l_int > 0 and c_int > 0 and b_int < 0.99:
            b_eff_plot.append(b_int)
            l_rej_plot.append(1/l_int)
            c_rej_plot.append(1/c_int)

        else:
            rejected += 1

    print "{} rejected:{}".format(discrim_var, rejected)


    # Plot the ROC curves with these arrays.
    b_eff_input = array.array('f', tuple(b_eff_plot))
    l_rej_input = array.array('f', tuple(l_rej_plot))
    c_rej_input = array.array('f', tuple(c_rej_plot))

    l_curve = TGraph(len(b_eff_input), b_eff_input, l_rej_input)
    c_curve = TGraph(len(b_eff_input), b_eff_input, c_rej_input)
    print "{} accepted:{}".format(discrim_var, len(b_eff_input))
    print "{} total:{}".format(discrim_var, len(b_eff_input) + rejected)

    l_curve.SaveAs('lROC_{}.root'.format(discrim_var))
    c_curve.SaveAs('cROC_{}.root'.format(discrim_var))

    # Write and close.
    write_file.Write()
    write_file.Close()



def normalise(hist):
    """Normalises a given histogram."""
    integral = hist.Integral()
    norm_factor = 1 / integral
    hist.Scale(norm_factor)


def main():
    jim_directory = "./"
    tim_directory = "/unix/atlas2/scanlon/group.perf-flavtag.mc15_13TeV."\
    "410000.PowhegPythiaEvtGen_s2608_s2183_r7725_r7676."\
    "BTAGNTUP_V47_full_Akt4EMTo/"

    # SET DIRECTORY MODE
    mode = jim_directory

    # Process TChains.
    filenames = getFilenames(mode)
    mychain = processToChain(filenames)


    # ROC creations.
    createROC(mychain, 'jet_mv2c20', 10000, -1, 1)
    createROC(mychain, 'jet_sv0_sig3d', 10000, -101, 50)
    createROC(mychain, 'jet_sv1_sig3d', 10000, -101, 50)
    createROC(mychain, 'jet_ip2d_llr', 10000, -102, 20)
    createROC(mychain, 'jet_ip3d_llr', 10000, -102, 30)
    createROC(mychain, 'jet_jf_llr', 10000, -100, 10)
    createROC(mychain, 'jet_jfcombnn_llr', 10000, -24, 7)
    createROC(mychain, 'jet_sv1ip3d', 10000, -12, 20)

    # Immediately terminate unless in jim_directory mode.
    if mode == jim_directory:
        raw_input("Press Enter to close program.")


if __name__ == "__main__":
    main()
