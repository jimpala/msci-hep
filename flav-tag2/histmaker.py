import ROOT

# sv0 statistics to study with, given as name:hist_params
sv0_stats = {'sv0_mass' : (50, 0, 4000), 'sv0_ntracks_v' : (10, 0, 10),
             'sv0_normdist' : (50, 0, 120)}

# Flavour colours
flav_colours = {'light' : ROOT.kBlue, 'charm' : ROOT.kOrange,
                'bottom' : ROOT.kRed, 'tau' : ROOT.kGreen}

def HistMaker(flavour, title, sv0_parameter):
    assert type(flavour) == str
    assert type(title) == str
    assert type(sv0_parameter) == str

    # Create hist.
    hist = ROOT.TH1D(title, title, *sv0_stats[sv0_parameter]) #  * unpacks the tuple

    # Format hist.
    hist.GetXaxis().SetTitle(title)
    hist.GetYaxis().SetTitle('n')
    hist.SetMarkerColor(flav_colours[flavour])
    hist.SetLineColor(flav_colours[flavour])
    # hist.SetFillColorAlpha(flav_colours[flavour], 0.5)

    return hist


class histContainer(object):
    flavours = ['light', 'charm', 'bottom', 'tau']
    pt_bands = ['20-40GeV', '40-80GeV', '80-140GeV', '140GeV+']
    sv0_stats = ['sv0_mass', 'sv0_ntracks_v', 'sv0_normdist']


    def __init__(self, hist):
        self.hist = hist
        self.title = hist.GetTitle()
        self.nentries = hist.GetEntries()

    def GetFlavour(self):
        for flav in histContainer.flavours:
            if flav in self.title: return flav

    def GetPtBand(self):
        for band in histContainer.pt_bands:
            if band in self.title: return band

    def GetSv0Stat(self):
        for stat in histContainer.sv0_stats:
            if stat in self.title: return stat

    def Prettify(self):
        self.hist.SetMarkerColor(flav_colours[self.GetFlavour()])
        self.hist.SetLineColor(flav_colours[self.GetFlavour()])
        self.hist.SetFillColorAlpha(flav_colours[self.GetFlavour()], 0.5)