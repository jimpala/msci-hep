from ROOT import TFile, TChain , gDirectory, TH1D, TColor, TCanvas, TLegend

truthflavs = [0,4,5]

truthflav_lookup = {0: 'light', 4: 'charm',
                    5: 'bottom'}

jet_colour = {0: kBlue, 4: kYellow, 5: kRed}

pt_bands = {20000: '20-40GeV', 40000: '40-80GeV', 80000: '80-140GeV',
            140000: '140GeV+'}

pt_band_arg = {1 : 'jet_pt > 20000 && jet_pt < 40000',
               4 : 'jet_pt > 20000 && jet_pt < 40000',
               5: 'jet_pt > 20000 && jet_pt < 40000'}

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




    # Initialise canvas and hist arrays with counters.
    canvas_array = [TCanvas() for x in range(12)]
    legend_array = [TLegend(0.55,0.65,0.76,0.82)]
    hist_array = [TH1D() for x in range(36)]
    i_canvas = 0
    i_hist = 0

    for bands in pt_bands.keys():
        stat = 'jet_sv0_m'
        canvas_array.cd(i_canvas)
        for truthflav in truthflavs:
            mychain.Draw("jet_sv0_m>>hist_array[%s]" % i_canvas,
                         "%s && jet_truthflav == %s" %
                         (pt_band_arg[truthflav], truthflav), "same hist")
            HistFormat(hist_array[i_hist])
            i_hist += 1

        legend_array[i].Draw()
        canvas_array[i].Print('plots.pdf%s' % MultipageToken(i_canvas))

        i_canvas+=1

    for bands in pt_bands.keys():
        stat = 'jet_sv0_ntrkv'
        canvas_array.cd(i_canvas)
        for truthflav in truthflavs:
            mychain.Draw("jet_sv0_ntrkv>>hist_array[%s]" % i_canvas,
                         "%s && jet_truthflav == %s" %
                         (pt_band_arg[truthflav], truthflav), "same hist")
            HistFormat(hist_array[i_hist])
            i_hist += 1
        i_canvas += 1

        legend_array[i].Draw()
        canvas_array[i].Print('plots.pdf%s' % MultipageToken(i_canvas))

    for band in pt_bands.keys():
        stat = 'jet_sv0_normdist'
        canvas_array.cd(i_canvas)
        for truthflav in truthflavs:
            mychain.Draw("jet_sv0_normdist>>hist_array[%s]" % i_canvas,
                         "%s && jet_truthflav == %s" %
                         (pt_band_arg[truthflav], truthflav), "same hist")
            HistFormat(hist_array[i_hist], truthflav, band, stat)
            i_hist += 1
        i_canvas += 1

        legend_array[i].Draw()
        canvas_array[i].Print('plots.pdf%s' % MultipageToken(i_canvas))


def HistFormat(hist, truthflav, band, stat):
    title = pt_bands[band] + ' ' + stat
    hist.SetMarkerColor(jet_colour[truthflav])
    hist.SetLineColor(jet_colour[truthflav])
    hist.SetTitle(title)



def main():
    jim_directory = "./"

    filenames = GetFilenames(jim_directory)

    Plot(filenames)

    raw_input("Press Enter to close program.")


if __name__ == __main__:
    main()