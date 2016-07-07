from ROOT import TFile, TChain , gDirectory
import numpy as np
import histmaker
from histmaker import histContainer


# Truth flav lookup dict
truthflav_lookup = {0 : 'light', 4 : 'charm',
                    5 : 'bottom'}

# Root file has pt measurements in MeV
pt_bands = {20000 : '20-40GeV', 40000 : '40-80GeV', 80000 : '80-140GeV',
            140000 : '140GeV+'}




def GetJetProperties(root_filenames):
    """"Creates an array of jet property dicts for all jets in a given list of .root files."""
    underflow = 0
    mychain = TChain()

    for filename in root_filenames:
        current_file = TFile(filename)
        tree_name = current_file.GetListOfKeys().At(0).GetName()


        mychain.AddFile(current_file.GetName(), 100000, tree_name)

    entries = mychain.GetEntriesFast() # no. of entries as long

    jets_array = []
    for jentry in xrange(entries):
        nb= mychain.GetEntry(jentry)
        if nb <= 0: continue #Breaks this pass and moves on to next.
        if not np.array(mychain.jet_pt).size: continue  # Checks if entry is empty

        underflow = 1
        i = 0

        for jet in xrange(np.array(mychain.jet_pt).size):
            jet_properties = dict()

            # Extract properties for jet. Ignore tau jets (except clause).
            try:
                jet_properties['flav'] = truthflav_lookup[mychain.jet_truthflav[i]]
            except LookupError:
                underflow += 1
                continue


            jet_properties['pt'] = mychain.jet_pt[i]
            for key in sorted(pt_bands.keys(), key=int):
                if jet_properties['pt'] >= key:
                    jet_properties['pt_band'] = pt_bands[key]
                else: break
            jet_properties['sv0_exists'] = True if (mychain.jet_sv0_sig3d[i] > -50) else False
            if jet_properties['sv0_exists']:
                jet_properties['sv0_mass'] = mychain.jet_sv0_m[i]
                jet_properties['sv0_ntracks_v'] = mychain.jet_sv0_ntrkv[i]
                jet_properties['sv0_normdist'] = mychain.jet_sv0_normdist[i]
            else: underflow += 1

            jets_array.append(jet_properties)
            i+=1

    return jets_array, underflow


def BandPlot(jets_array, underflow):
    """pt-bins an array of jets property dicts and fills them into hists."""
    # Initialize hists dict.
    hists = {}

    # Initialize hists for all flavour, pt band and sv0 stat combos
    # key:values are title:hists
    for flavour in truthflav_lookup.values():
        for band in pt_bands.values():
            for stat in histmaker.sv0_stats.keys():
                name = "%s %s %s" % (flavour, band, stat)
                hists[name] = histmaker.HistMaker(flavour, name, stat)

    # Iterate through the jets
    for jet in jets_array:
        if not jet['sv0_exists']: continue

        for key in jet.keys():
            try_name = "%s %s %s" % (jet['flav'], jet['pt_band'], key)
            try:
                hists[try_name].Fill(jet[key])
            except LookupError:
                pass

    for hist in hists.values():
        hist.Fill(-100, underflow)

    histcontainers = []
    for hist in hists.values():
        histcontainers.append(histContainer(hist))

    return histcontainers