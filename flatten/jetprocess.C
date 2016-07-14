#define jetprocess_cxx
#include "jetprocess.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>
#include <string>

void jetprocess::Loop()
{
//   In a ROOT session, you can do:
//      root> .L jetprocess.C
//      root> jetprocess t
//      root> t.GetEntry(12); // Fill t data members with entry number 12
//      root> t.Show();       // Show values of entry 12
//      root> t.Show(16);     // Read and show values of entry 16
//      root> t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
	//if (fChain == 0) return;

	string tester = "Gets to here...";

	fChain->Print();



	first_pass = true;

	// gDirectory is the working root file.
	// 
	// Get its name as a string for renaming
	// file to be created.
	dir_name = gDirectory->GetName();

	//Cut off .root suffix.
	prefix = dir_name.substr(0, dir_name.size() - 5);

	// Set new output file.
	outputfile = new TFile((TString) prefix + "_proc.root", "RECREATE");

	outputtree = new TNtuple("outputtree","Jet Level Tree",
	"jet_pt:jet_truthflav:jet_sv0_sig3d:jet_sv0_m:jet_sv0_ntrkv:jet_sv0_normdist");

	

	fChain->SetBranchStatus("*",0);

	cout << tester << endl;

	fChain->SetBranchStatus("jet_pt",1);
	fChain->SetBranchStatus("jet_sv0_sig3d",1);
	fChain->SetBranchStatus("jet_truthflav",1);
	fChain->SetBranchStatus("jet_sv0_m",1);
	fChain->SetBranchStatus("jet_sv0_ntrkv",1);
	fChain->SetBranchStatus("jet_sv0_normdist",1);



	Long64_t nentries = fChain->GetEntriesFast();

	for (Long64_t jentry=0; jentry<nentries;jentry++) {
			Long64_t ientry = LoadTree(jentry);
		if (ientry < 0) break;

		if (first_pass) {
    	treename = fChain->GetName();
    	first_pass = false;
    	}
		int i = 0;


		for (auto thisjet : (*jet_truthflav)) {

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

    outputfile->Write();
    outputfile->Close();
}

void jetprocess::BranchSet()
{
	fChain->SetBranchStatus("*",0);

	fChain->SetBranchStatus("jet_pt",1);
	fChain->SetBranchStatus("jet_sv0_sig3d",1);
	fChain->SetBranchStatus("jet_truthflav",1);
	fChain->SetBranchStatus("jet_sv0_m",1);
	fChain->SetBranchStatus("jet_sv0_ntrkv",1);
	fChain->SetBranchStatus("jet_sv0_normdist",1);

}