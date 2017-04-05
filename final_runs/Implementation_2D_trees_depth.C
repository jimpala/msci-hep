#include <string>
#include <iostream>
#include <sstream>
#include <dirent.h>
#include <TMath.h>
#include <TMVA/Reader.h>
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TStyle.h"
#include <fstream>

using namespace std;

TChain* myT_Background;
TChain* myT_Signal;


void Transform(TH1F* back, TH1F* sig, float& sensitivity, float& error){

  float back_int = back->Integral(0,back->GetNbinsX()+1);
  float sig_int = sig->Integral(0,sig->GetNbinsX()+1);

  int EndBin = back->GetNbinsX()+1;
  int z_s=10;
  int z_b=5;
  float z=0;

  float n_s_total=0;
  float n_b_total=0;

  for(int i=EndBin;i>=0;i--){
    double delta_s = 0;
    double delta_b = 0;
    float n_s = sig->IntegralAndError(i,EndBin,delta_s);
    float n_b = back->IntegralAndError(i,EndBin,delta_b);

    if(n_s > 0 && n_b >0){

      z = ( z_b * n_b / back_int ) + ( z_s * n_s / sig_int );
//    std::cout << "At bin i = " << i << " z = " << z << std::endl;

      if( z>1 || i==0 ){
	EndBin = i - 1;
      }else{
	continue;
      }

      n_s_total+=n_s;
      n_b_total+=n_b;

      float dSens = (2 * ((n_s+n_b)*log(1+(n_s/n_b))-n_s));

      float dSens_ds = 2 * log(1 + (n_s / n_b));
      float dSens_db = 2 * (log(1 + (n_s / n_b)) - (n_s / n_b));

      float dError = pow(dSens_ds * delta_s,2) + pow(dSens_db * delta_b,2);

/*
      std::cout << "n_s " << n_s << std::endl;
      std::cout << "n_b " << n_b << std::endl;
      std::cout << "delta_s " << delta_s << std::endl;
      std::cout << "delta_b " << delta_b << std::endl;
      std::cout << "dSens_ds " << dSens_ds << std::endl;
      std::cout << "dSens_db " << dSens_db << std::endl;
      std::cout << "dSens " << dSens << std::endl;
      std::cout << "dError " << dError << std::endl;
*/
      if(dSens==dSens)  sensitivity += (2 * ((n_s+n_b)*log(1+n_s/n_b)-n_s));
      if(dError==dError)  error += dError;
/*
  std::cout << pow(sensitivity,0.5) << std::endl;
  std::cout << n_s_total << std::endl;
  std::cout << n_b_total << std::endl;
*/
    }
  }
//  error = error / sensitivity;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Implementation(){
  gStyle->SetOptStat(0);

  ofstream myfile;
  TString openfile = "sensitivities.txt";
  myfile.open (openfile);

  bool usingV2=false;
  bool MVADiboson=false;

  std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "DON'T FORGET THE MASSIVE BUG YOU'VE PUT IN WITH .AT(0)!!!!" << std::endl;

  std::vector<TString> mvaNames;
  mvaNames.push_back("0905_BDT_AdaBoost");


  mvaNames.push_back("BDT_scan_100ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_120ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_140ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_160ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_180ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_200ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_220ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_240ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_260ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_280ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_300ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_320ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_340ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_360ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_380ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_400ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_420ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_440ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_460ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_480ntree_2depth_0.01minnode");
  mvaNames.push_back("BDT_scan_500ntree_2depth_0.01minnode");

  mvaNames.push_back("BDT_scan_100ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_120ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_140ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_160ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_180ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_200ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_220ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_240ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_260ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_280ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_300ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_320ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_340ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_360ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_380ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_400ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_420ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_440ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_460ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_480ntree_3depth_0.01minnode");
  mvaNames.push_back("BDT_scan_500ntree_3depth_0.01minnode");

  mvaNames.push_back("BDT_scan_100ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_120ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_140ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_160ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_180ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_200ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_220ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_240ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_260ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_280ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_300ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_320ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_340ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_360ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_380ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_400ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_420ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_440ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_460ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_480ntree_4depth_0.01minnode");
  mvaNames.push_back("BDT_scan_500ntree_4depth_0.01minnode");

  mvaNames.push_back("BDT_scan_100ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_120ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_140ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_160ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_180ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_200ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_220ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_240ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_260ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_280ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_300ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_320ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_340ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_360ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_380ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_400ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_420ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_440ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_460ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_480ntree_5depth_0.01minnode");
  mvaNames.push_back("BDT_scan_500ntree_5depth_0.01minnode");

/*
//  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_a");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_b");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_c");
//  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_c_unit");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_d");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v9_e");

mvaNames.push_back("Sherpa221_BDT_AdaBoost_v16");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v16_a_RENAMED");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v16_b");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v6");
mvaNames.push_back("Sherpa221_TruthAll_BDT_AdaBoost_v6_RENAMED");
mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_BDT_AdaBoost_v6_RENAMED");

mvaNames.push_back("Sherpa221_correctWeights_BDT_AdaBoost_v6_RENAMED");

/*
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v10");
//  mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_correctWeights_BDT_AdaBoost_v10");
mvaNames.push_back("Sherpa221_mBBCut_BDT_AdaBoost_v11");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v12");
mvaNames.push_back("Sherpa221_BDT_AdaBoost_v13");
*/
/*
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v17");
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v18");
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v19");
*/
/*
  mvaNames.push_back("Sherpa221_TruthAll_BDT_AdaBoost_v5");
  mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_BDT_AdaBoost_v5");
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v5");
  mvaNames.push_back("Sherpa221_TruthAll_BDT_AdaBoost_v6");
  mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_BDT_AdaBoost_v6");
*/
/*
  mvaNames.push_back("Sherpa221_TruthAll_BDT_AdaBoost_v7");
  mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_BDT_AdaBoost_v7");
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v7");
  mvaNames.push_back("Sherpa221_TruthAll_BDT_AdaBoost_v8");
  mvaNames.push_back("SystTTbar_Sherpa221_TruthAll_BDT_AdaBoost_v8");
  mvaNames.push_back("Sherpa221_BDT_AdaBoost_v8");
*/
  TMVA::Reader* reader_2jet = new TMVA::Reader();
//  TMVA::Reader* reader_2jet_ICHEP = new TMVA::Reader();
  TMVA::Reader* reader_3jet = new TMVA::Reader();
//  TMVA::Reader* reader_3jet_ICHEP = new TMVA::Reader();
  float dRBB,mBB,dPhiVBB,dPhiLBmin,pTV,pTB1,pTB2,mTW,Mtop,dYWH,MET,mBBJ,pTJ3;
  float EventWeight, BDT;
  int nTags, nJ;
  unsigned long long EventNumber;
  std::string *sample = 0;

  reader_2jet->AddVariable("dRBB", &dRBB);
  reader_2jet->AddVariable("mBB", &mBB);
  reader_2jet->AddVariable("dPhiVBB", &dPhiVBB);
  reader_2jet->AddVariable("dPhiLBmin", &dPhiLBmin);
  reader_2jet->AddVariable("pTV", &pTV);
  reader_2jet->AddVariable("pTB1", &pTB1);
  reader_2jet->AddVariable("pTB2", &pTB2);
  reader_2jet->AddVariable("mTW", &mTW);
  reader_2jet->AddVariable("Mtop", &Mtop);
  reader_2jet->AddVariable("dYWH", &dYWH);
  reader_2jet->AddVariable("MET", &MET);

  reader_3jet->AddVariable("dRBB", &dRBB);
  reader_3jet->AddVariable("mBB", &mBB);
  reader_3jet->AddVariable("dPhiVBB", &dPhiVBB);
  reader_3jet->AddVariable("dPhiLBmin", &dPhiLBmin);
  reader_3jet->AddVariable("pTV", &pTV);
  reader_3jet->AddVariable("pTB1", &pTB1);
  reader_3jet->AddVariable("pTB2", &pTB2);
  reader_3jet->AddVariable("mTW", &mTW);
  reader_3jet->AddVariable("Mtop", &Mtop);
  reader_3jet->AddVariable("dYWH", &dYWH);
  reader_3jet->AddVariable("MET", &MET);
  reader_3jet->AddVariable("mBBJ", &mBBJ);
  reader_3jet->AddVariable("pTJ3", &pTJ3);


//  std::string dir    = "/unix/atlas3/abell/VHbb_MVATrain/train_v2/weights/";
  std::string dir    = "/unix/atlasvhbb/jpyne/VHbb_MVATrain/train_v3/weights/";

  if(MVADiboson){
    myT_Background=new TChain("Nominal");
    myT_Background->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Normal_Diboson.root" );
    myT_Signal=new TChain("Nominal");
    myT_Signal->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Direct_Diboson_signal.root" );  }
  else{
    myT_Background=new TChain("Nominal");
    myT_Background->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/background_Normal.root" );
    myT_Signal=new TChain("Nominal");
    myT_Signal->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results_v3/Direct_Signal.root" );
  }

//  myT_Background->SetBranchAddress("sample",&sample);
  myT_Background->SetBranchAddress("dRBB",&dRBB);
  myT_Background->SetBranchAddress("mBB",&mBB);
  myT_Background->SetBranchAddress("dPhiVBB",&dPhiVBB);
  myT_Background->SetBranchAddress("dPhiLBmin",&dPhiLBmin);
  myT_Background->SetBranchAddress("pTV",&pTV);
  myT_Background->SetBranchAddress("pTB1",&pTB1);
  myT_Background->SetBranchAddress("pTB2",&pTB2);
  myT_Background->SetBranchAddress("mTW",&mTW);
  myT_Background->SetBranchAddress("Mtop",&Mtop);
  myT_Background->SetBranchAddress("dYWH",&dYWH);
  myT_Background->SetBranchAddress("MET",&MET);
  myT_Background->SetBranchAddress("mBBJ",&mBBJ);
  myT_Background->SetBranchAddress("pTJ3",&pTJ3);
  myT_Background->SetBranchAddress("BDT",&BDT);
  myT_Background->SetBranchAddress("EventNumber",&EventNumber);
  myT_Background->SetBranchAddress("EventWeight",&EventWeight);
  myT_Background->SetBranchAddress("nJ",&nJ);
  myT_Background->SetBranchAddress("nTags",&nTags);
  myT_Background->SetBranchAddress("sample",&sample);

//  myT_Signal->SetBranchAddress("sample",&sample);
  myT_Signal->SetBranchAddress("dRBB",&dRBB);
  myT_Signal->SetBranchAddress("mBB",&mBB);
  myT_Signal->SetBranchAddress("dPhiVBB",&dPhiVBB);
  myT_Signal->SetBranchAddress("dPhiLBmin",&dPhiLBmin);
  myT_Signal->SetBranchAddress("pTV",&pTV);
  myT_Signal->SetBranchAddress("pTB1",&pTB1);
  myT_Signal->SetBranchAddress("pTB2",&pTB2);
  myT_Signal->SetBranchAddress("mTW",&mTW);
  myT_Signal->SetBranchAddress("Mtop",&Mtop);
  myT_Signal->SetBranchAddress("dYWH",&dYWH);
  myT_Signal->SetBranchAddress("MET",&MET);
  myT_Signal->SetBranchAddress("mBBJ",&mBBJ);
  myT_Signal->SetBranchAddress("pTJ3",&pTJ3);
  myT_Signal->SetBranchAddress("EventNumber",&EventNumber);
  myT_Signal->SetBranchAddress("EventWeight",&EventWeight);
  myT_Signal->SetBranchAddress("nJ",&nJ);
  myT_Signal->SetBranchAddress("nTags",&nTags);
  myT_Signal->SetBranchAddress("BDT",&BDT);
  myT_Signal->SetBranchAddress("sample",&sample);

  TFile* outputFile=new TFile("output.root","RECREATE");
  outputFile->cd();


  for(int j=0;j<mvaNames.size();j++){

    std::cout << mvaNames.at(j) << std::endl;
//    if(j!=1&&j!=3){
      reader_2jet->BookMVA("BDT_even"+mvaNames.at(j), dir + "TMVAClassification_1lep2jet_0of2_Sherpa221_"+mvaNames.at(j)+".weights.xml");
      reader_2jet->BookMVA("BDT_odd"+mvaNames.at(j), dir + "TMVAClassification_1lep2jet_1of2_Sherpa221_"+mvaNames.at(j)+".weights.xml");
//    }
//    if(j!=2&&j!=4){
      reader_3jet->BookMVA("BDT_even"+mvaNames.at(j), dir + "TMVAClassification_1lep3jet_0of2_Sherpa221_"+mvaNames.at(j)+".weights.xml");
      reader_3jet->BookMVA("BDT_odd"+mvaNames.at(j), dir + "TMVAClassification_1lep3jet_1of2_Sherpa221_"+mvaNames.at(j)+".weights.xml");
//    }

    cout << "TOTAL number of background events is: " << myT_Background->GetEntries() << endl;
    cout << "TOTAL number of signal events is: " << myT_Signal->GetEntries() << endl;

    TH1F* background_2tag2jet_150ptv_SR_mBB = new TH1F("background_2tag2jet_150ptv_SR_mBB"+mvaNames.at(j),"background_2tag2jet_150ptv_SR_mBB"+mvaNames.at(j),1000,0,500000);
    TH1F* background_2tag3jet_150ptv_SR_mBB = new TH1F("background_2tag3jet_150ptv_SR_mBB"+mvaNames.at(j),"background_2tag3jet_150ptv_SR_mBB"+mvaNames.at(j),1000,0,500000);
    TH1F* signal_2tag2jet_150ptv_SR_mBB = new TH1F("signal_2tag2jet_150ptv_SR_mBB"+mvaNames.at(j),"signal_2tag2jet_150ptv_SR_mBB"+mvaNames.at(j),1000,0,500000);
    TH1F* signal_2tag3jet_150ptv_SR_mBB = new TH1F("signal_2tag3jet_150ptv_SR_mBB"+mvaNames.at(j),"signal_2tag3jet_150ptv_SR_mBB"+mvaNames.at(j),1000,0,500000);

    TH1F* background_2tag2jet_150ptv_SR_mva = new TH1F("background_2tag2jet_150ptv_SR_mva"+mvaNames.at(j),"background_2tag2jet_150ptv_SR_mva"+mvaNames.at(j),1000,-1,1);
    TH1F* background_2tag3jet_150ptv_SR_mva = new TH1F("background_2tag3jet_150ptv_SR_mva"+mvaNames.at(j),"background_2tag3jet_150ptv_SR_mva"+mvaNames.at(j),1000,-1,1);
    TH1F* signal_2tag2jet_150ptv_SR_mva = new TH1F("signal_2tag2jet_150ptv_SR_mva"+mvaNames.at(j),"signal_2tag2jet_150ptv_SR_mva"+mvaNames.at(j),1000,-1,1);
    TH1F* signal_2tag3jet_150ptv_SR_mva = new TH1F("signal_2tag3jet_150ptv_SR_mva"+mvaNames.at(j),"signal_2tag3jet_150ptv_SR_mva"+mvaNames.at(j),1000,-1,1);



    background_2tag2jet_150ptv_SR_mva->Sumw2();
    background_2tag3jet_150ptv_SR_mva->Sumw2();
    signal_2tag2jet_150ptv_SR_mva->Sumw2();
    signal_2tag3jet_150ptv_SR_mva->Sumw2();


    float ttbar2jetWeight=0;
    float ttbar3jetWeight=0;
    float Wbb2jetWeight=0;
    float Wbb3jetWeight=0;

    float signal2jetWeight=0;
    float signal3jetWeight=0;

    float sensitivity_2jet_D=0;
    float sensitivity_3jet_D=0;
    float error_2jet_D=0;
    float error_3jet_D=0;
//    for(int i=0;i<1000;i++){
    for(int i=0;i<myT_Background->GetEntries();i++){
      if(i%5000000==0) std::cout << "Processing entry " << i << std::endl;

      float bdt=0;
      myT_Background->GetEntry(i);
//      std::cout << sample << std::endl;


//      std::cout << "HERE1" << std::endl;  
/*
      if(j==0){
//      if(!(j==0||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v9_c_unit"||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v16_a_RENAMED"||mvaNames.at(j).Contains("RENAMED"))){
	mBB*=1e3;
//    pTV*=1e3;
	pTB1*=1e3;
	pTB2*=1e3;
//    mTW*=1e3;
//    Mtop*=1e3;
	MET*=1e3;
	mBBJ*=1e3;
	pTJ3*=1e3;
      }
/*
  if(!(j==0||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v9_c_unit"||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v16_a_RENAMED"||mvaNames.at(j).Contains("RENAMED"))){
  mBB/=1e3;
  pTB1/=1e3;
  pTB2/=1e3;
  pTJ3/=1e3;
  mBBJ/=1e3;
  MET/=1e3;
  }
*/


      if(nTags!=2) continue;
      if(Mtop>225e3&&mBB<75e3) continue;
      
      if(nJ!=2 && nJ!=3) continue;

      if(mvaNames.at(j)=="Sherpa221_mBBCut_BDT_AdaBoost_v11"){
	if(mBB < 100 || mBB>150) continue;
      }

      if(nJ==2){
//	if(j==1||j==3)	bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(0)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(0));
        bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	background_2tag2jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	background_2tag2jet_150ptv_SR_mva->Fill(bdt,EventWeight); 
	if(sample->find("ttbar") != std::string::npos) ttbar2jetWeight+=EventWeight;
	if(sample->find("Wbb") != std::string::npos) Wbb2jetWeight+=EventWeight;
      }
      else if(nJ==3){
//	if(j==2||j==4) bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(0)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(0));
	bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	background_2tag3jet_150ptv_SR_mva->Fill(bdt,EventWeight);
	background_2tag3jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
        if(sample->find("ttbar") != std::string::npos) ttbar3jetWeight+=EventWeight;
        if(sample->find("Wbb") != std::string::npos) Wbb3jetWeight+=EventWeight;
      }
    }

//    for(int i=0;i<1000;i++){
    for(int i=0;i<myT_Signal->GetEntries();i++){
      if(i%10000==0) std::cout << "Processing entry " << i << std::endl;
      float bdt=0;
      myT_Signal->GetEntry(i);
      /*
      if(j==0){
	mBB*=1e3;
//    pTV*=1e3;
	pTB1*=1e3;
	pTB2*=1e3;
//    mTW*=1e3;
//    Mtop*=1e3;
	MET*=1e3;
	mBBJ*=1e3;
	pTJ3*=1e3;
      }
/*
  if(!(j==0||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v9_c_unit"||mvaNames.at(j)=="Sherpa221_BDT_AdaBoost_v16_a_RENAMED"||mvaNames.at(j).Contains("RENAMED"))){
  mBB/=1e3;
  pTB1/=1e3;
  pTB2/=1e3;
  pTJ3/=1e3;
  mBBJ/=1e3;
  MET/=1e3;
  }
*/
      if(nTags!=2) continue;
      if(Mtop>225e3&&mBB<75e3) continue;
      if(nJ!=2 && nJ!=3) continue;
      if(mvaNames.at(j)=="Sherpa221_mBBCut_BDT_AdaBoost_v11"){
	if(mBB < 100 || mBB>150) continue;
      }

      if(nJ==2){
//	if(j==1||j==3)	bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(0)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(0));
        bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	signal_2tag2jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	signal_2tag2jet_150ptv_SR_mva->Fill(bdt,EventWeight);
	signal2jetWeight+=EventWeight;
      }
      else if(nJ==3){
//	if(j==2||j==4) bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(0)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(0));
	bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	signal_2tag3jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	signal_2tag3jet_150ptv_SR_mva->Fill(bdt,EventWeight);
	signal3jetWeight+=EventWeight;
      }
    }

    std::cout << "ttbar, 2-jet: " << ttbar2jetWeight << std::endl;
    std::cout << "ttbar, 3-jet: " << ttbar3jetWeight << std::endl;
    std::cout << "Wbb, 2-jet: " << Wbb2jetWeight << std::endl;
    std::cout << "Wbb, 3-jet: " << Wbb3jetWeight << std::endl;
    std::cout << "Signal, 2-jet: " << signal2jetWeight << std::endl;
    std::cout << "Signal, 3-jet: " << signal3jetWeight << std::endl;

    Transform(background_2tag2jet_150ptv_SR_mva,signal_2tag2jet_150ptv_SR_mva,sensitivity_2jet_D,error_2jet_D);
    Transform(background_2tag3jet_150ptv_SR_mva,signal_2tag3jet_150ptv_SR_mva,sensitivity_3jet_D,error_3jet_D);
//    Transform(background_2tag2jet_150ptv_SR_mva,signal_2tag2jet_150ptv_SR_mva,sensitivity_2jet_D, error_2jet_D);
//    Transform(background_2tag3jet_150ptv_SR_mva,signal_2tag3jet_150ptv_SR_mva,sensitivity_3jet_D, error_3jet_D);

    std::cout << "<<<<<<<<<<<BEGIN RESULT<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << "MVA training" << mvaNames.at(j) << std::endl;
    std::cout << "<<<<<<<<<<<Trafo D <<<<<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << "sensitivity_2jet " << pow(sensitivity_2jet_D,0.5) << " +/- " << 0.5 * pow(error_2jet_D / sensitivity_2jet_D,0.5) << std::endl;
    std::cout << "sensitivity_3jet " << pow(sensitivity_3jet_D,0.5) << " +/- " << 0.5 * pow(error_3jet_D / sensitivity_3jet_D,0.5) << std::endl;
    std::cout << "Combined sensitivity " << pow(sensitivity_2jet_D+sensitivity_3jet_D,0.5) << " +/- " << 0.5 * pow((error_2jet_D + error_3jet_D) / (sensitivity_2jet_D + sensitivity_3jet_D),0.5) << std::endl;


//    std::cout << "<<<<<<<<<<<Trafo F <<<<<<<<<<<<<<<<<<<<" << std::endl;
//    std::cout << "sensitivity_2jet " << pow(sensitivity_2jet_F,0.5) << std::endl;
//    std::cout << "sensitivity_3jet " << pow(sensitivity_3jet_F,0.5) << std::endl;
//    std::cout << "Combined sensitivity " << pow(sensitivity_2jet_F+sensitivity_3jet_F,0.5) << std::endl;
    std::cout << "<<<<<<<<<<<END RESULT<<<<<<<<<<<<<<<<" << std::endl;
/*
  std::cout << "<<<<<<<<<<<BEGIN RESULT mBB<<<<<<<<<<<<<<<<" << std::endl;
  std::cout << "sensitivity_2jet " << pow(sensitivity_2jet_mBB,0.5) << std::endl;
  std::cout << "sensitivity_3jet " << pow(sensitivity_3jet_mBB,0.5) << std::endl;
  std::cout << "Combined sensitivity " << pow(sensitivity_2jet_mBB+sensitivity_3jet_mBB,0.5) << std::endl;
  std::cout << "<<<<<<<<<<<END RESULT<<<<<<<<<<<<<<<<" << std::endl;
*/


    myfile << "<<<<<<<<<<<BEGIN RESULT<<<<<<<<<<<<<<<<" << std::endl;
    myfile << "MVA training" << mvaNames.at(j) << std::endl;
    myfile << "<<<<<<<<<<<Trafo D <<<<<<<<<<<<<<<<<<<<" << std::endl;
    myfile << "sensitivity_2jet " << pow(sensitivity_2jet_D,0.5) << " +/- " << 0.5 * pow(error_2jet_D / sensitivity_2jet_D,0.5) << std::endl;
    myfile << "sensitivity_3jet " << pow(sensitivity_3jet_D,0.5) << " +/- " << 0.5 * pow(error_3jet_D / sensitivity_3jet_D,0.5) << std::endl;
    myfile << "Combined sensitivity " << pow(sensitivity_2jet_D+sensitivity_3jet_D,0.5) << " +/- " << 0.5 * pow((error_2jet_D + error_3jet_D) / (sensitivity_2jet_D + sensitivity_3jet_D),0.5) << std::endl;
//    myfile << "<<<<<<<<<<<Trafo F <<<<<<<<<<<<<<<<<<<<" << std::endl;
//    myfile << "sensitivity_2jet " << pow(sensitivity_2jet_F,0.5) << std::endl;
//    myfile << "sensitivity_3jet " << pow(sensitivity_3jet_F,0.5) << std::endl;
//    myfile << "Combined sensitivity " << pow(sensitivity_2jet_F+sensitivity_3jet_F,0.5) << std::endl;
    myfile << "<<<<<<<<<<<END RESULT<<<<<<<<<<<<<<<<" << std::endl;


    background_2tag2jet_150ptv_SR_mva->Write();
    background_2tag3jet_150ptv_SR_mva->Write();
    signal_2tag2jet_150ptv_SR_mva->Write();
    signal_2tag3jet_150ptv_SR_mva->Write();
    
    background_2tag2jet_150ptv_SR_mva->Delete();
    background_2tag3jet_150ptv_SR_mva->Delete();
    signal_2tag2jet_150ptv_SR_mva->Delete();
    signal_2tag3jet_150ptv_SR_mva->Delete();
    
  }
  outputFile->Write();
  outputFile->Close();

}
