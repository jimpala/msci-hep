from ROOT import TFile, TChain , gDirectory, TH1D, TColor, TCanvas, TLegend
import ROOT
import os
import sys
import fnmatch

truthflavs = [0,4,5]

truthflav_lookup = {0: 'light', 4: 'charm',
                    5: 'bottom'}

jet_colour = {0: ROOT.kBlue, 4: ROOT.kGreen, 5: ROOT.kRed}

pt_bands = ['20-40GeV', '40-80GeV', '80-140GeV', '140GeV+']

pt_band_arg = {'20-40GeV' : 'jet_pt > 20000 && jet_pt < 40000',
               '40-80GeV' : 'jet_pt > 40000 && jet_pt < 80000',
               '80-140GeV': 'jet_pt > 80000 && jet_pt < 1400000',
               '140GeV+': 'jet_pt > 140000'}

# sv0 statistics to study with, given as name:hist_params
sv0_stats = {'sv0_mass' : (50, 0, 4000), 'sv0_ntracks_v' : (10, 0, 10),
             'sv0_normdist' : (50, 0, 120)}

#-----------------------------------------------

def GetFilenames(directory):
    root_files_in_directory = [directory + f for f in os.listdir(directory)
                               if fnmatch.fnmatch(f, "*.root") and "proc" in f]
    return root_files_in_directory


def MultipageToken(i):
    if i==0: return "("
    if i==11: return ")"
    else: return ""

def Plot(root_filenames):
    """"Creates an array of jet property dicts for all jets in a given list of .root files."""
    underflow = 0
    mychain = TChain()

    for filename in root_filenames:
        current_file = TFile(filename)
        tree_name = current_file.GetListOfKeys().At(0).GetName()

        mychain.AddFile(current_file.GetName(), 100000, tree_name)

    print "Read-in complete."


    print mychain.GetEntries()

    # Initialise canvas and hist arrays with counters.
    dummy_canvas = TCanvas('Dummy','Dummy',1)
    canvas_array = [TCanvas('Canvas %s' % x,'Canvas %s' % x,1) for x in range(12)]
    legend_array = [TLegend(0.55,0.65,0.76,0.82) for x in range(12)]
    hist_array = []
    i_canvas = 0
    i_hist = 0

    # jet_sv0_m - SV MASS ITERATION
    for band in pt_bands:
        stat = 'jet_sv0_m'
        for truthflav in truthflavs:
            dummy_canvas.cd()
            filter_string = "%s && jet_truthflav == %s" % (str(pt_band_arg[band]), str(truthflav))
            mychain.Draw("jet_sv0_m>>hist", filter_string, "same hist")
            hist = gDirectory.Get("hist")
            HistFormat(hist, truthflav, band, stat)
            hist_array.append(hist)
            canvas_array[i_canvas].cd()
            hist.Draw("same hist")
            i_hist += 1

        legend_array[i_canvas].Draw()
        canvas_array[i_canvas].Print('plots.pdf%s' % MultipageToken(i_canvas))

        i_canvas+=1


    # jet_sv0_m - SV N TRACKS ITERATION
    for band in pt_bands:
        stat = 'jet_sv0_ntrkv'

        for truthflav in truthflavs:
            dummy_canvas.cd()
            filter_string = "%s && jet_truthflav == %s" % (str(pt_band_arg[band]), str(truthflav))
            mychain.Draw("jet_sv0_ntrkv>>hist", filter_string, "same hist")
            hist = gDirectory.Get("hist")
            HistFormat(hist, truthflav, band, stat)
            canvas_array[i_canvas].cd()
            hist.Draw("same hist")
            hist_array.append(hist)
            i_hist += 1

        legend_array[i_canvas].Draw()
        canvas_array[i_canvas].Print('plots.pdf%s' % MultipageToken(i_canvas))

        i_canvas += 1

    # jet_sv0_ntrkv - SV NORMDIST ITERATION
    for band in pt_bands:
        stat = 'jet_sv0_normdist'
        canvas_array[i_canvas].cd()
        for truthflav in truthflavs:
            dummy_canvas.cd()
            filter_string = "%s && jet_truthflav == %s" % (str(pt_band_arg[band]), str(truthflav))
            mychain.Draw("jet_sv0_m>>hist", filter_string, "same hist")
            hist = gDirectory.Get("hist")
            HistFormat(hist, truthflav, band, stat)
            canvas_array[i_canvas].cd()
            hist.Draw("same hist")
            hist_array.append(hist)
            i_hist += 1

        legend_array[i_canvas].Draw()
        canvas_array[i_canvas].Print('plots.pdf%s' % MultipageToken(i_canvas))

        i_canvas += 1


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