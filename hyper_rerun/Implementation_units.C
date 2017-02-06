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

void Transform(TH1F* back, TH1F* sig, float& sensitivity){

  float back_int = back->Integral(0,back->GetNbinsX()+1);
  float sig_int = sig->Integral(0,sig->GetNbinsX()+1);

  int EndBin = back->GetNbinsX()+1;
  int z_s=10;
  int z_b=10;
  float z=0;

  float n_s_total=0;
  float n_b_total=0;

  for(int i=EndBin;i>=0;i--){
    float n_s = sig->Integral(i,EndBin);
    float n_b = back->Integral(i,EndBin);

    z = ( z_b * n_b / back_int ) + ( z_s * n_s / sig_int );
//    std::cout << "At bin i = " << i << " z = " << z << std::endl;

    if( z>1 || i==0 ){
      EndBin = i - 1;
    }else{
      continue;
    }

    n_s_total+=n_s;
    n_b_total+=n_b;

    float dSens = (2 * ((n_s+n_b)*log(1+n_s/n_b)-n_s));
    std::cout << dSens << std::endl;
    if(dSens==dSens)  sensitivity += (2 * ((n_s+n_b)*log(1+n_s/n_b)-n_s));
/*
  std::cout << pow(sensitivity,0.5) << std::endl;
  std::cout << n_s_total << std::endl;
  std::cout << n_b_total << std::endl;
*/
  }
}


