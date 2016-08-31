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
    create_file1 = TFile("discrim_mv2c20.root", "RECREATE")
    create_file1.Close()
    create_file2 = TFile("discrim_mv2c10.root", "RECREATE")
    create_file2.Close()
    create_file3 = TFile("discrim_mv2c00.root", "RECREATE")
    create_file3.Close()


    # # Make hists (fine grained)
    #
    # write_file = TFile("Output.root", "UPDATE")
    #
    # # Note that jet pt cuts off below 20MeV
    # total_hist = TH1D('total_hist', 'All jets (mv2c20 > 0)', 200, 20000, 50000)
    # bottom_hist = TH1D('bottom_hist', 'B jets (mv2c20 > 0)', 200, 20000, 50000)
    # charm_hist = TH1D('charm_hist', 'C jets (mv2c20 > 0)', 200, 20000, 50000)
    # light_hist = TH1D('light_hist', 'L jets (mv2c20 > 0)', 200, 20000, 50000)
    # b_pt_efficiency_hist = TH1D('b_pt_efficiency_hist', 'B-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)
    # c_pt_efficiency_hist = TH1D('c_pt_efficiency_hist', 'C-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)
    # l_pt_efficiency_hist = TH1D('l_pt_efficiency_hist', 'L-tag efficiency (mv2c20 > 0)', 200, 20000, 50000)
    #
    # mv2c20_cut = "jet_mv2c20 > 0 "
    # total_filter = mv2c20_cut
    # bottom_filter = mv2c20_cut + " && jet_truthflav == 5"
    # charm_filter = mv2c20_cut + " && jet_truthflav == 4"
    # light_filter = mv2c20_cut + " && jet_truthflav == 0"
    #
    # # '+' in front of the histogram name appends to existing hist.
    # mychain.Draw("jet_pt>>+total_hist", total_filter)
    # mychain.Draw("jet_pt>>+bottom_hist", bottom_filter)
    # mychain.Draw("jet_pt>>+charm_hist", charm_filter)
    # mychain.Draw("jet_pt>>+light_hist", light_filter)
    #
    #
    # # Divide to get efficiency hists.
    # b_pt_efficiency_hist.Divide(bottom_hist, total_hist,1,1,'B')
    # c_pt_efficiency_hist.Divide(charm_hist, total_hist,1,1,'B')
    # l_pt_efficiency_hist.Divide(light_hist, total_hist,1,1,'B')
    #
    # write_file.Write()
    # write_file.Close()
    
    # ---------------------------------------------------
    # ---------------------------------------------------
    write_file1 = TFile("discrim_mv2c20.root", "UPDATE")

    b_mv2c20_hist = TH1D('b_mv2c20_hist', 'mv2c20 discriminant variable', 100, -1, 1)
    l_mv2c20_hist = TH1D('l_mv2c20_hist', 'mv2c20 discriminant variable', 100, -1, 1)
    c_mv2c20_hist = TH1D('c_mv2c20_hist', 'mv2c20 discriminant variable', 100, -1, 1)


    # REMEMBER when using TBrowser to set y-axis to a log scale.
    # Right click the canvas and select 'setlogy'.
    mychain.Draw("jet_mv2c20>>+b_mv2c20_hist", "jet_truthflav == 5")
    mychain.Draw("jet_mv2c20>>+c_mv2c20_hist", "jet_truthflav == 4")
    mychain.Draw("jet_mv2c20>>+l_mv2c20_hist", "jet_truthflav == 0")

    # Normalise to obviate frequency distribution effects.
    normalise(b_mv2c20_hist)
    normalise(c_mv2c20_hist)
    normalise(l_mv2c20_hist)


    ### EFFICIENCY/REJECTION ###

    # Soft check that histograms are binned the same.
    assert(b_mv2c20_hist.GetNbinsX() == l_mv2c20_hist.GetNbinsX() ==
    c_mv2c20_hist.GetNbinsX() )

    # Initialise iteration parameters.
    end_bin = b_mv2c20_hist.GetNbinsX()
    b_eff_mv2c20_plot = []
    l_rej_mv2c20_plot = []
    c_rej_mv2c20_plot = []

    # Iterate through integrals between discrim variable cut and 1.
    for this_bin in range(1, end_bin):

        # Integrate between current bin and end bin.
        b_int = b_mv2c20_hist.Integral(this_bin, end_bin)
        l_int = l_mv2c20_hist.Integral(this_bin, end_bin)
        c_int = c_mv2c20_hist.Integral(this_bin, end_bin)

        # Append to plot arrays. l and cl are rejection plots,
        # so need to be inverse.
        b_eff_mv2c20_plot.append(b_int)
        l_rej_mv2c20_plot.append(1/l_int)
        c_rej_mv2c20_plot.append(1/c_int)


    # Plot the ROC curves with these arrays.
    b_eff_mv2c20_input = array.array('f', tuple(b_eff_mv2c20_plot))
    l_rej_mv2c20_input = array.array('f', tuple(l_rej_mv2c20_plot))
    c_rej_mv2c20_input = array.array('f', tuple(c_rej_mv2c20_plot))

    l_mv2c20_curve = TGraph(len(b_eff_mv2c20_input), b_eff_mv2c20_input, l_rej_mv2c20_input)
    c_mv2c20_curve = TGraph(len(b_eff_mv2c20_input), b_eff_mv2c20_input, c_rej_mv2c20_input)

    l_mv2c20_curve.SaveAs('l_mv2c20.root')
    c_mv2c20_curve.SaveAs('c_mv2c20.root')

    # Write and close.
    write_file1.Write()
    write_file1.Close()

    # ---------------------------------------------------
    # ---------------------------------------------------

    # ---------------------------------------------------
    # ---------------------------------------------------
    write_file2 = TFile("discrim_mv2c10.root", "UPDATE")

    b_mv2c10_hist = TH1D('b_mv2c10_hist', 'mv2c10 discriminant variable', 100, -1, 1)
    l_mv2c10_hist = TH1D('l_mv2c10_hist', 'mv2c10 discriminant variable', 100, -1, 1)
    c_mv2c10_hist = TH1D('c_mv2c10_hist', 'mv2c10 discriminant variable', 100, -1, 1)

    # REMEMBER when using TBrowser to set y-axis to a log scale.
    # Right click the canvas and select 'setlogy'.
    mychain.Draw("jet_mv2c10>>+b_mv2c10_hist", "jet_truthflav == 5")
    mychain.Draw("jet_mv2c10>>+c_mv2c10_hist", "jet_truthflav == 4")
    mychain.Draw("jet_mv2c10>>+l_mv2c10_hist", "jet_truthflav == 0")

    # Normalise to obviate frequency distribution effects.
    normalise(b_mv2c10_hist)
    normalise(c_mv2c10_hist)
    normalise(l_mv2c10_hist)

    ### EFFICIENCY/REJECTION ###

    # Soft check that histograms are binned the same.
    assert (b_mv2c10_hist.GetNbinsX() == l_mv2c10_hist.GetNbinsX() ==
            c_mv2c10_hist.GetNbinsX())

    # Initialise iteration parameters.
    end_bin = b_mv2c10_hist.GetNbinsX()
    b_eff_mv2c10_plot = []
    l_rej_mv2c10_plot = []
    c_rej_mv2c10_plot = []

    # Iterate through integrals between discrim variable cut and 1.
    for this_bin in range(1, end_bin):
        # Integrate between current bin and end bin.
        b_int = b_mv2c10_hist.Integral(this_bin, end_bin)
        l_int = l_mv2c10_hist.Integral(this_bin, end_bin)
        c_int = c_mv2c10_hist.Integral(this_bin, end_bin)

        # Append to plot arrays. l and cl are rejection plots,
        # so need to be inverse.
        b_eff_mv2c10_plot.append(b_int)
        l_rej_mv2c10_plot.append(1 / l_int)
        c_rej_mv2c10_plot.append(1 / c_int)

    # Plot the ROC curves with these arrays.
    b_eff_mv2c10_input = array.array('f', tuple(b_eff_mv2c10_plot))
    l_rej_mv2c10_input = array.array('f', tuple(l_rej_mv2c10_plot))
    c_rej_mv2c10_input = array.array('f', tuple(c_rej_mv2c10_plot))

    l_mv2c10_curve = TGraph(len(b_eff_mv2c10_input), b_eff_mv2c10_input, l_rej_mv2c10_input)
    c_mv2c10_curve = TGraph(len(b_eff_mv2c10_input), b_eff_mv2c10_input, c_rej_mv2c10_input)

    l_mv2c10_curve.SaveAs('l_mv2c10.root')
    c_mv2c10_curve.SaveAs('c_mv2c10.root')

    # Write and close.
    write_file2.Write()
    write_file2.Close()

    # ---------------------------------------------------
    # ---------------------------------------------------

    # ---------------------------------------------------
    # ---------------------------------------------------
    write_file3 = TFile("discrim_mv2c00.root", "UPDATE")

    b_mv2c00_hist = TH1D('b_mv2c00_hist', 'mv2c00 discriminant variable', 100, -1, 1)
    l_mv2c00_hist = TH1D('l_mv2c00_hist', 'mv2c00 discriminant variable', 100, -1, 1)
    c_mv2c00_hist = TH1D('c_mv2c00_hist', 'mv2c00 discriminant variable', 100, -1, 1)

    # REMEMBER when using TBrowser to set y-axis to a log scale.
    # Right click the canvas and select 'setlogy'.
    mychain.Draw("jet_mv2c00>>+b_mv2c00_hist", "jet_truthflav == 5")
    mychain.Draw("jet_mv2c00>>+c_mv2c00_hist", "jet_truthflav == 4")
    mychain.Draw("jet_mv2c00>>+l_mv2c00_hist", "jet_truthflav == 0")

    # Normalise to obviate frequency distribution effects.
    normalise(b_mv2c00_hist)
    normalise(c_mv2c00_hist)
    normalise(l_mv2c00_hist)

    ### EFFICIENCY/REJECTION ###

    # Soft check that histograms are binned the same.
    assert (b_mv2c00_hist.GetNbinsX() == l_mv2c00_hist.GetNbinsX() ==
            c_mv2c00_hist.GetNbinsX())

    # Initialise iteration parameters.
    end_bin = b_mv2c00_hist.GetNbinsX()
    b_eff_mv2c00_plot = []
    l_rej_mv2c00_plot = []
    c_rej_mv2c00_plot = []

    # Iterate through integrals between discrim variable cut and 1.
    for this_bin in range(1, end_bin):
        # Integrate between current bin and end bin.
        b_int = b_mv2c00_hist.Integral(this_bin, end_bin)
        l_int = l_mv2c00_hist.Integral(this_bin, end_bin)
        c_int = c_mv2c00_hist.Integral(this_bin, end_bin)

        # Append to plot arrays. l and cl are rejection plots,
        # so need to be inverse.
        b_eff_mv2c00_plot.append(b_int)
        l_rej_mv2c00_plot.append(1 / l_int)
        c_rej_mv2c00_plot.append(1 / c_int)


    # Plot the ROC curves with these arrays.
    b_eff_mv2c00_input = array.array('f', tuple(b_eff_mv2c00_plot))
    l_rej_mv2c00_input = array.array('f', tuple(l_rej_mv2c00_plot))
    c_rej_mv2c00_input = array.array('f', tuple(c_rej_mv2c00_plot))

    l_mv2c00_curve = TGraph(len(b_eff_mv2c00_input), b_eff_mv2c00_input, l_rej_mv2c00_input)
    c_mv2c00_curve = TGraph(len(b_eff_mv2c00_input), b_eff_mv2c00_input, c_rej_mv2c00_input)

    l_mv2c00_curve.SaveAs('l_mv2c00.root')
    c_mv2c00_curve.SaveAs('c_mv2c00.root')

    # WRITE AND CLOSE DELAYED FOR FINAL PART

    # ---------------------------------------------------
    # ---------------------------------------------------

    ### TAGGER COMPARISON METRICS ###
    # Use functional here!


    # Use fit formulae to generate corresponding co-ords for c20 and l00.
    # l00 equ: e^(9.7173-10.6069x)
    # c20 equ: e^(21.9226-21.9956x)
    # (where x is b efficiency)
    e = math.exp(1)


    print e
    l_rej_mv2c00_gen = [e ** (21.9993 - (20.7678*x)) for x in b_eff_mv2c20_plot]
    c_rej_mv2c20_gen = [e ** (9.7173 - (10.6069*x)) for x in b_eff_mv2c00_plot]
    print b_eff_mv2c20_plot
    print l_rej_mv2c00_gen

    l_rej_mv2c00_gen_input = array.array('f', tuple(l_rej_mv2c00_gen))
    l_gen_curve = TGraph(len(l_rej_mv2c00_gen_input), b_eff_mv2c20_input, l_rej_mv2c00_gen_input)
    l_gen_curve.SaveAs('test.root')


    l_ratio_plot = map(lambda x,y: x/y, l_rej_mv2c00_gen, l_rej_mv2c20_plot)
    c_ratio_plot = map(lambda x,y: x/y, c_rej_mv2c20_gen, c_rej_mv2c00_plot)


    # Convert to ROOT friendly inputs; plot and save.

    l_ratio_input = array.array('f', tuple(l_ratio_plot))
    c_ratio_input = array.array('f', tuple(c_ratio_plot))

    l_ratio_curve = TGraph(len(b_eff_mv2c00_input), b_eff_mv2c20_input, l_ratio_input)
    c_ratio_curve = TGraph(len(b_eff_mv2c00_input), b_eff_mv2c00_input, c_ratio_input)

    l_ratio_curve.SaveAs('l_tag_ratio.root')
    c_ratio_curve.SaveAs('c_tag_ratio.root')

    # Write and close.
    write_file3.Write()
    write_file3.Close()



## HIST NORMALISING HELPER ##
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
