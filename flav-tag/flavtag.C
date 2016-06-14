#define flavtag_cxx
// The class definition in flavtag.h has been generated automatically
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
// root> T->Process("flavtag.C")
// root> T->Process("flavtag.C","some options")
// root> T->Process("flavtag.C+")
//


#include "flavtag.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <typeinfo>

// Initializations here.
// Histograms and canvasses.

TH1 *ljethist = nullptr;
TH1 *cjethist = nullptr;
TH1 *bjethist = nullptr;
TH1 *tjethist = nullptr;

TCanvas *c1 = nullptr;


void flavtag::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   ljethist = new TH1D("Jets", "Jet Transverse Momenta Histogram", 100, 0, 200000);
   ljethist->SetMarkerColor(kBlue);
   ljethist->SetLineColor(kBlue);
   cjethist = new TH1D("cjethist", "C Jet Histogram", 100, 0, 200000);
   cjethist->SetMarkerColor(kMagenta);
   cjethist->SetLineColor(kMagenta);
   bjethist = new TH1D("bjethist", "B Jet Histogram", 100, 0, 200000);
   bjethist->SetMarkerColor(kRed);
   bjethist->SetLineColor(kRed);
   tjethist = new TH1D("tjethist", "Tau Jet Histogram", 100, 0, 200000);
   tjethist->SetMarkerColor(kGreen);
   tjethist->SetLineColor(kGreen);

   ljethist->GetXaxis()->SetTitle("pt / MeV");
   ljethist->GetYaxis()->SetTitle("n");

   c1 = new TCanvas("c1", "Canvas 1", 1);


}

void flavtag::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t flavtag::Process(Long64_t entry)
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
   bTag_AntiKt4EMTopoJets->GetEntry(entry);
   
   auto i = 0;

   for (auto thisjet : jet_truthflav) {

      if (thisjet == 0) {
         ljethist->Fill(jet_pt[i]);
      }
      if (thisjet == 4) {
         cjethist->Fill(jet_pt[i]);
      }
      if (thisjet == 5) {
         bjethist->Fill(jet_pt[i]);
      }
      if (thisjet == 15) {
         tjethist->Fill(jet_pt[i]);
      }

      i++;
   }

   
   return kTRUE;
}

void flavtag::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void flavtag::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   c1->cd();
   ljethist->Draw();
   cjethist->Draw("same");
   bjethist->Draw("same");
   tjethist->Draw("same");

   TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
   legend->AddEntry(ljethist,"L jets","f");
   legend->AddEntry(cjethist,"C jets","f");
   legend->AddEntry(bjethist,"B jets","f");
   legend->AddEntry(tjethist,"T jets","f");
   legend->Draw();

}