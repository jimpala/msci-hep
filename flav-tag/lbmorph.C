#define lbmorph_cxx
// The class definition in lbmorph.h has been generated automatically
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
// root> T->Process("lbmorph.C")
// root> T->Process("lbmorph.C","some options")
// root> T->Process("lbmorph.C+")
//


#include "lbmorph.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <typeinfo>
#include <map>
#include <math.h>


// Initializations here.
// Histograms and canvasses.

TH1 *ljethist = nullptr;
TH1 *cjethist = nullptr;
TH1 *bjethist = nullptr;
TH1 *tjethist = nullptr;

TH1 *lsig3hist = nullptr;
TH1 *bsig3hist = nullptr;
TH1 *morphhist = nullptr;
TH1 *lsig3adjusthist = nullptr;
TH1 *bsig3adjusthist = nullptr;

TCanvas *c1 = nullptr;
TCanvas *c2 = nullptr;
TCanvas *c3 = nullptr;
TCanvas *c4 = nullptr;
TCanvas *c5 = nullptr;
TCanvas *c6 = nullptr;
TCanvas *c7 = nullptr;

Double_t l_integral;
Double_t c_integral;
Double_t b_integral;
Double_t t_integral;
Double_t norm_factor;

Double_t lsig3_integral;
Double_t bsig3_integral;
Double_t morph_integral;
Double_t lsig3adjust_integral;
Double_t bsig3adjust_integral;

std::map<double,double> *l_pt_sig3d_pairs = nullptr;
std::map<double,double> *b_pt_sig3d_pairs = nullptr;
std::map<int,double> *l_weight_lookup = nullptr;



void lbmorph::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   // Needs to be set such that errors scale with normalisation.
   TH1::SetDefaultSumw2();

   /*
   Pt FLAVOUR TAGGING HISTS
   */

   ljethist = new TH1D("Jets", "", 100, 0, 200000);
   ljethist->SetMarkerColor(kBlue);
   ljethist->SetLineColor(kBlue);



   cjethist = new TH1D("cjethist", "", 100, 0, 200000);
   cjethist->SetMarkerColor(kMagenta);
   cjethist->SetLineColor(kMagenta);Double_t l_integral = ljethist->Integral(1,100);
   


   bjethist = new TH1D("bjethist", "", 100, 0, 200000);
   bjethist->SetMarkerColor(kRed);
   bjethist->SetLineColor(kRed);
   


   tjethist = new TH1D("tjethist", "", 100, 0, 200000);
   tjethist->SetMarkerColor(kGreen);
   tjethist->SetLineColor(kGreen);
   

   ljethist->GetXaxis()->SetTitle("p_{T} / MeV");
   ljethist->GetYaxis()->SetTitle("n");


   /*
   Sig3 FLAVOURTAGGING HISTS
   */
   lsig3hist = new TH1D("Data", "", 100, -25, 100);
   lsig3hist->SetMarkerColor(kBlue);
   lsig3hist->SetLineColor(kBlue);

   bsig3hist = new TH1D("Sig3b", "Unweighted", 100, -25, 100);
   bsig3hist->SetMarkerColor(kRed);
   bsig3hist->SetLineColor(kRed);

   lsig3hist->GetXaxis()->SetTitle("sv0_sig3d");
   lsig3hist->GetYaxis()->SetTitle("Normalised Frequency");
   bsig3hist->GetXaxis()->SetTitle("sig3");
   bsig3hist->GetYaxis()->SetTitle("n");

   /*
   Weighting HISTS
   */

   morphhist = new TH1D("Morph", "Morph Hist", 100, 0, 200000);

   morphhist->GetXaxis()->SetTitle("Bin");
   morphhist->GetYaxis()->SetTitle("Weight");

   l_pt_sig3d_pairs = new std::map<double,double>();
   b_pt_sig3d_pairs = new std::map<double,double>();
   l_weight_lookup = new std::map<int,double>();

   lsig3adjusthist = new TH1D("Sig3l_adjust", "Weighted", 100, -25, 100);
   lsig3adjusthist->SetMarkerColor(kGreen);
   lsig3adjusthist->SetLineColor(kGreen);

   // bsig3adjusthist = new TH1D("Sig3b_adjust", "Weighted", 100, -25, 100);
   // bsig3adjusthist->SetMarkerColor(kOrange);
   // bsig3adjusthist->SetLineColor(kOrange);





   c1 = new TCanvas("c1", "Canvas 1", 1);
   c2 = new TCanvas("c2", "Canvas 2", 1);
   c3 = new TCanvas("c3", "Canvas 3", 1);
   c4 = new TCanvas("c4", "Canvas 4", 1);
   c5 = new TCanvas("c5", "Canvas 5", 1);
   // c6 = new TCanvas("c6", "Canvas 6", 1);
   c7 = new TCanvas("c7", "Canvas 7", 1500, 1500);

}

