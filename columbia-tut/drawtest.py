import ROOT

def main():
    myFile = ROOT.TFile("experiment.root")
    tree1 = ROOT.gROOT.FindObject("tree1")


    tree1.Scan()
    tree1.Draw("ebeam")

    raw_input("Press Enter to close program.")

if __name__ == "__main__":
    main()