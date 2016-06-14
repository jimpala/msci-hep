#define AnalyzeVariablesCut_cxx
// The class definition in AnalyzeVariablesCut.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("AnalyzeVariablesCut.C")
// root> T->Process("AnalyzeVariablesCut.C","some options")
// root> T->Process("AnalyzeVariablesCut.C+")
//


#include "AnalyzeVariablesCut.h"
#include <TH2.h>
#include <TStyle.h>

TH1* pthist = nullptr;
TH1* thetahist = nullptr;
TH2* chithetahist = nullptr;

TCanvas* c1 = nullptr;
TCanvas* c2 = nullptr;
TCanvas* c3 = nullptr;

void AnalyzeVariablesCut::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   
   pthist = new TH1D("pt", "Histogram of pt", 100, 0, 30);
   thetahist = new TH1D("theta", "Histogram of theta", 100, 0, 0.2);
   chithetahist = new TH2D("chitheta", "Scatter of chi2 vs theta", 100, 0.4, 1.6, 100, 0, 0.15);
   chithetahist->GetXaxis()->SetTitle("chi2");
   chithetahist->GetYaxis()->SetTitle("theta");

   c1 = new TCanvas("c1", "Canvas 1", 1);
   c2 = new TCanvas("c2", "Canvas 2", 1);
   c3 = new TCanvas("c3", "Canvas 3", 1);

   TString option = GetOption();
}

void AnalyzeVariablesCut::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t AnalyzeVariablesCut::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);
   tree1->GetEntry(entry);

   // Tree parameters must be dereferenced.
   Double_t pt = TMath::Sqrt((*px) * (*px) + (*py) * (*py));
   pthist->Fill(pt);
   Double_t theta = TMath::ATan2(pt,*pz);
   thetahist->Fill(theta);

   if (*chi2 < 1.5 && theta < 0.15) {
      chithetahist->Fill(*chi2, theta);
   }
   return kTRUE;
}

void AnalyzeVariablesCut::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void AnalyzeVariablesCut::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   c1->cd();
   pthist->Draw();
   c2->cd();
   thetahist->Draw();
   c3->cd();
   chithetahist->Draw();

}