#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TFile.h>
#include <TChain.h>
#include <TString.h>
#include <TMVA/Factory.h>
#include <string>



/*
Take FIVE arguments.
1: jetnum
2: lowpt
3: kIndex
4: ntrees
5: ncuts
*/
int main(int argc, char * argv[]) {  

// Create ouput file, factory object and open the input file

  int jetnum = std::atoi(argv[1]);

  bool lowpt = ( std::atoi(argv[2]) == 1 );

//  bool tracks = false;
  bool tracks = false;
  bool useMET = false;

  std::cout << "Low pt " << lowpt << std::endl;
  std::cout << "Jetnum " << jetnum << std::endl;


  TString output_file = (jetnum==2) ? "_2jet" : "_3jet";
  TString output_file_pt = (lowpt) ? "_lowpt" : "";
  TString output_file_met = (useMET) ? "_MET" : "";
  TString trackstr = (tracks) ? "_tracks" : "";

  TString ntrees_id = argv[4];
  TString ncuts_id = argv[5];

  TFile* outputFile = TFile::Open( "TMVA"+output_file+output_file_pt+output_file_met+trackstr+ntrees_id+"trees"+ncuts_id+"cuts"+".root", "RECREATE" );
  TMVA::Factory* factory = new TMVA::Factory("tmvaTest"+output_file+output_file_pt+output_file_met+trackstr, outputFile, "");
//  TFile* trainingFile = new TFile("./../../../PGS/outputGeneral_fit_ntuple.mc15_13TeV.410000.DAOD_FTAG2_rescaling_train.root");
//  TFile* testFile = new TFile("./../../../PGS/outputGeneral_fit_ntuple.mc15_13TeV.410000.DAOD_FTAG2_rescaling_test.root");
//  TFile* trainingFile = new TFile("./../../../PGS/outputGeneral_fit_all_rescaling_train.root");
//  TFile* testFile = new TFile("./../../../PGS/outputGeneral_fit_all_rescaling_test.root");


  TString signal = "tree_55";
  TString background_A = "tree_51";
  TString background_B = "tree_11";

  if(jetnum==3){
    signal = "tree_551";
    background_A = "tree_511";
    background_B = "tree_111";
  }

  TChain* sig = new TChain(signal);

  std::vector<std::string> signalFiles;
  signalFiles.push_back("");
  /*
  signalFiles.push_back("SysMCAtNLO");
  signalFiles.push_back("SysPowhegHerwig");
  signalFiles.push_back("SysttbarRadHi");
  signalFiles.push_back("SysttbarRadLo");
  */
  for(int r=0;r<signalFiles.size();r++){
    int entriesToGet = 30000;
    if(tracks) sig->Add(("/unix/atlasvhbb/abell/tmvaTraining/PGS/PGS_tracks/outputGeneral_fit_top"+signalFiles.at(r)+"_rescaling.root").c_str());
    else sig->Add(("/unix/atlasvhbb/abell/tmvaTraining/PGS/outputGeneral_fit_top"+signalFiles.at(r)+"_rescaling.root").c_str());
//      sig->Add(("/unix/atlasvhbb/abell/mc15data7/PGS/outputGeneral_fit_top"+signalFiles.at(r)+"_rescaling.root").c_str(), entriesToGet);
//      std::cout << sig->GetEntries() <<std::endl;
  }
//    TFile* inputFileSignal = new TFile("/unix/atlasvhbb/abell/mc15data7/PGS/outputGeneral_fit_top_all_rescaling.root");
  TFile* inputFile; // = new TFile("/unix/atlasvhbb/abell/mc15data7/PGS/outputGeneral_fit_all_rescaling.root");
  if(tracks) inputFile = TFile::Open("/unix/atlasvhbb/abell/tmvaTraining/PGS/PGS_tracks/outputGeneral_fit_all_rescaling.root");
  else inputFile = TFile::Open("/unix/atlasvhbb/abell/tmvaTraining/PGS/PGS/outputGeneral_fit_all_rescaling.root");

// get the TTree objects from the input files

//    TTree* sig = (TTree*)inputFileSignal->Get(signal);
  TTree* bkg_A = (TTree*)inputFile->Get(background_A);
  TTree* bkg_B = (TTree*)inputFile->Get(background_B);

  outputFile->cd();
// global event weights (see below for setting event-wise weights)

  double sigWeight = 1.0;
  double bkgWeight = 1.0;

//  factory->AddSpectator("weight", 'F');

  TCut cutstring_train = "";
  TCut cutstring_test = "";

  factory->AddSignalTree(sig);

  factory->AddBackgroundTree(bkg_A);
  factory->AddBackgroundTree(bkg_B);

// Define the input variables that shall be used for the MVA training
// (the variables used in the expression must exist in the original TTree).

  factory->SetSignalWeightExpression( "weight" );
  factory->SetBackgroundWeightExpression( "weight" );

  factory->AddVariable("imbalance", 'F');
  factory->AddVariable("leading_pt", 'F');
  factory->AddVariable("sub_leading_pt", 'F');
//     factory->AddVariable("forwardjets", 'I');
  if(jetnum==3) factory->AddVariable("sub_sub_leading_pt", 'F');
  factory->AddVariable("delta_phi", 'F');
  factory->AddVariable("delta_r", 'F');
  factory->AddVariable("mlj", 'F');
  if(useMET) factory->AddVariable("train_met", 'F');

  factory->AddVariable("minl1j", 'F');
  factory->AddVariable("minl2j", 'F');

  factory->AddVariable("kIndex", 'F');

//  factory->AddVariable("train_met", 'F');


  std::string cutstring;

  if(tracks){
    if(lowpt){
cutstring = "sub_leading_pt < 0.02  && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
if(jetnum==3) cutstring = "sub_leading_pt < 0.02 || sub_sub_leading_pt < 0.02 && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
    }
    else {
cutstring = "sub_leading_pt > 0.02 && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
if(jetnum==3) cutstring = "sub_leading_pt > 0.02 && sub_sub_leading_pt > 0.02  && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
    }
  }else{
    if(lowpt){
cutstring = "sub_leading_pt < 0.03  && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
if(jetnum==3) cutstring = "sub_leading_pt < 0.03 || sub_sub_leading_pt < 0.03 && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
    }
    else {
cutstring = "sub_leading_pt > 0.03 && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
if(jetnum==3) cutstring = "sub_leading_pt > 0.03 && sub_sub_leading_pt > 0.03  && (leptontype==0 || (etmiss>60e3 && (mll<80e3 || mll>100e3) && mll>50e3))";
    }
  }

  std::string kcondition = " && kIndex == 6";
  
  cutstring.append(kcondition);
  
  
  TCut mycut = cutstring.c_str();
  TCut nocut = "";


// Tell the factory how to use the training and testing events
  factory->PrepareTrainingAndTestTree(nocut, "NormMode=None:!V" );

  

// Book MVA methods (see TMVA manual).
  TString ntrees = *argv[4];
  TString ncuts = *argv[5];
  TString method_name = "BDTntrees" + ntrees + "nCuts" + ncuts;
  std::cout << "breakpt" << std::endl;
  factory->BookMethod(TMVA::Types::kBDT, method_name ,"IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.10:SeparationType=GiniIndex:nCuts=100:PruneMethod=NoPruning");

// Train, test and evaluate all methods

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();    

// Save the output and finish up

  outputFile->Close();
  std::cout << "==> wrote root file TMVA.root" << std::endl;
  std::cout << "==> TMVAnalysis is done!" << std::endl; 

  exit(0);

}
