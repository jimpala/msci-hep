#define Analyze_cxx
// The class definition in Analyze.h has been generated automatically
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
// root> T->Process("Analyze.C")
// root> T->Process("Analyze.C","some options")
// root> T->Process("Analyze.C+")
//


#include "Analyze.h"
#include <TH2.h>
#include <TStyle.h>

// Declarations
TH1* chi2hist = nullptr;
TH1* ebeamhist = nullptr;
TH2* chibeamhist = nullptr;

TCanvas* c1 = nullptr;
TCanvas* c2 = nullptr;
TCanvas* c3 = nullptr;

void Analyze::Begin(TTree *)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).


   // Initializations
   chi2hist = new TH1D("chi2", "Histogram of chi2", 100, 0 ,20);
   chi2hist->GetXaxis()->SetTitle("chi2");
   chi2hist->GetYaxis()->SetTitle("number of events");
   ebeamhist = new TH1D("ebeam", "Histogram of ebeam", 50, 149, 151);
   ebeamhist->GetXaxis()->SetTitle("ebeam");
   ebeamhist->GetYaxis()->SetTitle("number of events");
   chibeamhist = new TH2D("chibeam", "Scatter of chi2 vs beam", 100, 0, 20, 100, 149, 151);
   chibeamhist->GetXaxis()->SetTitle("chi2");
   chibeamhist->GetYaxis()->SetTitle("ebeam");

   c1 = new TCanvas("c1", "Canvas 1", 1);
   c2 = new TCanvas("c2", "Canvas 2", 1);
   c3 = new TCanvas("c3", "Canvas 3", 1);


   TString option = GetOption();
}

void Analyze::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t Analyze::Process(Long64_t entry)
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
   chi2hist->Fill(*chi2);
   ebeamhist->Fill(*ebeam);
   chibeamhist->Fill(*chi2, *ebeam);


   return kTRUE;
}

void Analyze::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void Analyze::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   c1->cd();
   chi2hist->Draw();
   c2->cd();
   ebeamhist->Draw();
   ebeamhist->Fit("gaus");
   gStyle->SetOptFit(0001);
   c3->cd();
   chibeamhist->Draw();
   
}