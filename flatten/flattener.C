#define flattener_cxx
#include <iostream>
#include <string>
// The class definition in flattener.h has been generated automatically
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
// root> T->Process("flattener.C")
// root> T->Process("flattener.C","some options")
// root> T->Process("flattener.C+")
//

#include "flattener.h"
#include <TH2.h>
#include <TStyle.h>




// INITIALISATIONS //

// We'll use a TNtuple instead of a TTree.
// TNtuples are a bit easier to set up as they
// only store doubles.
TNtuple *outputtree = nullptr;
TFile *outputfile = nullptr;

// This is for getting the tree name.
// It isn't actually needed as fReader
// does all the work.
TTree *mytree = nullptr;
TString treename;
bool first_pass;

std::string dir_name;
std::string prefix;

// Tree params of interest
double pt;
double truthflav;
double sv0_sig3d;
double sv0_m;
double sv0_ntrkv;
double sv0_normdist;



void flattener::Begin(TTree * /*tree*/)
{
   TString option = GetOption();

   first_pass = true;

   // gDirectory is the working root file.
   // 
   // Get its name as a string for renaming
   // file to be created.
   dir_name = gDirectory->GetName();

   // Cut off .root suffix.
   prefix = dir_name.substr(0, dir_name.size() - 5);


   outputfile = new TFile((TString) prefix + "_proc.root", "RECREATE");

   outputtree = new TNtuple("outputtree","Jet Level Tree",
   "jet_pt:jet_truthflav:jet_sv0_sig3d:jet_sv0_m:jet_sv0_ntrkv:jet_sv0_normdist");

}

void flattener::SlaveBegin(TTree * /*tree*/)
{
   

   TString option = GetOption();

}

Bool_t flattener::Process(Long64_t entry)
{
   // Remember to unpack TTreeReaderValue objects with a preceding *.
   // TTreeReaderValueArray doesn't need this - just index.

   fReader.SetEntry(entry);

   if (first_pass) {
      mytree = fReader.GetTree();
      treename = mytree->GetName();

      

      first_pass = false;
   }

   int i = 0;
   for (auto thisjet : jet_truthflav) {

   pt = jet_pt[i];
   truthflav = jet_truthflav[i];
   sv0_sig3d = jet_sv0_sig3d[i];
   sv0_m = jet_sv0_m[i];
   sv0_ntrkv = jet_sv0_ntrkv[i];
   sv0_normdist = jet_sv0_normdist[i];

   outputtree->Fill(pt,truthflav,sv0_sig3d,
   sv0_m,sv0_ntrkv,sv0_normdist);

   i++;
   }

   return kTRUE;
}

void flattener::SlaveTerminate()
{
   

}

void flattener::Terminate()
{

   outputfile->Write();
   outputfile->Close();

}