void Sensitivity(TH1F* back, TH1F* sig, float& sensitivity){
  int EndBin = back->GetNbinsX()+1;

  for(int i=EndBin;i>=0;i--){
    float n_s = sig->Integral(i,EndBin);
    float n_b = back->Integral(i,EndBin);

    float dSens = (2 * ((n_s+n_b)*log(1+n_s/n_b)-n_s));

    if(dSens==dSens)  sensitivity += (2 * ((n_s+n_b)*log(1+n_s/n_b)-n_s));
/*
  std::cout << pow(sensitivity,0.5) << std::endl;
  std::cout << n_s_total << std::endl;
  std::cout << n_b_total << std::endl;
*/
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Implementation(){
  gStyle->SetOptStat(0);

  ofstream myfile;
  TString openfile = "sensitivities.txt";
  myfile.open (openfile);

  std::vector<TString> mvaNames;
  mvaNames.push_back("100ntrees_3maxdepth");
  mvaNames.push_back("100ntrees_4maxdepth");
  mvaNames.push_back("100ntrees_5maxdepth");
  mvaNames.push_back("100ntrees_6maxdepth");
  mvaNames.push_back("100ntrees_7maxdepth");





  mvaNames.push_back("120ntrees_3maxdepth");
  mvaNames.push_back("120ntrees_4maxdepth");
  mvaNames.push_back("120ntrees_5maxdepth");
  mvaNames.push_back("120ntrees_6maxdepth");
  mvaNames.push_back("120ntrees_7maxdepth");





  mvaNames.push_back("140ntrees_3maxdepth");
  mvaNames.push_back("140ntrees_4maxdepth");
  mvaNames.push_back("140ntrees_5maxdepth");
  mvaNames.push_back("140ntrees_6maxdepth");
  mvaNames.push_back("140ntrees_7maxdepth");





  mvaNames.push_back("160ntrees_3maxdepth");
  mvaNames.push_back("160ntrees_4maxdepth");
  mvaNames.push_back("160ntrees_5maxdepth");
  mvaNames.push_back("160ntrees_6maxdepth");
  mvaNames.push_back("160ntrees_7maxdepth");





  mvaNames.push_back("180ntrees_3maxdepth");
  mvaNames.push_back("180ntrees_4maxdepth");
  mvaNames.push_back("180ntrees_5maxdepth");
  mvaNames.push_back("180ntrees_6maxdepth");
  mvaNames.push_back("180ntrees_7maxdepth");





  mvaNames.push_back("200ntrees_3maxdepth");
  mvaNames.push_back("200ntrees_4maxdepth");
  mvaNames.push_back("200ntrees_5maxdepth");
  mvaNames.push_back("200ntrees_6maxdepth");
  mvaNames.push_back("200ntrees_7maxdepth");





  mvaNames.push_back("220ntrees_3maxdepth");
  mvaNames.push_back("220ntrees_4maxdepth");
  mvaNames.push_back("220ntrees_5maxdepth");
  mvaNames.push_back("220ntrees_6maxdepth");
  mvaNames.push_back("220ntrees_7maxdepth");





  mvaNames.push_back("240ntrees_3maxdepth");
  mvaNames.push_back("240ntrees_4maxdepth");
  mvaNames.push_back("240ntrees_5maxdepth");
  mvaNames.push_back("240ntrees_6maxdepth");
  mvaNames.push_back("240ntrees_7maxdepth");





  mvaNames.push_back("260ntrees_3maxdepth");
  mvaNames.push_back("260ntrees_4maxdepth");
  mvaNames.push_back("260ntrees_5maxdepth");
  mvaNames.push_back("260ntrees_6maxdepth");
  mvaNames.push_back("260ntrees_7maxdepth");





  mvaNames.push_back("280ntrees_3maxdepth");
  mvaNames.push_back("280ntrees_4maxdepth");
  mvaNames.push_back("280ntrees_5maxdepth");
  mvaNames.push_back("280ntrees_6maxdepth");
  mvaNames.push_back("280ntrees_7maxdepth");





  mvaNames.push_back("300ntrees_3maxdepth");
  mvaNames.push_back("300ntrees_4maxdepth");
  mvaNames.push_back("300ntrees_5maxdepth");
  mvaNames.push_back("300ntrees_6maxdepth");
  mvaNames.push_back("300ntrees_7maxdepth");





  mvaNames.push_back("320ntrees_3maxdepth");
  mvaNames.push_back("320ntrees_4maxdepth");
  mvaNames.push_back("320ntrees_5maxdepth");
  mvaNames.push_back("320ntrees_6maxdepth");
  mvaNames.push_back("320ntrees_7maxdepth");





  mvaNames.push_back("340ntrees_3maxdepth");
  mvaNames.push_back("340ntrees_4maxdepth");
  mvaNames.push_back("340ntrees_5maxdepth");
  mvaNames.push_back("340ntrees_6maxdepth");
  mvaNames.push_back("340ntrees_7maxdepth");





  mvaNames.push_back("360ntrees_3maxdepth");
  mvaNames.push_back("360ntrees_4maxdepth");
  mvaNames.push_back("360ntrees_5maxdepth");
  mvaNames.push_back("360ntrees_6maxdepth");
  mvaNames.push_back("360ntrees_7maxdepth");





  mvaNames.push_back("380ntrees_3maxdepth");
  mvaNames.push_back("380ntrees_4maxdepth");
  mvaNames.push_back("380ntrees_5maxdepth");
  mvaNames.push_back("380ntrees_6maxdepth");
  mvaNames.push_back("380ntrees_7maxdepth");





  mvaNames.push_back("400ntrees_3maxdepth");
  mvaNames.push_back("400ntrees_4maxdepth");
  mvaNames.push_back("400ntrees_5maxdepth");
  mvaNames.push_back("400ntrees_6maxdepth");
  mvaNames.push_back("400ntrees_7maxdepth");


    TMVA::Reader* reader_2jet = new TMVA::Reader();
//  TMVA::Reader* reader_2jet_ICHEP = new TMVA::Reader();
  TMVA::Reader* reader_3jet = new TMVA::Reader();
//  TMVA::Reader* reader_3jet_ICHEP = new TMVA::Reader();
  float dRBB,mBB,dPhiVBB,dPhiLBmin,pTV,pTB1,pTB2,mTW,Mtop,dYWH,MET,mBBJ,pTJ3;
  float EventWeight, BDT;
  int nBJ, nJ;
  unsigned long long EventNumber;

  reader_2jet->AddVariable("jjdR", &dRBB);
  reader_2jet->AddVariable("jjM", &mBB);
  reader_2jet->AddVariable("WjjdPhi", &dPhiVBB);
  reader_2jet->AddVariable("ljmindPhi", &dPhiLBmin);
  reader_2jet->AddVariable("WPt", &pTV);
  reader_2jet->AddVariable("j1Pt", &pTB1);
  reader_2jet->AddVariable("j2Pt", &pTB2);
  reader_2jet->AddVariable("WMt", &mTW);
  reader_2jet->AddVariable("mtop", &Mtop);
  reader_2jet->AddVariable("dYWH", &dYWH);
  reader_2jet->AddVariable("met", &MET);

  reader_3jet->AddVariable("jjdR", &dRBB);
  reader_3jet->AddVariable("jjM", &mBB);
  reader_3jet->AddVariable("WjjdPhi", &dPhiVBB);
  reader_3jet->AddVariable("ljmindPhi", &dPhiLBmin);
  reader_3jet->AddVariable("WPt", &pTV);
  reader_3jet->AddVariable("j1Pt", &pTB1);
  reader_3jet->AddVariable("j2Pt", &pTB2);
  reader_3jet->AddVariable("WMt", &mTW);
  reader_3jet->AddVariable("mtop", &Mtop);
  reader_3jet->AddVariable("dYWH", &dYWH);
  reader_3jet->AddVariable("met", &MET);
  reader_3jet->AddVariable("jjjM", &dYWH);
  reader_3jet->AddVariable("j3Pt", &MET);


  std::string dir    = "/unix/atlasvhbb/jpyne/VHbb_MVATrain/train_v2/weights/";


  myT_Background=new TChain("Nominal");
  myT_Background->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results/background_Normal.root" );
  myT_Signal=new TChain("Nominal");
  myT_Signal->Add( "/unix/atlas3/abell/VHbb_MVATrain/Implementation/Results/Direct_Signal.root" );

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
  myT_Background->SetBranchAddress("nBJ",&nBJ);

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
  myT_Signal->SetBranchAddress("nBJ",&nBJ);
  myT_Signal->SetBranchAddress("BDT",&BDT);

  TFile* outputFile=new TFile("output.root","RECREATE");
  outputFile->cd();


  for(int j=0;j<mvaNames.size();j++){

    std::cout << mvaNames.at(j) << std::endl;

    reader_2jet->BookMVA("BDT_even"+mvaNames.at(j), dir + "TMVAClassification_1lep2jet_"+mvaNames.at(j)+"_0of2_BDT_AdaBoost_v6.weights.xml");
    reader_2jet->BookMVA("BDT_odd"+mvaNames.at(j), dir + "TMVAClassification_1lep2jet_"+mvaNames.at(j)+"_1of2_BDT_AdaBoost_v6.weights.xml");
    
    reader_3jet->BookMVA("BDT_even"+mvaNames.at(j), dir + "TMVAClassification_1lep3jet_"+mvaNames.at(j)+"_0of2_BDT_AdaBoost_v6.weights.xml");
    reader_3jet->BookMVA("BDT_odd"+mvaNames.at(j), dir + "TMVAClassification_1lep3jet_"+mvaNames.at(j)+"_1of2_BDT_AdaBoost_v6.weights.xml");
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


    float sensitivity_2jet=0;
    float sensitivity_3jet=0;
    float sensitivity_2jet_mBB=0;
    float sensitivity_3jet_mBB=0;
//    for(int i=0;i<1000;i++){
    for(int i=0;i<myT_Background->GetEntries();i++){
      if(i%5000000==0) std::cout << "Processing entry " << i << std::endl;
      float bdt=0;
      myT_Background->GetEntry(i);

//      std::cout << "HERE1" << std::endl;  

      if(j==0){
//	mBB*=1e3;
    pTV/=1e3;
//	pTB1*=1e3;
//	pTB2*=1e3;
    mTW/=1e3;
    Mtop/=1e3;
//	MET*=1e3;
//	mBBJ*=1e3;
//	pTJ3*=1e3;
      }
/*
    pTV/=1e3;
    mTW/=1e3;
    Mtop/=1e3;
*/

      if(nBJ!=2) continue;
//      if(Mtop>225e3&&mBB<75e3) continue;
      
      if(nJ!=2 && nJ!=3) continue;


      if(nJ==2){
	bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	background_2tag2jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	background_2tag2jet_150ptv_SR_mva->Fill(bdt,EventWeight); 
      }
      else if(nJ==3){
	bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	background_2tag3jet_150ptv_SR_mva->Fill(bdt,EventWeight);
	background_2tag3jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
      }
    }

//    for(int i=0;i<1000;i++){
    for(int i=0;i<myT_Signal->GetEntries();i++){
      if(i%10000==0) std::cout << "Processing entry " << i << std::endl;
      float bdt=0;
      myT_Signal->GetEntry(i);

      if(j==0){
//	mBB*=1e3;
    pTV*=1e3;
//	pTB1*=1e3;
//	pTB2*=1e3;
    mTW/=1e3;
    Mtop/=1e3;
//	MET*=1e3;
//	mBBJ*=1e3;
//	pTJ3*=1e3;
      }
/*
    pTV/=1e3;
    mTW/=1e3;
    Mtop/=1e3;
*/

      if(nBJ!=2) continue;
//      if(Mtop>225e3&&mBB<75e3) continue;
      if(nJ!=2 && nJ!=3) continue;

      if(nJ==2){
	bdt = (EventNumber%2!=0) ? reader_2jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_2jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	signal_2tag2jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	signal_2tag2jet_150ptv_SR_mva->Fill(bdt,EventWeight);
      }
      else if(nJ==3){
	bdt = (EventNumber%2!=0) ? reader_3jet->EvaluateMVA("BDT_odd"+mvaNames.at(j)) : reader_3jet->EvaluateMVA("BDT_even"+mvaNames.at(j));
	signal_2tag3jet_150ptv_SR_mBB->Fill(mBB,EventWeight);
	signal_2tag3jet_150ptv_SR_mva->Fill(bdt,EventWeight);
      }
    }

    Transform(background_2tag2jet_150ptv_SR_mva,signal_2tag2jet_150ptv_SR_mva,sensitivity_2jet);
    Transform(background_2tag3jet_150ptv_SR_mva,signal_2tag3jet_150ptv_SR_mva,sensitivity_3jet);


    Sensitivity(background_2tag2jet_150ptv_SR_mBB,signal_2tag2jet_150ptv_SR_mBB,sensitivity_2jet_mBB);
    Sensitivity(background_2tag3jet_150ptv_SR_mBB,signal_2tag3jet_150ptv_SR_mBB,sensitivity_3jet_mBB);


    std::cout << "<<<<<<<<<<<BEGIN RESULT<<<<<<<<<<<<<<<<" << std::endl;
    std::cout << "MVA training" << mvaNames.at(j) << std::endl;
    std::cout << "sensitivity_2jet " << pow(sensitivity_2jet,0.5) << std::endl;
    std::cout << "sensitivity_3jet " << pow(sensitivity_3jet,0.5) << std::endl;
    std::cout << "Combined sensitivity " << pow(sensitivity_2jet+sensitivity_3jet,0.5) << std::endl;
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
    myfile << "sensitivity_2jet " << pow(sensitivity_2jet,0.5) << std::endl;
    myfile << "sensitivity_3jet " << pow(sensitivity_3jet,0.5) << std::endl;
    myfile << "Combined sensitivity " << pow(sensitivity_2jet+sensitivity_3jet,0.5) << std::endl;
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
