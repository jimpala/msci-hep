#define bTag_cxx
#include "bTag.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <iostream>

void bTag::Loop()
{
//   In a ROOT session, you can do:
//      root> .L bTag.C
//      root> bTag t
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
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry < nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry); // WHY not fChain->LoadTree(...)?
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      std::cout << "eventnb = "<< eventnb << " has "<< njets << " njets: "<< std::endl;
      for(int ijet = 0; ijet < njets; ijet ++){
          std::cout << "   jet " << ijet << "-> jet_pt = "<< (*jet_pt)[ijet] << " GeV" <<  std::endl;
      }
   }
}

void bTag::write_jet_ntuple() {

   TFile ofile("bc_jets.root","RECREATE");

   // Get number of entries.
   Long64_t nentries = fChain->GetEntriesFast();

   TNtuple bc_jets("bc_data", "BC Jet Analysis", "Bjets:Cjets");

   Long64_t nbytes = 0, nb = 0;
   
   for (Long64_t jentry=0; jentry < nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;
      Int_t bjets_no = 0;
      Int_t cjets_no = 0;
      for (auto eachjet : *jet_jf_pb) {
         if (eachjet > 0.5){
            bjets_no++;
         }
      }
      for (auto eachjet : *jet_jf_pc) {
         if (eachjet > 0.5){
            cjets_no++;
         }
      }

      bc_jets.Fill(bjets_no, cjets_no);

      std::cout << "Event "<< eventnb << " had "<< bjets_no << " B jets."<< std::endl;
      std::cout << "Event "<< eventnb << " had "<< cjets_no << " C jets."<< std::endl;


      /*
      for (auto eachjet : *jet_jf_pb) {
         std::cout << "Momentum of b jet: " << eachjet << std::endl;
      }
      for (auto eachjet : *jet_jf_pc) {
         std::cout << "Momentum of c jet: " << eachjet << std::endl;
      }
      */

   }

   bc_jets.Write();
   ofile.Close();

}

