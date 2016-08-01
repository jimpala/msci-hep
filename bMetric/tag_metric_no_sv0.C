#define tag_metric_no_sv0_cxx
#include "tag_metric_no_sv0.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void tag_metric_no_sv0::Loop()
{
	//if (fChain == 0) return;

	string tester = "Gets to here...";




	first_pass = true;

	// gDirectory is the working root file.
	// 
	// Get its name as a string for renaming
	// file to be created.
	dir_name = gDirectory->GetName();

	//Cut off .root suffix.
	prefix = dir_name.substr(0, dir_name.size() - 5);

	// Get rid of processtree.
	// gDirectory->Delete("processtree");

	//Set new output file.
	outputfile = new TFile((TString) prefix + "_proc.root", "RECREATE");

	outputtree = new TNtuple("outputtree","Jet Level Tree",
	"jet_pt:jet_truthflav:jet_mv2c20");

	

	fChain->SetBranchStatus("*",0);

	

	fChain->SetBranchStatus("jet_pt",1);
	fChain->SetBranchStatus("jet_sv0_sig3d",1);
	fChain->SetBranchStatus("jet_truthflav",1);
	fChain->SetBranchStatus("jet_mv2c20",1);




	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
     	nbytes += nb;

     	int i = 0;

		// Iteration for each jet using iterator for-loop.
		// Remember, CINT C++ does not support C++11 features.
		for (vector<int>::iterator it = (*jet_truthflav).begin();
			it != (*jet_truthflav).end(); ++it) {

			// Dereference the arrays (* binding) THEN index.

			pt = (*jet_pt)[i];
			truthflav = (*jet_truthflav)[i];
			mv2c20 = (*jet_mv2c20)[i];

			// Filter out jets without a secondary vertex.
			if ((*jet_sv0_sig3d)[i] > -15) {
				outputtree->Fill(pt,truthflav,mv2c20);
			}

			i++;
		}   	
    }


    // Write outputtree in memory to outputfile as key.
    outputfile->Write();

    // Delete all objects left in memory.
    outputfile->Close();


}