void lbmorph::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t lbmorph::Process(Long64_t entry)
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

      // LIGHT TAG
      if (thisjet == 0) {
         ljethist->Fill(jet_pt[i]);
         if (jet_sv0_sig3d[i] > -50) {
            lsig3hist->Fill(jet_sv0_sig3d[i]);
            // std::cout << "[L] sig3d: " << jet_sv0_sig3d[i] << std::endl;

            // Add to map for lookup later.
            l_pt_sig3d_pairs->insert( std::pair<double,double>(jet_pt[i], jet_sv0_sig3d[i]) );
         }
         // else {
         //    std::cout << "[L] sig3d: NO SECONDARY." << std::endl;
         // }
      }


      // CHARM TAG
      if (thisjet == 4) {
         cjethist->Fill(jet_pt[i]);
      }


      // BOTTOM TAG
      if (thisjet == 5) {
         bjethist->Fill(jet_pt[i]);
         if (jet_sv0_sig3d[i] > -50) {
            bsig3hist->Fill(jet_sv0_sig3d[i]);
            // std::cout << "[B] sig3d: " << jet_sv0_sig3d[i] << std::endl;

            // Add to map for lookup later.
            // b_pt_sig3d_pairs->insert( std::pair<double,double>(jet_pt[i], jet_sv0_sig3d[i]) );
         }
         // else {
         //    std::cout << "[B] sig3d: NO SECONDARY." << std::endl;
         // }
      }


      // TAU TAG
      if (thisjet == 15) {
         tjethist->Fill(jet_pt[i]);
      }

      // Increment.
      i++;
   }

   
   return kTRUE;
}

void lbmorph::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void lbmorph::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

   /*
   pT SECTION
   */

   l_integral = ljethist->Integral();
   std::cout << "L integral: " << l_integral << std::endl;
   norm_factor = 1 / l_integral;
   std::cout << "Normalisation factor: " << norm_factor << std::endl;
   ljethist->Scale(norm_factor);

   c_integral = cjethist->Integral();
   norm_factor = 1 / c_integral;
   cjethist->Scale(norm_factor);

   b_integral= bjethist->Integral();
   norm_factor = 1 / b_integral;
   bjethist->Scale(norm_factor);

   t_integral= tjethist->Integral();
   norm_factor = 1 / t_integral;
   tjethist->Scale(norm_factor);

   std::cout << "L jet bins:" << ljethist->GetNbinsX() << std::endl;
   std::cout << "B jet bins:" << bjethist->GetNbinsX() << std::endl;


   /*
   Sig3 SECTION
   */

   lsig3_integral = lsig3hist->Integral();
   norm_factor = 1 / lsig3_integral;
   lsig3hist->Scale(norm_factor);

   bsig3_integral = bsig3hist->Integral();
   norm_factor = 1 / bsig3_integral;
   bsig3hist->Scale(norm_factor);


   /*
   l->b Morph SECTION
   */

   //No normalisation necessary on morphhist.
   morphhist->Divide(bjethist, ljethist);

   // Test pairs.
   for (auto &thispair : *l_pt_sig3d_pairs) {
      std::cout << "pt: " << thispair.first << " ==> " << thispair.second << std::endl;
   }

   // Fill weighted LIGHT and BOTTOM sig3d hist.
   for (auto &thispair : *l_pt_sig3d_pairs) {
      Double_t pt = thispair.first;
      Double_t sig3d = thispair.second;

      // Calculate bin no. corresponding to current pt.
      int bin_no = floor(pt / morphhist->GetBinWidth(1));
      std::cout << "pt: " << pt << endl;
      std::cout << "Bin number: " << bin_no << endl;

      // Exclude bins outside of our range.
      if ( bin_no <= morphhist->GetNbinsX() ) {
         double weight = morphhist->GetBinContent(bin_no);
         Double_t weighted_sig3d = weight * sig3d;
         lsig3adjusthist->Fill(weighted_sig3d);
      }
   }

   // for (auto &thispair : *b_pt_sig3d_pairs) {
   //    Double_t pt = thispair.first;
   //    Double_t sig3d = thispair.second;

   //    int bin_no = floor(pt / morphhist->GetBinWidth(1));

   //    if ( bin_no <= morphhist->GetNbinsX() ) {
   //       double weight = morphhist->GetBinContent(bin_no);
   //       Double_t weighted_sig3d = weight * sig3d;
   //       bsig3adjusthist->Fill(weighted_sig3d);
   //    }
   // }

   // Normalise  these adjusted hists.

   lsig3adjust_integral = lsig3adjusthist->Integral();
   norm_factor = 1 / lsig3adjust_integral;
   lsig3adjusthist->Scale(norm_factor);

   // bsig3adjust_integral = bsig3adjusthist->Integral();
   // norm_factor = 1 / bsig3adjust_integral;
   // bsig3adjusthist->Scale(norm_factor);


   /*
   DRAW STUFF
   */

   c1->cd();
   ljethist->Draw();
   cjethist->Draw("same");
   bjethist->Draw("same");
   tjethist->Draw("same");

   TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
   legend->AddEntry(ljethist,"L jets","l");
   legend->AddEntry(cjethist,"C jets","l");
   legend->AddEntry(bjethist,"B jets","l");
   legend->AddEntry(tjethist,"T jets","l");
   legend->Draw();


   c2->cd();
   lsig3hist->Draw();

   c3->cd();
   bsig3hist->Draw();

   c4->cd();
   morphhist->Draw();

   c5->cd();
   lsig3adjusthist->Draw();

   // c6->cd();
   // bsig3adjusthist->Draw();


   c7->cd();
   lsig3hist->Draw();
   bsig3hist->Draw("same");
   lsig3adjusthist->Draw("same");
   // bsig3adjusthist->Draw("same");

   TLegend *legend2 = new TLegend(0.55,0.65,0.76,0.82);
   legend2->AddEntry(lsig3hist,"L jets (p_{T} unweighted)","l");
   legend2->AddEntry(lsig3adjusthist,"L jets (p_{T} weighted)","l");
   legend2->AddEntry(bsig3hist,"B jets","l");
   // legend2->AddEntry(bsig3adjusthist,"B jets (weighted)","l");
   legend2->Draw();


   /*
   GARBAGE COLLECTION
   */

   delete(l_pt_sig3d_pairs);
   delete(b_pt_sig3d_pairs);


}