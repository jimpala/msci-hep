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
  TString ntrees = argv[3];
  TString maxdepth = argv[4];

  bool useAlternative = 0;
  bool useSherpa221 = 1;
  bool useTruthAll = 0;
  bool MVADiboson = 0;
  bool massWindowCut = 0;

  TString nameStr = "temp";
  nameStr += (jetnum==2) ? "_2jet" : "_3jet";
//  nameStr += (i==0) ? "_0of2" : "_1of2";
  nameStr += (i==1) ? "_0of2" : "_1of2";
  nameStr += (useAlternative) ? "_Alternative" : "";
  nameStr += (useTruthAll) ? "_TruthAll" : "";
  nameStr += (MVADiboson) ? "_MVADiboson" : "";
  nameStr += (massWindowCut) ? "_mBBCut" : "";

  float reduction = (useAlternative) ? 0.2 : 1.0;

  std::cout << "Jetnum " << jetnum << std::endl;
  std::cout << "Iteration " << i << std::endl;
  std::cout << "UseAlternative " << useAlternative << std::endl;
  std::cout << "useSherpa221 " << useSherpa221 << std::endl;
  std::cout << "useTruthAll " << useTruthAll << std::endl;
  std::cout << "MVADiboson " << MVADiboson << std::endl;
  std::cout << "massWindowCut " << massWindowCut << std::endl;

//  for(int i=0;i<2;i++){

  TString output_file = (jetnum==2) ? "2jet" : "3jet";
//  TString kFold = (i==0) ? "0of2" : "1of2";
  TString kFold = (i==1) ? "0of2" : "1of2";
  TString tree_string = "_" + ntrees + "ntrees";
  TString depth_string = "_" + maxdepth + "maxdepth";
  output_file += (tree_string + depth_string);

//TMVAClassification_1lep2jet_0of2_BDT_AdaBoost.weights.xml  File name example
//TMVAClassification_1lep2jet_WZ_0of2_BDT_AdaBoost.weights.xml File name example for diboson

  TFile* outputFile = TFile::Open( "TMVAClassification_1lep"+output_file+"_"+kFold+".root", "RECREATE" );
  TMVA::Factory* factory = new TMVA::Factory("TMVAClassification_1lep"+output_file+"_"+kFold, outputFile, "");

//    factory->AddSpectator("EventWeight",   'F');
//    factory->AddSpectator("nJet",          'I');

  TCut jetcut = (jetnum==2) ? "nJets==2" : "nJets==3"; // set the cut to nJet>=2 if you want to use MethodCategory
  TCut btagcut = "nTags==2"; // requiring exactly 2 btagged jets
  TCut pTVcut = "WPt > 150";
  TCut mBBcut = (massWindowCut) ? "jjM < 150 && jjM > 100" : "";

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

  factory->AddVariable("jjdR",           'F');
  factory->AddVariable("jjM",            'F');
  factory->AddVariable("WjjdPhi",        'F');
  factory->AddVariable("ljmindPhi",      'F');
  factory->AddVariable("WPt",            'F');
  factory->AddVariable("j1Pt",           'F');
  factory->AddVariable("j2Pt",           'F');
  factory->AddVariable("WMt",            'F');
  factory->AddVariable("mtop",           'F');
  factory->AddVariable("dYWH",           'F');
  factory->AddVariable("met",            'F');

  // 3jets only

  if(jetnum == 3)
  {
    factory->AddVariable("jjjM",            'F');
    factory->AddVariable("j3Pt",            'F');
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

#include "./../truth.icc"

  if(useSherpa221){
#include "./../truth_Sherpa221.icc"
    if(useTruthAll){
#include "./../direct_truthed_Sherpa221.icc"
    }else{
#include "./../direct_Sherpa221.icc"
    }
  }else{
#include "./../truth_Sherpa220.icc"
    if(useTruthAll){
#include "./../direct_truthed_Sherpa220.icc"
    }else{
#include "./../direct_Sherpa220.icc"
    }
  }

  if(MVADiboson){
    if(useTruthAll){
#include "./../direct_truthed_VZ.icc"
  }else{
#include "./../direct_VZ.icc"
    }
  }else{
    if(useTruthAll){
#include "./../direct_truthed.icc"
    }else{
#include "./../direct.icc"
    } 
  }

  if(useAlternative){
    if(useTruthAll){
#include "./../truth_alt.icc"
    }else{
#include "./../direct_alt.icc"
    }
  }

  TString min = (jetnum==2) ? "0.04" : "0.01";

  std::cout << "The min is " << min << std::endl;

// Set individual event weights (the variable EventWeight must exist as branch in inputs)
  factory->SetWeightExpression("EventWeight");

  factory->PrepareTrainingAndTestTree( "", "",
				       "V:NormMode=NumEvents" );

//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v1","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100:MinNodeSize=0.1:PruneMethod=NoPruning");
//  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v2","IgnoreNegWeightsInTraining:!H:!V:NTrees=200:MaxDepth=4:BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100:MinNodeSize="+min+":PruneMethod=NoPruning");
  factory->BookMethod(TMVA::Types::kBDT, "BDT_AdaBoost_v6","IgnoreNegWeightsInTraining:!H:!V:NTrees="+ntrees+":MaxDepth="+maxdepth+":BoostType=AdaBoost:AdaBoostBeta=0.15:SeparationType=GiniIndex:nCuts=100::MinNodeSize=1%:PruneMethod=NoPruning");

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

  delete factory;
//  }
  return 0;

}
