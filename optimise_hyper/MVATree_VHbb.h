#ifndef CxAODReader_MVATree_VHbb_H
#define CxAODReader_MVATree_VHbb_H

#include <string>

#include "CxAODReader/MVATree.h"
#include "CxAODReader/StaticMVATree.h"

namespace CxAODReader_MVATree_VHbb_internal {
  template<class Tree> using STree = StaticMVATree<Tree,
                                                   bool, int, unsigned,
                                                   long, unsigned long,
                                                   float, double,
                                                   std::string>;
}

class MVATree_VHbb : public MVATree, public CxAODReader_MVATree_VHbb_internal::STree<MVATree_VHbb> {

friend CxAODReader_MVATree_VHbb_internal::STree<MVATree_VHbb>;

protected:

  std::string m_analysisType;

  virtual void AddBranch(TString name, Float_t* address);
  virtual void AddBranch(TString name, Int_t* address);
  virtual void AddBranch(TString name, ULong64_t * address);
  //virtual void AddBranch(TString name, std::string* address);
  template <class T>
  void AddBranch(TString name, T* address)
  {
    for (std::pair<std::string, TTree*> tree : m_treeMap) {
      tree.second -> Branch(name, address);
    }
  }
  virtual void SetBranches() override;
  virtual void TransformVars() override;
  float getBinnedMV1c(float MV1c);

public:

  MVATree_VHbb(bool persistent, bool readMVA, std::string analysisType, EL::Worker* wk, std::vector<std::string> variations, bool nominalOnly);
//  MVATree_VHbb(bool persistent, bool readMVA, std::string analysisType, EL::Worker* wk);//, std::vector<std::string> variations, bool nominalOnly);

  ~MVATree_VHbb() {}

  virtual void Reset() override;
  virtual void ReadMVA();
  void Fill();

  std::string sample;

  float EventWeight;
  float weight; // compatibility with Paul's MVA code
  unsigned long long EventNumber;
  int isOdd; // compatibility with Paul's MVA code

  float mBB;
  float dRBB;
  float dEtaBB;
  float dEtaVBB;
  float dPhiVBB;
  float pTV;
  float pTB1;
  float pTB2;
  float pTBB;
  float pTBBoverMET;
  float pTJ3;
  float etaJ3; 
  float MV1cJ3; 
  float dRB1J3; 
  float dRB2J3; 
  float mBBJ;
  float MV1cB1;
  float MV1cB2;
  float MET;
  float HT;
  int nSigJet; 
  int nForwardJet; 
  float mLL;
  int nJ;
  int nTag;
  float sumPt;
  float dPhiBB;
  float dPhiMETMPT;
  float dPhiMETdijet;
  float mindPhi;
  float dPhiLBmin;
  float MEff;
  float MEff3;
  float etaB1;
  float etaB2;
  float mTW;
  float MPT;
  float METHT;
  float pTL;
  float etaL;
  float Tau21;
  float Tau32;
  float mVH;
  float mVH1;
  float mVH2;
  float pTVHnorm;
  float Mtop;
  float MtopFullMETShift;
  float dEtaWH;
  float dYWH;
  float dPhiLMET;
  float mBB_corr;
  float BDT;

