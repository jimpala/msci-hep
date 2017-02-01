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

  float BDT_300ntrees_7maxdepth;
  float BDT_300ntrees_8maxdepth;
  float BDT_300ntrees_9maxdepth;
  float BDT_300ntrees_10maxdepth;
  float BDT_300ntrees_11maxdepth;





  float BDT_320ntrees_7maxdepth;
  float BDT_320ntrees_8maxdepth;
  float BDT_320ntrees_9maxdepth;
  float BDT_320ntrees_10maxdepth;
  float BDT_320ntrees_11maxdepth;





  float BDT_340ntrees_7maxdepth;
  float BDT_340ntrees_8maxdepth;
  float BDT_340ntrees_9maxdepth;
  float BDT_340ntrees_10maxdepth;
  float BDT_340ntrees_11maxdepth;





  float BDT_360ntrees_7maxdepth;
  float BDT_360ntrees_8maxdepth;
  float BDT_360ntrees_9maxdepth;
  float BDT_360ntrees_10maxdepth;
  float BDT_360ntrees_11maxdepth;





  float BDT_380ntrees_7maxdepth;
  float BDT_380ntrees_8maxdepth;
  float BDT_380ntrees_9maxdepth;
  float BDT_380ntrees_10maxdepth;
  float BDT_380ntrees_11maxdepth;





  float BDT_400ntrees_7maxdepth;
  float BDT_400ntrees_8maxdepth;
  float BDT_400ntrees_9maxdepth;
  float BDT_400ntrees_10maxdepth;
  float BDT_400ntrees_11maxdepth;





  float BDT_420ntrees_7maxdepth;
  float BDT_420ntrees_8maxdepth;
  float BDT_420ntrees_9maxdepth;
  float BDT_420ntrees_10maxdepth;
  float BDT_420ntrees_11maxdepth;





  float BDT_440ntrees_7maxdepth;
  float BDT_440ntrees_8maxdepth;
  float BDT_440ntrees_9maxdepth;
  float BDT_440ntrees_10maxdepth;
  float BDT_440ntrees_11maxdepth;





  float BDT_460ntrees_7maxdepth;
  float BDT_460ntrees_8maxdepth;
  float BDT_460ntrees_9maxdepth;
  float BDT_460ntrees_10maxdepth;
  float BDT_460ntrees_11maxdepth;





  float BDT_480ntrees_7maxdepth;
  float BDT_480ntrees_8maxdepth;
  float BDT_480ntrees_9maxdepth;
  float BDT_480ntrees_10maxdepth;
  float BDT_480ntrees_11maxdepth;





  float BDT_500ntrees_7maxdepth;
  float BDT_500ntrees_8maxdepth;
  float BDT_500ntrees_9maxdepth;
  float BDT_500ntrees_10maxdepth;
  float BDT_500ntrees_11maxdepth;



















































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
