#define test_cxx
#include "test.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

void test::Loop()
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
	"jet_pt:jet_truthflav:jet_sv0_sig3d:jet_sv0_m:jet_sv0_ntrkv:jet_sv0_normdist");

	

	fChain->SetBranchStatus("*",0);

	

	fChain->SetBranchStatus("jet_pt",1);
	fChain->SetBranchStatus("jet_sv0_sig3d",1);
	fChain->SetBranchStatus("jet_truthflav",1);
	fChain->SetBranchStatus("jet_sv0_m",1);
	fChain->SetBranchStatus("jet_sv0_ntrkv",1);
	fChain->SetBranchStatus("jet_sv0_normdist",1);




	Long64_t nentries = fChain->GetEntriesFast();

	Long64_t nbytes = 0, nb = 0;
	for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;
		nb = fChain->GetEntry(jentry);
     	nbytes += nb;

		if (first_pass) {
    	treename = fChain->GetName();
    	first_pass = false;
    	}
		int i = 0;


		// Iteration for each jet using iterator for-loop.
		// Remember, CINT C++ does not support C++11 features.
		for (vector<int>::iterator it = (*jet_truthflav).begin();
			it != (*jet_truthflav).end(); ++it) {

			// Dereference the arrays (* binding) THEN index.

			pt = (*jet_pt)[i];
			truthflav = (*jet_truthflav)[i];
			sv0_sig3d = (*jet_sv0_sig3d)[i];
			sv0_m = (*jet_sv0_m)[i];
			sv0_ntrkv = (*jet_sv0_ntrkv)[i];
			sv0_normdist = (*jet_sv0_normdist)[i];


			outputtree->Fill(pt,truthflav,sv0_sig3d,
			sv0_m,sv0_ntrkv,sv0_normdist);

			i++;
		}   	
    }


    // Write outputtree in memory to outputfile as key.
    outputfile->Write();

    // Delete all objects left in memory.
    gDirectory->GetList()->Delete();


}
