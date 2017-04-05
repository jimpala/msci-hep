#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <TFile.h>
#include <TChain.h>
#include <TString.h>
#include <TMVA/Factory.h>

int main(int argc, char * argv[]) {  

// Create ouput file, factory object and open the input file

  int jetnum = std::atoi(argv[1]);
  int i = std::atoi(argv[2]);

  bool useAlternative = (std::atoi(argv[3])==1);
  bool useSherpa221 = true;
//  bool useSherpa221 = (std::atoi(argv[4])==1);
  bool useTruthAll = (std::atoi(argv[4])==1);
  bool MVADiboson = false;
  bool massWindowCut = false;
  bool useWeights = false;// (std::atoi(argv[5])==1);

  TString nameStr = "temp";
  nameStr += (jetnum==2) ? "_2jet" : "_3jet";
//  nameStr += (i==0) ? "_0of2" : "_1of2";
  nameStr += (i==1) ? "_0of2" : "_1of2";
  nameStr += (useAlternative) ? "_Alternative" : "";
  nameStr += (useTruthAll) ? "_TruthAll" : "";
  nameStr += (massWindowCut) ? "_mBBCut" : "";
  nameStr += (useWeights) ? "_correctWeight" : "";

  float reduction = (useAlternative) ? 0.2 : 1.0;

  std::cout << "Jetnum " << jetnum << std::endl;
  std::cout << "Iteration " << i << std::endl;
  std::cout << "UseAlternative " << useAlternative << std::endl;
  std::cout << "useSherpa221 " << useSherpa221 << std::endl;
  std::cout << "useTruthAll " << useTruthAll << std::endl;
  std::cout << "MVADiboson " << MVADiboson << std::endl;
  std::cout << "massWindowCut " << massWindowCut << std::endl;
  std::cout << "useWeights " << useWeights << std::endl;

//  for(int i=0;i<2;i++){

  TString nJet = (jetnum==2) ? "2jet" : "3jet";
  TString output_file;
//  TString kFold = (i==0) ? "0of2" : "1of2";
  TString kFold = (i==1) ? "0of2" : "1of2";
  TString version = "_v9";
  if(useAlternative) output_file+="_SystTTbar";
  if(MVADiboson) output_file+="_WZ";
  if(useSherpa221) output_file+="_Sherpa221";
  if(!useSherpa221) output_file+="_Sherpa220";
  if(useTruthAll) output_file+= "_TruthAll";
  if(massWindowCut) output_file+= "_mBBCut";
  if(useWeights) output_file+= "_correctWeights";

//TMVAClassification_1lep2jet_0of2_BDT_AdaBoost.weights.xml  File name example
//TMVAClassification_1lep2jet_WZ_0of2_BDT_AdaBoost.weights.xml File name example for diboson

  TFile* outputFile = TFile::Open( "TMVAClassification_1lep"+nJet+"_"+kFold+output_file+".root", "RECREATE" );
  TMVA::Factory* factory = new TMVA::Factory("TMVAClassification_1lep"+nJet+"_"+kFold+output_file, outputFile, "");

//    factory->AddSpectator("EventWeight",   'F');
//    factory->AddSpectator("nJet",          'I');

  TCut jetcut = (jetnum==2) ? "nJ==2" : "nJ==3"; // set the cut to nJet>=2 if you want to use MethodCategory
  TCut btagcut = "nTags==2"; // requiring exactly 2 btagged jets
  TCut pTVcut = "pTV > 150e3";
//  TCut Rangecuts = "pTB1 < 350 && MET < 320";
  TCut mBBcut = (massWindowCut) ? "mBB < 150 && mBB > 100" : "";

  TCut mycut_train = (i==0) ? "((EventNumber%2)==0)" : "((EventNumber%2)!=0)";
  TCut mycut_test = (i!=0) ? "((EventNumber%2)==0)" : "((EventNumber%2)!=0)";


  TCut mycut_train_signal = (i==0) ? "((EventNumber%2)==0)" : "((EventNumber%2)!=0)";
  TCut mycut_test_signal = (i!=0) ? "((EventNumber%2)==0) " : "((EventNumber%2)!=0)";
//  TCut mycut_train_signal = (i==0) ? "((EventNumber%2)==0) && j1Flav==5 && j2Flav==5" : "((EventNumber%2)!=0) && j1Flav==5 && j2Flav==5";
//  TCut mycut_test_signal = (i!=0) ? "((EventNumber%2)==0) && j1Flav==5 && j2Flav==5" : "((EventNumber%2)!=0) && j1Flav==5 && j2Flav==5";

  mycut_train = mycut_train + jetcut + btagcut + pTVcut + mBBcut;
  mycut_test = mycut_test + jetcut + btagcut + pTVcut + mBBcut;
  mycut_train_signal = mycut_train_signal + jetcut + btagcut + pTVcut + mBBcut;
  mycut_test_signal = mycut_test_signal + jetcut + btagcut + pTVcut + mBBcut;

  std::cout << "mycut_train: " << mycut_train << std::endl;
  std::cout << "mycut_test: " << mycut_test << std::endl;
  std::cout << "mycut_train_signal: " << mycut_train_signal << std::endl;
  std::cout << "mycut_test_signal: " << mycut_test_signal << std::endl;

  // Define the input variables that shall be used for the MVA training

  factory->AddVariable("dRBB",           'F');
  factory->AddVariable("mBB",        'F');
  factory->AddVariable("dPhiVBB",        'F');
  factory->AddVariable("dPhiLBmin",      'F');
  factory->AddVariable("pTV",            'F');
  factory->AddVariable("pTB1",       'F');
  factory->AddVariable("pTB2",       'F');
  factory->AddVariable("mTW",            'F');
  factory->AddVariable("Mtop",           'F');
  factory->AddVariable("dYWH",           'F');
  factory->AddVariable("MET",        'F');

  // 3jets only

  if(jetnum == 3)
  {
    factory->AddVariable("mBBJ",            'F');
    factory->AddVariable("pTJ3",            'F');
  }
/*

  factory->AddVariable("jjdR","dRBB",           'F');
  factory->AddVariable("jjM","mBB",             'F');
  factory->AddVariable("WjjdPhi","dPhiVBB",     'F');
  factory->AddVariable("ljmindPhi","dPhiLBmin", 'F');
  factory->AddVariable("WPt","pTV",             'F');
  factory->AddVariable("j1Pt","pTB1",           'F');
  factory->AddVariable("j2Pt","pTB2",           'F');
  factory->AddVariable("WMt","mTW",             'F');
  factory->AddVariable("mtop","Mtop",           'F');
  factory->AddVariable("dYWH","dYWH",           'F');
  factory->AddVariable("met","MET",             'F');

  // 3jets only

  if(jetnum == 3)
  {
  factory->AddVariable("jjjM","mBBJ",            'F');
  factory->AddVariable("j3Pt","pTJ3",            'F');
  }
*/
  // cross sections were already taken into account in the framework
  Double_t signalWeight     = 1.0;
  Double_t backgroundWeight = 1.0;
  int threshold = 10; // missing statistic can result in empty categories for some sample, and inexisting branches

//#include "./../direct_test_truthed.icc"
/*
  TFile *f_singletop_s_196        = new TFile("/unix/atlas3/abell/CxAODFramework_trunk_191016_SVN/FullSyst_nJET/batchSubmit-01-11-16_09-42-39/SingleTop_subjob0/singletop_s/submitDir/data-MVATree/singletop_s.root","READ" );
  TTree *t_singletop_s_196 = (TTree*)f_singletop_s_196->GetObjectChecked("Nominal", "TTree");
  TFile *f_singletop_s_196_temp = TFile::Open("output_f_singletop_s_196"+nameStr+".root","RECREATE");
  TTree* t_clone=t_singletop_s_196->CloneTree(0);
  if(t_singletop_s_196->GetEntries() > threshold){
  factory->AddTree(t_singletop_s_196,"Background",backgroundWeight,mycut_train,"train");
  factory->AddTree(t_singletop_s_196,"Background",backgroundWeight,mycut_test,"test");
  }

*/

  if(useWeights){

    float ttbarWeight = (jetnum==2) ? 0.85 : 0.863;
      float wjetWeight = (jetnum==2) ? 1.58: 1.50;

    if(useTruthAll&&!useAlternative){

      TFile *f_Background_Direct_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//background_Truthed_nottbar_nowjets.root",        "READ");
      TTree *t_Background_Direct_truthed = (TTree*)f_Background_Direct_truthed->Get("Nominal");
      TFile *f_Background_Direct_truthed_temp = TFile::Open("Background_Direct_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_temp=t_Background_Direct_truthed->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_test,"test");
      }


      TFile *f_Background_Direct_truthed_ttbar        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/ttbar_Direct_truthed.root",        "READ");
      TTree *t_Background_Direct_truthed_ttbar = (TTree*)f_Background_Direct_truthed_ttbar->Get("Nominal");
      TFile *f_Background_Direct_truthed_ttbar_temp = TFile::Open("Background_Direct_truthed_ttbar_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_ttbar_temp=t_Background_Direct_truthed_ttbar->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed_ttbar,"Background",backgroundWeight*ttbarWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed_ttbar,"Background",backgroundWeight*ttbarWeight,mycut_test,"test");
      }

      TFile *f_Background_Direct_truthed_WBjet        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//WBjet_Direct_truthed.root",        "READ");
      TTree *t_Background_Direct_truthed_WBjet = (TTree*)f_Background_Direct_truthed_WBjet->Get("Nominal");
      TFile *f_Background_Direct_truthed_WBjet_temp = TFile::Open("Background_Direct_truthed_WBjet_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_WBJet_temp=t_Background_Direct_truthed_WBjet->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed_WBjet,"Background",backgroundWeight*wjetWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed_WBjet,"Background",backgroundWeight*wjetWeight,mycut_test,"test");
      }

      TFile *f_Background_Direct_truthed_WCLjet        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//WCLjet_Direct_truthed.root",        "READ");
      TTree *t_Background_Direct_truthed_WCLjet = (TTree*)f_Background_Direct_truthed_WCLjet->Get("Nominal");
      TFile *f_Background_Direct_truthed_WCLjet_temp = TFile::Open("Background_Direct_truthed_WCLjet_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_WCLJet_temp=t_Background_Direct_truthed_WCLjet->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed_WCLjet,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed_WCLjet,"Background",backgroundWeight,mycut_test,"test");
      }


      TFile *f_Signal_Truth        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Truth_Signal.root",        "READ");
      TTree *t_Signal_Truth = (TTree*)f_Signal_Truth->Get("Nominal");
      TFile *f_Signal_Truth_temp = TFile::Open("Signal_Truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Truth_temp=t_Signal_Truth->CloneTree(0);
      if(t_Signal_Truth->GetEntries() > threshold){
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_test,"test");
      }
    }

    else if(useTruthAll&&useAlternative){

      TFile *f_Background_Direct_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_nottbar_nowjets.root",        "READ");
      TTree *t_Background_Direct_truthed = (TTree*)f_Background_Direct_truthed->Get("Nominal");
      TFile *f_Background_Direct_truthed_temp = TFile::Open("Background_Direct_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_temp=t_Background_Direct_truthed->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_test,"test");
      }


      TFile *f_Background_ttbarSyst_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_ttbar.root",        "READ");
      TTree *t_Background_ttbarSyst_truthed = (TTree*)f_Background_ttbarSyst_truthed->Get("Nominal");
      TFile *f_Background_ttbarSyst_truthed_temp = TFile::Open("Background_ttbarSyst_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_ttbarSyst_truthed_temp=t_Background_ttbarSyst_truthed->CloneTree(0);
      if(t_Background_ttbarSyst_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_ttbarSyst_truthed,"Background",backgroundWeight*0.2*ttbarWeight,mycut_train,"train");
	factory->AddTree(t_Background_ttbarSyst_truthed,"Background",backgroundWeight*0.2*ttbarWeight,mycut_test,"test");
      }


      TFile *f_Background_Wjets_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_Wjets.root",        "READ");
      TTree *t_Background_Wjets_truthed = (TTree*)f_Background_Wjets_truthed->Get("Nominal");
      TFile *f_Background_Wjets_truthed_temp = TFile::Open("Background_Wjets_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Wjets_truthed_temp=t_Background_Wjets_truthed->CloneTree(0);
      if(t_Background_Wjets_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Wjets_truthed,"Background",backgroundWeight*0.5*wjetWeight,mycut_train,"train");
	factory->AddTree(t_Background_Wjets_truthed,"Background",backgroundWeight*0.5*wjetWeight,mycut_test,"test");
      }

      TFile *f_Signal_Truth        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Truth_Signal.root",        "READ");
      TTree *t_Signal_Truth = (TTree*)f_Signal_Truth->Get("Nominal");
      TFile *f_Signal_Truth_temp = TFile::Open("Signal_Truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Truth_temp=t_Signal_Truth->CloneTree(0);
      if(t_Signal_Truth->GetEntries() > threshold){
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_test,"test");
      }
    }


    else if(!useTruthAll){

      TFile *f_Background_Normal        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//background_Normal_nottbar_noWBjet.root",        "READ");
      TTree *t_Background_Normal = (TTree*)f_Background_Normal->Get("Nominal");
      TFile *f_Background_Normal_temp = TFile::Open("Background_Normal_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Normal_temp=t_Background_Normal->CloneTree(0);
      if(t_Background_Normal->GetEntries() > threshold){
	factory->AddTree(t_Background_Normal,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Normal,"Background",backgroundWeight,mycut_test,"test");
      }

      TFile *f_Background_Normal_ttbar        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/ttbar_Direct.root",        "READ");
      TTree *t_Background_Normal_ttbar = (TTree*)f_Background_Normal_ttbar->Get("Nominal");
      TFile *f_Background_Normal_ttbar_temp = TFile::Open("Background_Normal_ttbar_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Normal_ttbar_temp=t_Background_Normal_ttbar->CloneTree(0);
      if(t_Background_Normal_ttbar->GetEntries() > threshold){
	factory->AddTree(t_Background_Normal_ttbar,"Background",backgroundWeight*ttbarWeight,mycut_train,"train");
	factory->AddTree(t_Background_Normal_ttbar,"Background",backgroundWeight*ttbarWeight,mycut_test,"test");
      }

      TFile *f_Background_Normal_Wjet        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//WBjet_Direct.root",        "READ");
      TTree *t_Background_Normal_Wjet = (TTree*)f_Background_Normal_Wjet->Get("Nominal");
      TFile *f_Background_Normal_Wjet_temp = TFile::Open("Background_Normal_Wjet_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Normal_Wjet_temp=t_Background_Normal_Wjet->CloneTree(0);
      if(t_Background_Normal_Wjet->GetEntries() > threshold){
	factory->AddTree(t_Background_Normal_Wjet,"Background",backgroundWeight*wjetWeight,mycut_train,"train");
	factory->AddTree(t_Background_Normal_Wjet,"Background",backgroundWeight*wjetWeight,mycut_test,"test");
      }



      TFile *f_Signal_Direct        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Direct_Signal.root",        "READ");
      TTree *t_Signal_Direct = (TTree*)f_Signal_Direct->Get("Nominal");
      TFile *f_Signal_Direct_temp = TFile::Open("Signal_Directed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Direct_temp=t_Signal_Direct->CloneTree(0);
      if(t_Signal_Direct->GetEntries() > threshold){
	factory->AddTree(t_Signal_Direct,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Direct,"Signal",signalWeight,mycut_test,"test");
      }
    }

  }
  else{


    if(useTruthAll&&!useAlternative){

      TFile *f_Background_Direct_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3//background_Truthed.root",        "READ");
      TTree *t_Background_Direct_truthed = (TTree*)f_Background_Direct_truthed->Get("Nominal");
      TFile *f_Background_Direct_truthed_temp = TFile::Open("Background_Direct_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_temp=t_Background_Direct_truthed->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_test,"test");
      }

      TFile *f_Signal_Truth        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Truth_Signal.root",        "READ");
      TTree *t_Signal_Truth = (TTree*)f_Signal_Truth->Get("Nominal");
      TFile *f_Signal_Truth_temp = TFile::Open("Signal_Truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Truth_temp=t_Signal_Truth->CloneTree(0);
      if(t_Signal_Truth->GetEntries() > threshold){
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_test,"test");
      }
    }

    else if(useTruthAll&&useAlternative){

      TFile *f_Background_Direct_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_nottbar_nowjets.root",        "READ");
      TTree *t_Background_Direct_truthed = (TTree*)f_Background_Direct_truthed->Get("Nominal");
      TFile *f_Background_Direct_truthed_temp = TFile::Open("Background_Direct_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Direct_truthed_temp=t_Background_Direct_truthed->CloneTree(0);
      if(t_Background_Direct_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Direct_truthed,"Background",backgroundWeight,mycut_test,"test");
      }


      TFile *f_Background_ttbarSyst_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_ttbar.root",        "READ");
      TTree *t_Background_ttbarSyst_truthed = (TTree*)f_Background_ttbarSyst_truthed->Get("Nominal");
      TFile *f_Background_ttbarSyst_truthed_temp = TFile::Open("Background_ttbarSyst_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_ttbarSyst_truthed_temp=t_Background_ttbarSyst_truthed->CloneTree(0);
      if(t_Background_ttbarSyst_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_ttbarSyst_truthed,"Background",backgroundWeight*0.2,mycut_train,"train");
	factory->AddTree(t_Background_ttbarSyst_truthed,"Background",backgroundWeight*0.2,mycut_test,"test");
      }


      TFile *f_Background_Wjets_truthed        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Truthed_Wjets.root",        "READ");
      TTree *t_Background_Wjets_truthed = (TTree*)f_Background_Wjets_truthed->Get("Nominal");
      TFile *f_Background_Wjets_truthed_temp = TFile::Open("Background_Wjets_truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Wjets_truthed_temp=t_Background_Wjets_truthed->CloneTree(0);
      if(t_Background_Wjets_truthed->GetEntries() > threshold){
	factory->AddTree(t_Background_Wjets_truthed,"Background",backgroundWeight*0.5,mycut_train,"train");
	factory->AddTree(t_Background_Wjets_truthed,"Background",backgroundWeight*0.5,mycut_test,"test");
      }

      TFile *f_Signal_Truth        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Truth_Signal.root",        "READ");
      TTree *t_Signal_Truth = (TTree*)f_Signal_Truth->Get("Nominal");
      TFile *f_Signal_Truth_temp = TFile::Open("Signal_Truthed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Truth_temp=t_Signal_Truth->CloneTree(0);
      if(t_Signal_Truth->GetEntries() > threshold){
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Truth,"Signal",signalWeight,mycut_test,"test");
      }
    }


    else if(!useTruthAll){

      TFile *f_Background_Normal        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Normal.root",        "READ");
      TTree *t_Background_Normal = (TTree*)f_Background_Normal->Get("Nominal");
      TFile *f_Background_Normal_temp = TFile::Open("Background_Normal_"+nameStr+"temp.root","RECREATE");
      TTree* t_clone_Background_Normal_temp=t_Background_Normal->CloneTree(0);
      if(t_Background_Normal->GetEntries() > threshold){
	factory->AddTree(t_Background_Normal,"Background",backgroundWeight,mycut_train,"train");
	factory->AddTree(t_Background_Normal,"Background",backgroundWeight,mycut_test,"test");
      }

      TFile *f_Signal_Direct        = new TFile("/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Direct_Signal.root",        "READ");
      TTree *t_Signal_Direct = (TTree*)f_Signal_Direct->Get("Nominal");
      TFile *f_Signal_Direct_temp = TFile::Open("Signal_Directed_"+nameStr+"temp.root","RECREATE");
      TTree* t_Signal_Direct_temp=t_Signal_Direct->CloneTree(0);
      if(t_Signal_Direct->GetEntries() > threshold){
	factory->AddTree(t_Signal_Direct,"Signal",signalWeight,mycut_train,"train");
	factory->AddTree(t_Signal_Direct,"Signal",signalWeight,mycut_test,"test");
      }
    }
  }

  TString min_v6 = (useTruthAll) ? "0.1" : "1";
  if(useTruthAll&&useAlternative) min_v6= "0.05";

  TString min_v8 = (useTruthAll) ? "0.5" : "1";
  if(useTruthAll&&useAlternative) min_v8 = "0.1";


//  TString min = (jetnum==2) ? "0.04" : "0.01";

//  std::cout << "The min is " << min << std::endl;

// Set individual event weights (the variable EventWeight must exist as branch in inputs)
  factory->SetWeightExpression("EventWeight");

  factory->PrepareTrainingAndTestTree( "", "", "V:NormMode=NumEvents" );

//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v1","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100:MinNodeSize=0.1:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v2","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100:MinNodeSize="+min+":PruneMethod=NoPruning");

//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v5","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=5%:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v6","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v6+"%:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v7","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=1%:PruneMethod=NoPruning");
/*
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9","IgnoreNegWeightsInTraining:!H:!V:NTrees=250:MaxDepth=7:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9_a","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9_b","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");



  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9_c","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9_d","IgnoreNegWeightsInTraining:!H:!V:NTrees=600:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v9_e","IgnoreNegWeightsInTraining:!H:!V:NTrees=700:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

/*
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v10","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=7:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v12","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=6:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v13","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v14","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=6:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v15","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=8:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
*/
  //factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v16_b","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
/*
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v16","IgnoreNegWeightsInTraining:!H:!V:NTrees=150:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v17","IgnoreNegWeightsInTraining:!H:!V:NTrees=150:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v18","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v19","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

*/

//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v11","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize="+min_v8+"%:PruneMethod=NoPruning");

  // JIMBO MVAs
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_2depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_3depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_4depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_5depth_0.03minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=3%:PruneMethod=NoPruning");

//------------------------------------------

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_2depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=2:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_3depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=3:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_4depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");

  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_100ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=100:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_120ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=120:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_140ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=140:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_160ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=160:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_180ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=180:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_200ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_220ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=220:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_240ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=240:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_260ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=260:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_280ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=280:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_300ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=300:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_320ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=320:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_340ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=340:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_360ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=360:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_380ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=380:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_400ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=400:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_420ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=420:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_440ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=440:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_460ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=460:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_480ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=480:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_scan_500ntree_5depth_0.04minnode","IgnoreNegWeightsInTraining:!H:!V:NTrees=500:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=4%:PruneMethod=NoPruning");




/*
  factory->BookMethod( TMVA::Types::kBDT, "BDTD"+version,
  "!H:!V:NTrees=400:MinNodeSize=5%:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:VarTransform=Decorrelate" );

  factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN"+version, "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  );

  factory->BookMethod( TMVA::Types::kSVM, "SVM"+version, "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
*/

  factory->TrainAllMethods();
  factory->TestAllMethods();
  factory->EvaluateAllMethods();    

// Save the output and finish up

  outputFile->Close();
  std::cout << "==> wrote root file TMVA.root" << std::endl;
  std::cout << "==> TMVAnalysis is done!" << std::endl; 

//  delete factory;
//  }
  return 0;

}
