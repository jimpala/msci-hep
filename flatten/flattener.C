#define flattener_cxx
#include <iostream>
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

TTree *mytree = nullptr;
TString treename;
bool first_pass;

void flattener::Begin(TTree * /*tree*/)
{
   

   TString option = GetOption();
   first_pass = true;

}

void flattener::SlaveBegin(TTree * /*tree*/)
{
   

   TString option = GetOption();

}

Bool_t flattener::Process(Long64_t entry)
{
   // Remember to unpack TTreeReaderValue objects with a preceding *.
   // You can get the tree name via fReader.GetTree()->

   fReader.SetEntry(entry);

   if (first_pass) {
      mytree = fReader.GetTree();
      treename = mytree->GetName();
      first_pass = false;
   }

   cout << *PVx << endl;


   return kTRUE;
}

void flattener::SlaveTerminate()
{
   

}

void flattener::Terminate()
{
   

   cout << treename << endl;

}