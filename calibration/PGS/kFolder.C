#define kFolder_cxx
#include "kFolder.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <cmath>
#include <algorithm>

void kFolder::Loop()
{
}


/*
Fold(k_val)
PARAMS: k_val - order of kFold.
*/
void kFolder::Fold(int k_val) {

	cout << "brkpt0" << endl;



	cout << "brkpt1" << endl;

	// Create randomised vector of kIndex values
	int nentries = fChain->GetEntriesFast();
	int foldsize = std::ceil(nentries / k_val);

	for (int i=1; i<=k_val; i++) {
		for (int j=1; j<=foldsize; j++) {
			k_assign.push_back(i);
		}
	}
	std::random_shuffle(k_assign.begin(), k_assign.end());

	cout << "brkpt2" <<endl;

	// Initialise new kIndex branch to append.
	int k;
	TBranch* k_branch = fChain->Branch("kIndex",&k, "kIndex/I");

	for (int jentry=0; jentry<nentries; jentry++) {
		k = k_assign.back();
		k_assign.pop_back();
		k_branch->Fill();
	}

	TTree* write_tree = fChain->GetTree();
	write_tree->Write();

}