  float BDT_100ntrees_3maxdepth
  float BDT_100ntrees_4maxdepth
  float BDT_100ntrees_5maxdepth
  float BDT_100ntrees_6maxdepth
  float BDT_100ntrees_7maxdepth
  float BDT_110ntrees_3maxdepth
  float BDT_110ntrees_4maxdepth
  float BDT_110ntrees_5maxdepth
  float BDT_110ntrees_6maxdepth
  float BDT_110ntrees_7maxdepth
  float BDT_120ntrees_3maxdepth
  float BDT_120ntrees_4maxdepth
  float BDT_120ntrees_5maxdepth
  float BDT_120ntrees_6maxdepth
  float BDT_120ntrees_7maxdepth
  float BDT_130ntrees_3maxdepth
  float BDT_130ntrees_4maxdepth
  float BDT_130ntrees_5maxdepth
  float BDT_130ntrees_6maxdepth
  float BDT_130ntrees_7maxdepth
  float BDT_140ntrees_3maxdepth
  float BDT_140ntrees_4maxdepth
  float BDT_140ntrees_5maxdepth
  float BDT_140ntrees_6maxdepth
  float BDT_140ntrees_7maxdepth
  float BDT_150ntrees_3maxdepth
  float BDT_150ntrees_4maxdepth
  float BDT_150ntrees_5maxdepth
  float BDT_150ntrees_6maxdepth
  float BDT_150ntrees_7maxdepth
  float BDT_160ntrees_3maxdepth
  float BDT_160ntrees_4maxdepth
  float BDT_160ntrees_5maxdepth
  float BDT_160ntrees_6maxdepth
  float BDT_160ntrees_7maxdepth
  float BDT_170ntrees_3maxdepth
  float BDT_170ntrees_4maxdepth
  float BDT_170ntrees_5maxdepth
  float BDT_170ntrees_6maxdepth
  float BDT_170ntrees_7maxdepth
  float BDT_180ntrees_3maxdepth
  float BDT_180ntrees_4maxdepth
  float BDT_180ntrees_5maxdepth
  float BDT_180ntrees_6maxdepth
  float BDT_180ntrees_7maxdepth
  float BDT_190ntrees_3maxdepth
  float BDT_190ntrees_4maxdepth
  float BDT_190ntrees_5maxdepth
  float BDT_190ntrees_6maxdepth
  float BDT_190ntrees_7maxdepth
  float BDT_200ntrees_3maxdepth
  float BDT_200ntrees_4maxdepth
  float BDT_200ntrees_5maxdepth
  float BDT_200ntrees_6maxdepth
  float BDT_200ntrees_7maxdepth
  float BDT_210ntrees_3maxdepth
  float BDT_210ntrees_4maxdepth
  float BDT_210ntrees_5maxdepth
  float BDT_210ntrees_6maxdepth
  float BDT_210ntrees_7maxdepth
  float BDT_220ntrees_3maxdepth
  float BDT_220ntrees_4maxdepth
  float BDT_220ntrees_5maxdepth
  float BDT_220ntrees_6maxdepth
  float BDT_220ntrees_7maxdepth
  float BDT_230ntrees_3maxdepth
  float BDT_230ntrees_4maxdepth
  float BDT_230ntrees_5maxdepth
  float BDT_230ntrees_6maxdepth
  float BDT_230ntrees_7maxdepth
  float BDT_240ntrees_3maxdepth
  float BDT_240ntrees_4maxdepth
  float BDT_240ntrees_5maxdepth
  float BDT_240ntrees_6maxdepth
  float BDT_240ntrees_7maxdepth
  float BDT_250ntrees_3maxdepth
  float BDT_250ntrees_4maxdepth
  float BDT_250ntrees_5maxdepth
  float BDT_250ntrees_6maxdepth
  float BDT_250ntrees_7maxdepth
  float BDT_260ntrees_3maxdepth
  float BDT_260ntrees_4maxdepth
  float BDT_260ntrees_5maxdepth
  float BDT_260ntrees_6maxdepth
  float BDT_260ntrees_7maxdepth
  float BDT_270ntrees_3maxdepth
  float BDT_270ntrees_4maxdepth
  float BDT_270ntrees_5maxdepth
  float BDT_270ntrees_6maxdepth
  float BDT_270ntrees_7maxdepth
  float BDT_280ntrees_3maxdepth
  float BDT_280ntrees_4maxdepth
  float BDT_280ntrees_5maxdepth
  float BDT_280ntrees_6maxdepth
  float BDT_280ntrees_7maxdepth
  float BDT_290ntrees_3maxdepth
  float BDT_290ntrees_4maxdepth
  float BDT_290ntrees_5maxdepth
  float BDT_290ntrees_6maxdepth
  float BDT_290ntrees_7maxdepth
  float BDT_300ntrees_3maxdepth
  float BDT_300ntrees_4maxdepth
  float BDT_300ntrees_5maxdepth
  float BDT_300ntrees_6maxdepth
  float BDT_300ntrees_7maxdepth
  float BDT_310ntrees_3maxdepth
  float BDT_310ntrees_4maxdepth
  float BDT_310ntrees_5maxdepth
  float BDT_310ntrees_6maxdepth
  float BDT_310ntrees_7maxdepth
  float BDT_320ntrees_3maxdepth
  float BDT_320ntrees_4maxdepth
  float BDT_320ntrees_5maxdepth
  float BDT_320ntrees_6maxdepth
  float BDT_320ntrees_7maxdepth
  float BDT_330ntrees_3maxdepth
  float BDT_330ntrees_4maxdepth
  float BDT_330ntrees_5maxdepth
  float BDT_330ntrees_6maxdepth
  float BDT_330ntrees_7maxdepth
  float BDT_340ntrees_3maxdepth
  float BDT_340ntrees_4maxdepth
  float BDT_340ntrees_5maxdepth
  float BDT_340ntrees_6maxdepth
  float BDT_340ntrees_7maxdepth
  float BDT_350ntrees_3maxdepth
  float BDT_350ntrees_4maxdepth
  float BDT_350ntrees_5maxdepth
  float BDT_350ntrees_6maxdepth
  float BDT_350ntrees_7maxdepth
  float BDT_360ntrees_3maxdepth
  float BDT_360ntrees_4maxdepth
  float BDT_360ntrees_5maxdepth
  float BDT_360ntrees_6maxdepth
  float BDT_360ntrees_7maxdepth
  float BDT_370ntrees_3maxdepth
  float BDT_370ntrees_4maxdepth
  float BDT_370ntrees_5maxdepth
  float BDT_370ntrees_6maxdepth
  float BDT_370ntrees_7maxdepth
  float BDT_380ntrees_3maxdepth
  float BDT_380ntrees_4maxdepth
  float BDT_380ntrees_5maxdepth
  float BDT_380ntrees_6maxdepth
  float BDT_380ntrees_7maxdepth
  float BDT_390ntrees_3maxdepth
  float BDT_390ntrees_4maxdepth
  float BDT_390ntrees_5maxdepth
  float BDT_390ntrees_6maxdepth
  float BDT_390ntrees_7maxdepth
  float BDT_400ntrees_3maxdepth
  float BDT_400ntrees_4maxdepth
  float BDT_400ntrees_5maxdepth
  float BDT_400ntrees_6maxdepth
  float BDT_400ntrees_7maxdepth

  float BDTCharles;
  float triggerSF;
  float leptonSF;
  float btagWeight;
  float PUWeight;
  float MCWeight;
  float QCDWeight;
  //float CSWeight;

  float jjdR;
  float jjM;
  float WjjdPhi;
  float ljmindPhi;
  float WPt;
  float j1Pt;
  float j2Pt;
  float WMt;
  float mtop;
  float met;
  float jjjM;
  float j3Pt;
};


#endif
