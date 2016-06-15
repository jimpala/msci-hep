import ROOT

# sv0 statistics to study with, given as name:hist_params
sv0_stats = {'sv0_mass' : (100, 0, 4000), 'sv0_ntracks_v' : (10, 0, 10),
             'sv0_normdist' : (50, 0, 120)}

# Flavour colours
flav_colours = {'light' : ROOT.kBlue, 'charm' : ROOT.kPink,
                'bottom' : ROOT.kRed, 'tau' : ROOT.kGreen}

def HistMaker(flavour, title, sv0_parameter):
    assert type(flavour) == str
    assert type(title) == str
    assert type(sv0_parameter) == str

    # Create hist.
    hist = ROOT.TH1D(title, title, *sv0_stats[sv0_parameter]) # * unpacks the tuple

    # Format hist.
    hist.GetXaxis().SetTitle(title)
    hist.GetYaxis().SetTitle('n')
    hist.SetMarkerColor(flav_colours[flavour])
    hist.SetLineColor(flav_colours[flavour])

    return hist


def CanvasDraw(hist, canvas):
    canvas.ls()
    hist.Draw()