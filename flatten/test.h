//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 14 10:23:10 2016 by ROOT version 5.34/36
// from TTree bTag_AntiKt4EMTopoJets/bTagAntiKt4EMTopoJets
// found on file: group.perf-flavtag.8324358.Akt4EMTo._002487.root
//////////////////////////////////////////////////////////

#ifndef test_h
#define test_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

//Jimmy's ROOT includes
#include <TNtuple.h>

//Jimmy's STL includes
#include <string>

// Header file for the classes stored in the TTree if any.
#include <vector>
#include <vector>
#include <vector>
#include <vector>

// Fixed size dimensions of array or collections stored in the TTree if any.

class test {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           runnb;
   Int_t           eventnb;
   Int_t           mcchan;
   Double_t        mcwg;
   Int_t           lbn;
   Int_t           coreFlag;
   Int_t           larError;
   Int_t           tileError;
   Int_t           nPV;
   Double_t        avgmu;
   Int_t           actmu;
   Double_t        PVx;
   Double_t        PVy;
   Double_t        PVz;
   Double_t        truth_PVx;
   Double_t        truth_PVy;
   Double_t        truth_PVz;
   Float_t         truth_LeadJet_pt;
   Int_t           njets;
   Int_t           nbjets;
   Int_t           nbjets_q;
   Int_t           nbjets_HadI;
   Int_t           nbjets_HadF;
   vector<float>   *jet_pt;
   vector<float>   *jet_eta;
   vector<float>   *jet_phi;
   vector<float>   *jet_pt_orig;
   vector<float>   *jet_eta_orig;
   vector<float>   *jet_phi_orig;
   vector<float>   *jet_E_orig;
   vector<float>   *jet_sumtrkS_pt;
   vector<float>   *jet_sumtrkV_pt;
   vector<float>   *jet_sumtrkV_eta;
   vector<float>   *jet_sumtrkV_phi;
   vector<int>     *jet_sumtrk_ntrk;
   vector<float>   *jet_E;
   vector<float>   *jet_m;
   vector<int>     *jet_nConst;
   vector<int>     *jet_truthflav;
   vector<int>     *jet_nBHadr;
   vector<int>     *jet_nCHadr;
   vector<int>     *jet_nGhostBHadrFromParent;
   vector<int>     *jet_nGhostCHadrFromParent;
   vector<int>     *jet_nGhostCHadrFromParentNotFromB;
   vector<int>     *jet_nGhostTauFromParent;
   vector<int>     *jet_nGhostHBosoFromParent;
   vector<int>     *jet_GhostL_q;
   vector<int>     *jet_GhostL_HadI;
   vector<int>     *jet_GhostL_HadF;
   vector<int>     *jet_LabDr_HadF;
   vector<int>     *jet_aliveAfterOR;
   vector<int>     *jet_aliveAfterORmu;
   vector<int>     *jet_truthMatch;
   vector<int>     *jet_isPU;
   vector<int>     *jet_isBadMedium;
   vector<float>   *jet_truthPt;
   vector<float>   *jet_dRiso;
   vector<float>   *jet_JVT;
   vector<float>   *jet_JVF;
   vector<float>   *jet_dRminToB;
   vector<float>   *jet_dRminToC;
   vector<float>   *jet_dRminToT;
   vector<float>   *jet_ip2d_pb;
   vector<float>   *jet_ip2d_pc;
   vector<float>   *jet_ip2d_pu;
   vector<float>   *jet_ip2d_llr;
   vector<float>   *jet_ip3d_pb;
   vector<float>   *jet_ip3d_pc;
   vector<float>   *jet_ip3d_pu;
   vector<float>   *jet_ip3d_llr;
   vector<float>   *jet_sv0_sig3d;
   vector<int>     *jet_sv0_ntrkj;
   vector<int>     *jet_sv0_ntrkv;
   vector<int>     *jet_sv0_n2t;
   vector<float>   *jet_sv0_m;
   vector<float>   *jet_sv0_efc;
   vector<float>   *jet_sv0_normdist;
   vector<int>     *jet_sv0_Nvtx;
   vector<vector<float> > *jet_sv0_vtx_x;
   vector<vector<float> > *jet_sv0_vtx_y;
   vector<vector<float> > *jet_sv0_vtx_z;
   vector<int>     *jet_sv1_ntrkj;
   vector<int>     *jet_sv1_ntrkv;
   vector<int>     *jet_sv1_n2t;
   vector<float>   *jet_sv1_m;
   vector<float>   *jet_sv1_efc;
   vector<float>   *jet_sv1_normdist;
   vector<float>   *jet_sv1_pb;
   vector<float>   *jet_sv1_pc;
   vector<float>   *jet_sv1_pu;
   vector<float>   *jet_sv1_llr;
   vector<int>     *jet_sv1_Nvtx;
   vector<float>   *jet_sv1_sig3d;
   vector<vector<float> > *jet_sv1_vtx_x;
   vector<vector<float> > *jet_sv1_vtx_y;
   vector<vector<float> > *jet_sv1_vtx_z;
   Float_t         PV_jf_x;
   Float_t         PV_jf_y;
   Float_t         PV_jf_z;
   vector<float>   *jet_jf_pb;
   vector<float>   *jet_jf_pc;
   vector<float>   *jet_jf_pu;
   vector<float>   *jet_jf_llr;
   vector<float>   *jet_jf_m;
   vector<float>   *jet_jf_mUncorr;
   vector<float>   *jet_jf_efc;
   vector<float>   *jet_jf_deta;
   vector<float>   *jet_jf_dphi;
   vector<float>   *jet_jf_dRFlightDir;
   vector<float>   *jet_jf_ntrkAtVx;
   vector<int>     *jet_jf_nvtx;
   vector<float>   *jet_jf_sig3d;
   vector<int>     *jet_jf_nvtx1t;
   vector<int>     *jet_jf_n2t;
   vector<int>     *jet_jf_VTXsize;
   vector<vector<float> > *jet_jf_vtx_chi2;
   vector<vector<float> > *jet_jf_vtx_ndf;
   vector<vector<int> > *jet_jf_vtx_ntrk;
   vector<vector<float> > *jet_jf_vtx_L3D;
   vector<vector<float> > *jet_jf_vtx_sig3D;
   vector<float>   *jet_jf_phi;
   vector<float>   *jet_jf_theta;
   vector<float>   *jet_jfcombnn_pb;
   vector<float>   *jet_jfcombnn_pc;
   vector<float>   *jet_jfcombnn_pu;
   vector<float>   *jet_jfcombnn_llr;
   vector<float>   *jet_dl1_pb;
   vector<float>   *jet_dl1_pc;
   vector<float>   *jet_dl1_pu;
   vector<double>  *jet_sv1ip3d;
   vector<double>  *jet_mv1;
   vector<double>  *jet_mv1c;
   vector<double>  *jet_mv2c00;
   vector<double>  *jet_mv2c10;
   vector<double>  *jet_mv2c20;
   vector<double>  *jet_mv2c100;
   vector<double>  *jet_mv2cl100;
   vector<double>  *jet_mv2m_pu;
   vector<double>  *jet_mv2m_pc;
   vector<double>  *jet_mv2m_pb;
   vector<double>  *jet_mvb;
   vector<vector<float> > *jet_exktsubjet_pt;
   vector<vector<float> > *jet_exktsubjet_eta;
   vector<vector<float> > *jet_exktsubjet_phi;
   vector<vector<float> > *jet_exktsubjet_m;
   vector<vector<int> > *jet_exktsubjet_ntrk;
   vector<vector<float> > *jet_exktsubjet_mv2c20;
   vector<double>  *jet_ExKtbb_Hbb_DoubleMV2c20;
   vector<double>  *jet_ExKtbb_Hbb_SingleMV2c20;
   vector<double>  *jet_ExKtbb_Hbb_MV2Only;
   vector<double>  *jet_ExKtbb_Hbb_MV2andJFDRSig;
   vector<double>  *jet_ExKtbb_Hbb_MV2andTopos;
   vector<float>   *bH1_pt;
   vector<float>   *bH1_eta;
   vector<float>   *bH1_phi;
   vector<float>   *bH1_Lxy;
   vector<float>   *bH1_x;
   vector<float>   *bH1_y;
   vector<float>   *bH1_z;
   vector<float>   *bH1_dRjet;
   vector<float>   *bH2_pt;
   vector<float>   *bH2_eta;
   vector<float>   *bH2_phi;
   vector<float>   *bH2_Lxy;
   vector<float>   *bH2_x;
   vector<float>   *bH2_y;
   vector<float>   *bH2_z;
   vector<float>   *bH2_dRjet;
   vector<float>   *bH_pt;
   vector<float>   *bH_eta;
   vector<float>   *bH_phi;
   vector<float>   *bH_Lxy;
   vector<float>   *bH_x;
   vector<float>   *bH_y;
   vector<float>   *bH_z;
   vector<float>   *bH_dRjet;
   vector<int>     *bH_nBtracks;
   vector<int>     *bH_nCtracks;
   vector<float>   *cH_pt;
   vector<float>   *cH_eta;
   vector<float>   *cH_phi;
   vector<float>   *cH_Lxy;
   vector<float>   *cH_x;
   vector<float>   *cH_y;
   vector<float>   *cH_z;
   vector<float>   *cH_dRjet;
   vector<int>     *cH_nCtracks;
   vector<int>     *jet_btag_ntrk;
   vector<vector<float> > *jet_trk_pt;
   vector<vector<float> > *jet_trk_eta;
   vector<vector<float> > *jet_trk_theta;
   vector<vector<float> > *jet_trk_phi;
   vector<vector<float> > *jet_trk_dr;
   vector<vector<int> > *jet_trk_assoc_msv;
   vector<vector<float> > *jet_trk_chi2;
   vector<vector<float> > *jet_trk_ndf;
   vector<vector<int> > *jet_trk_algo;
   vector<vector<int> > *jet_trk_orig;
   vector<vector<float> > *jet_trk_vtx_X;
   vector<vector<float> > *jet_trk_vtx_Y;
   vector<vector<int> > *jet_trk_nInnHits;
   vector<vector<int> > *jet_trk_nNextToInnHits;
   vector<vector<int> > *jet_trk_nBLHits;
   vector<vector<int> > *jet_trk_nsharedBLHits;
   vector<vector<int> > *jet_trk_nsplitBLHits;
   vector<vector<int> > *jet_trk_nPixHits;
   vector<vector<int> > *jet_trk_nsharedPixHits;
   vector<vector<int> > *jet_trk_nsplitPixHits;
   vector<vector<int> > *jet_trk_nSCTHits;
   vector<vector<int> > *jet_trk_nsharedSCTHits;
   vector<vector<int> > *jet_trk_expectBLayerHit;
   vector<vector<float> > *jet_trk_d0;
   vector<vector<float> > *jet_trk_z0;
   vector<vector<float> > *jet_trk_d0_truth;
   vector<vector<float> > *jet_trk_z0_truth;
   vector<vector<int> > *jet_trk_ip3d_grade;
   vector<vector<float> > *jet_trk_ip3d_d0;
   vector<vector<float> > *jet_trk_ip3d_d02D;
   vector<vector<float> > *jet_trk_ip3d_z0;
   vector<vector<float> > *jet_trk_ip3d_d0sig;
   vector<vector<float> > *jet_trk_ip3d_z0sig;
   vector<vector<float> > *jet_trk_ip2d_llr;
   vector<vector<float> > *jet_trk_ip3d_llr;
   vector<vector<int> > *jet_trk_jf_Vertex;
   vector<int>     *jet_sv1_ntrk;
   vector<int>     *jet_ip3d_ntrk;
   vector<int>     *jet_jf_ntrk;
   vector<float>   *jet_width;
   vector<int>     *jet_n_trk_sigd0cut;
   vector<float>   *jet_trk3_d0sig;
   vector<float>   *jet_trk3_z0sig;
   vector<float>   *jet_sv_scaled_efc;
   vector<float>   *jet_jf_scaled_efc;
   vector<vector<float> > *jet_trkjet_pt;
   vector<vector<float> > *jet_trkjet_eta;
   vector<vector<float> > *jet_trkjet_phi;
   vector<vector<float> > *jet_trkjet_m;
   vector<vector<int> > *jet_trkjet_ntrk;
   vector<vector<double> > *jet_trkjet_mv2c20;

   // List of branches
   TBranch        *b_runnb;   //!
   TBranch        *b_eventnb;   //!
   TBranch        *b_mcchan;   //!
   TBranch        *b_mcwg;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_coreFlag;   //!
   TBranch        *b_larError;   //!
   TBranch        *b_tileError;   //!
   TBranch        *b_nPV;   //!
   TBranch        *b_avgmu;   //!
   TBranch        *b_actmu;   //!
   TBranch        *b_PVx;   //!
   TBranch        *b_PVy;   //!
   TBranch        *b_PVz;   //!
   TBranch        *b_truth_PVx;   //!
   TBranch        *b_truth_PVy;   //!
   TBranch        *b_truth_PVz;   //!
   TBranch        *b_truth_LeadJet_pt;   //!
   TBranch        *b_njets;   //!
   TBranch        *b_nbjets;   //!
   TBranch        *b_nbjets_q;   //!
   TBranch        *b_nbjets_HadI;   //!
   TBranch        *b_nbjets_HadF;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_pt_orig;   //!
   TBranch        *b_jet_eta_orig;   //!
   TBranch        *b_jet_phi_orig;   //!
   TBranch        *b_jet_E_orig;   //!
   TBranch        *b_jet_sumtrkS_pt;   //!
   TBranch        *b_jet_sumtrkV_pt;   //!
   TBranch        *b_jet_sumtrkV_eta;   //!
   TBranch        *b_jet_sumtrkV_phi;   //!
   TBranch        *b_jet_sumtrk_ntrk;   //!
   TBranch        *b_jet_E;   //!
   TBranch        *b_jet_m;   //!
   TBranch        *b_jet_nConst;   //!
   TBranch        *b_jet_truthflav;   //!
   TBranch        *b_jet_nBHadr;   //!
   TBranch        *b_jet_nCHadr;   //!
   TBranch        *b_jet_nGhostBHadrFromParent;   //!
   TBranch        *b_jet_nGhostCHadrFromParent;   //!
   TBranch        *b_jet_nGhostCHadrFromParentNotFromB;   //!
   TBranch        *b_jet_nGhostTauFromParent;   //!
   TBranch        *b_jet_nGhostHBosoFromParent;   //!
   TBranch        *b_jet_GhostL_q;   //!
   TBranch        *b_jet_GhostL_HadI;   //!
   TBranch        *b_jet_GhostL_HadF;   //!
   TBranch        *b_jet_LabDr_HadF;   //!
   TBranch        *b_jet_aliveAfterOR;   //!
   TBranch        *b_jet_aliveAfterORmu;   //!
   TBranch        *b_jet_truthMatch;   //!
   TBranch        *b_jet_isPU;   //!
   TBranch        *b_jet_isBadMedium;   //!
   TBranch        *b_jet_truthPt;   //!
   TBranch        *b_jet_dRiso;   //!
   TBranch        *b_jet_JVT;   //!
   TBranch        *b_jet_JVF;   //!
   TBranch        *b_jet_dRminToB;   //!
   TBranch        *b_jet_dRminToC;   //!
   TBranch        *b_jet_dRminToT;   //!
   TBranch        *b_jet_ip2d_pb;   //!
   TBranch        *b_jet_ip2d_pc;   //!
   TBranch        *b_jet_ip2d_pu;   //!
   TBranch        *b_jet_ip2d_llr;   //!
   TBranch        *b_jet_ip3d_pb;   //!
   TBranch        *b_jet_ip3d_pc;   //!
   TBranch        *b_jet_ip3d_pu;   //!
   TBranch        *b_jet_ip3d_llr;   //!
   TBranch        *b_jet_sv0_sig3d;   //!
   TBranch        *b_jet_sv0_ntrkj;   //!
   TBranch        *b_jet_sv0_ntrkv;   //!
   TBranch        *b_jet_sv0_n2t;   //!
   TBranch        *b_jet_sv0_m;   //!
   TBranch        *b_jet_sv0_efc;   //!
   TBranch        *b_jet_sv0_normdist;   //!
   TBranch        *b_jet_sv0_Nvtx;   //!
   TBranch        *b_jet_sv0_vtx_x;   //!
   TBranch        *b_jet_sv0_vtx_y;   //!
   TBranch        *b_jet_sv0_vtx_z;   //!
   TBranch        *b_jet_sv1_ntrkj;   //!
   TBranch        *b_jet_sv1_ntrkv;   //!
   TBranch        *b_jet_sv1_n2t;   //!
   TBranch        *b_jet_sv1_m;   //!
   TBranch        *b_jet_sv1_efc;   //!
   TBranch        *b_jet_sv1_normdist;   //!
   TBranch        *b_jet_sv1_pb;   //!
   TBranch        *b_jet_sv1_pc;   //!
   TBranch        *b_jet_sv1_pu;   //!
   TBranch        *b_jet_sv1_llr;   //!
   TBranch        *b_jet_sv1_Nvtx;   //!
   TBranch        *b_jet_sv1_sig3d;   //!
   TBranch        *b_jet_sv1_vtx_x;   //!
   TBranch        *b_jet_sv1_vtx_y;   //!
   TBranch        *b_jet_sv1_vtx_z;   //!
   TBranch        *b_PV_jf_x;   //!
   TBranch        *b_PV_jf_y;   //!
   TBranch        *b_PV_jf_z;   //!
   TBranch        *b_jet_jf_pb;   //!
   TBranch        *b_jet_jf_pc;   //!
   TBranch        *b_jet_jf_pu;   //!
   TBranch        *b_jet_jf_llr;   //!
   TBranch        *b_jet_jf_m;   //!
   TBranch        *b_jet_jf_mUncorr;   //!
   TBranch        *b_jet_jf_efc;   //!
   TBranch        *b_jet_jf_deta;   //!
   TBranch        *b_jet_jf_dphi;   //!
   TBranch        *b_jet_jf_dRFlightDir;   //!
   TBranch        *b_jet_jf_ntrkAtVx;   //!
   TBranch        *b_jet_jf_nvtx;   //!
   TBranch        *b_jet_jf_sig3d;   //!
   TBranch        *b_jet_jf_nvtx1t;   //!
   TBranch        *b_jet_jf_n2t;   //!
   TBranch        *b_jet_jf_VTXsize;   //!
   TBranch        *b_jet_jf_vtx_chi2;   //!
   TBranch        *b_jet_jf_vtx_ndf;   //!
   TBranch        *b_jet_jf_vtx_ntrk;   //!
   TBranch        *b_jet_jf_vtx_L3D;   //!
   TBranch        *b_jet_jf_vtx_sig3D;   //!
   TBranch        *b_jet_jf_phi;   //!
   TBranch        *b_jet_jf_theta;   //!
   TBranch        *b_jet_jfcombnn_pb;   //!
   TBranch        *b_jet_jfcombnn_pc;   //!
   TBranch        *b_jet_jfcombnn_pu;   //!
   TBranch        *b_jet_jfcombnn_llr;   //!
   TBranch        *b_jet_dl1_pb;   //!
   TBranch        *b_jet_dl1_pc;   //!
   TBranch        *b_jet_dl1_pu;   //!
   TBranch        *b_jet_sv1ip3d;   //!
   TBranch        *b_jet_mv1;   //!
   TBranch        *b_jet_mv1c;   //!
   TBranch        *b_jet_mv2c00;   //!
   TBranch        *b_jet_mv2c10;   //!
   TBranch        *b_jet_mv2c20;   //!
   TBranch        *b_jet_mv2c100;   //!
   TBranch        *b_jet_mv2cl100;   //!
   TBranch        *b_jet_mv2m_pu;   //!
   TBranch        *b_jet_mv2m_pc;   //!
   TBranch        *b_jet_mv2m_pb;   //!
   TBranch        *b_jet_mvb;   //!
   TBranch        *b_jet_exktsubjet_pt;   //!
   TBranch        *b_jet_exktsubjet_eta;   //!
   TBranch        *b_jet_exktsubjet_phi;   //!
   TBranch        *b_jet_exktsubjet_m;   //!
   TBranch        *b_jet_exktsubjet_ntrk;   //!
   TBranch        *b_jet_exktsubjet_mv2c20;   //!
   TBranch        *b_jet_ExKtbb_Hbb_DoubleMV2c20;   //!
   TBranch        *b_jet_ExKtbb_Hbb_SingleMV2c20;   //!
   TBranch        *b_jet_ExKtbb_Hbb_MV2Only;   //!
   TBranch        *b_jet_ExKtbb_Hbb_MV2andJFDRSig;   //!
   TBranch        *b_jet_ExKtbb_Hbb_MV2andTopos;   //!
   TBranch        *b_bH1_pt;   //!
   TBranch        *b_bH1_eta;   //!
   TBranch        *b_bH1_phi;   //!
   TBranch        *b_bH1_Lxy;   //!
   TBranch        *b_bH1_x;   //!
   TBranch        *b_bH1_y;   //!
   TBranch        *b_bH1_z;   //!
   TBranch        *b_bH1_dRjet;   //!
   TBranch        *b_bH2_pt;   //!
   TBranch        *b_bH2_eta;   //!
   TBranch        *b_bH2_phi;   //!
   TBranch        *b_bH2_Lxy;   //!
   TBranch        *b_bH2_x;   //!
   TBranch        *b_bH2_y;   //!
   TBranch        *b_bH2_z;   //!
   TBranch        *b_bH2_dRjet;   //!
   TBranch        *b_bH_pt;   //!
   TBranch        *b_bH_eta;   //!
   TBranch        *b_bH_phi;   //!
   TBranch        *b_bH_Lxy;   //!
   TBranch        *b_bH_x;   //!
   TBranch        *b_bH_y;   //!
   TBranch        *b_bH_z;   //!
   TBranch        *b_bH_dRjet;   //!
   TBranch        *b_bH_nBtracks;   //!
   TBranch        *b_bH_nCtracks;   //!
   TBranch        *b_cH_pt;   //!
   TBranch        *b_cH_eta;   //!
   TBranch        *b_cH_phi;   //!
   TBranch        *b_cH_Lxy;   //!
   TBranch        *b_cH_x;   //!
   TBranch        *b_cH_y;   //!
   TBranch        *b_cH_z;   //!
   TBranch        *b_cH_dRjet;   //!
   TBranch        *b_cH_nCtracks;   //!
   TBranch        *b_jet_btag_ntrk;   //!
   TBranch        *b_jet_trk_pt;   //!
   TBranch        *b_jet_trk_eta;   //!
   TBranch        *b_jet_trk_theta;   //!
   TBranch        *b_jet_trk_phi;   //!
   TBranch        *b_jet_trk_dr;   //!
   TBranch        *b_jet_trk_assoc_msv;   //!
   TBranch        *b_jet_trk_chi2;   //!
   TBranch        *b_jet_trk_ndf;   //!
   TBranch        *b_jet_trk_algo;   //!
   TBranch        *b_jet_trk_orig;   //!
   TBranch        *b_jet_trk_vtx_X;   //!
   TBranch        *b_jet_trk_vtx_Y;   //!
   TBranch        *b_jet_trk_nInnHits;   //!
   TBranch        *b_jet_trk_nNextToInnHits;   //!
   TBranch        *b_jet_trk_nBLHits;   //!
   TBranch        *b_jet_trk_nsharedBLHits;   //!
   TBranch        *b_jet_trk_nsplitBLHits;   //!
   TBranch        *b_jet_trk_nPixHits;   //!
   TBranch        *b_jet_trk_nsharedPixHits;   //!
   TBranch        *b_jet_trk_nsplitPixHits;   //!
   TBranch        *b_jet_trk_nSCTHits;   //!
   TBranch        *b_jet_trk_nsharedSCTHits;   //!
   TBranch        *b_jet_trk_expectBLayerHit;   //!
   TBranch        *b_jet_trk_d0;   //!
   TBranch        *b_jet_trk_z0;   //!
   TBranch        *b_jet_trk_d0_truth;   //!
   TBranch        *b_jet_trk_z0_truth;   //!
   TBranch        *b_jet_trk_ip3d_grade;   //!
   TBranch        *b_jet_trk_ip3d_d0;   //!
   TBranch        *b_jet_trk_ip3d_d02D;   //!
   TBranch        *b_jet_trk_ip3d_z0;   //!
   TBranch        *b_jet_trk_ip3d_d0sig;   //!
   TBranch        *b_jet_trk_ip3d_z0sig;   //!
   TBranch        *b_jet_trk_ip2d_llr;   //!
   TBranch        *b_jet_trk_ip3d_llr;   //!
   TBranch        *b_jet_trk_jf_Vertex;   //!
   TBranch        *b_jet_sv1_ntrk;   //!
   TBranch        *b_jet_ip3d_ntrk;   //!
   TBranch        *b_jet_jf_ntrk;   //!
   TBranch        *b_jet_width;   //!
   TBranch        *b_jet_n_trk_sigd0cut;   //!
   TBranch        *b_jet_trk3_d0sig;   //!
   TBranch        *b_jet_trk3_z0sig;   //!
   TBranch        *b_jet_sv_scaled_efc;   //!
   TBranch        *b_jet_jf_scaled_efc;   //!
   TBranch        *b_jet_trkjet_pt;   //!
   TBranch        *b_jet_trkjet_eta;   //!
   TBranch        *b_jet_trkjet_phi;   //!
   TBranch        *b_jet_trkjet_m;   //!
   TBranch        *b_jet_trkjet_ntrk;   //!
   TBranch        *b_jet_trkjet_mv2c20;   //!

   test(TTree *tree=0);
   virtual ~test();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);


   // My declarations.
   // INITIALISATIONS //

   // We'll use a TNtuple instead of a TTree.
   // TNtuples are a bit easier to set up as they
   // only store doubles.
   TNtuple *outputtree;
   TFile *outputfile;

   // This is for getting the tree name.
   // It isn't actually needed as fReader
   // does all the work.
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

};

#endif


// IMPLEMENTATION starts here

#ifdef test_cxx
test::test(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("group.perf-flavtag.8324358.Akt4EMTo._002487.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("group.perf-flavtag.8324358.Akt4EMTo._002487.root");
      }
      f->GetObject("bTag_AntiKt4EMTopoJets",tree);
   }

   tree->SetName("processtree");
   Init(tree);
}

test::~test()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t test::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t test::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void test::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jet_pt = 0;
   jet_eta = 0;
   jet_phi = 0;
   jet_pt_orig = 0;
   jet_eta_orig = 0;
   jet_phi_orig = 0;
   jet_E_orig = 0;
   jet_sumtrkS_pt = 0;
   jet_sumtrkV_pt = 0;
   jet_sumtrkV_eta = 0;
   jet_sumtrkV_phi = 0;
   jet_sumtrk_ntrk = 0;
   jet_E = 0;
   jet_m = 0;
   jet_nConst = 0;
   jet_truthflav = 0;
   jet_nBHadr = 0;
   jet_nCHadr = 0;
   jet_nGhostBHadrFromParent = 0;
   jet_nGhostCHadrFromParent = 0;
   jet_nGhostCHadrFromParentNotFromB = 0;
   jet_nGhostTauFromParent = 0;
   jet_nGhostHBosoFromParent = 0;
   jet_GhostL_q = 0;
   jet_GhostL_HadI = 0;
   jet_GhostL_HadF = 0;
   jet_LabDr_HadF = 0;
   jet_aliveAfterOR = 0;
   jet_aliveAfterORmu = 0;
   jet_truthMatch = 0;
   jet_isPU = 0;
   jet_isBadMedium = 0;
   jet_truthPt = 0;
   jet_dRiso = 0;
   jet_JVT = 0;
   jet_JVF = 0;
   jet_dRminToB = 0;
   jet_dRminToC = 0;
   jet_dRminToT = 0;
   jet_ip2d_pb = 0;
   jet_ip2d_pc = 0;
   jet_ip2d_pu = 0;
   jet_ip2d_llr = 0;
   jet_ip3d_pb = 0;
   jet_ip3d_pc = 0;
   jet_ip3d_pu = 0;
   jet_ip3d_llr = 0;
   jet_sv0_sig3d = 0;
   jet_sv0_ntrkj = 0;
   jet_sv0_ntrkv = 0;
   jet_sv0_n2t = 0;
   jet_sv0_m = 0;
   jet_sv0_efc = 0;
   jet_sv0_normdist = 0;
   jet_sv0_Nvtx = 0;
   jet_sv0_vtx_x = 0;
   jet_sv0_vtx_y = 0;
   jet_sv0_vtx_z = 0;
   jet_sv1_ntrkj = 0;
   jet_sv1_ntrkv = 0;
   jet_sv1_n2t = 0;
   jet_sv1_m = 0;
   jet_sv1_efc = 0;
   jet_sv1_normdist = 0;
   jet_sv1_pb = 0;
   jet_sv1_pc = 0;
   jet_sv1_pu = 0;
   jet_sv1_llr = 0;
   jet_sv1_Nvtx = 0;
   jet_sv1_sig3d = 0;
   jet_sv1_vtx_x = 0;
   jet_sv1_vtx_y = 0;
   jet_sv1_vtx_z = 0;
   jet_jf_pb = 0;
   jet_jf_pc = 0;
   jet_jf_pu = 0;
   jet_jf_llr = 0;
   jet_jf_m = 0;
   jet_jf_mUncorr = 0;
   jet_jf_efc = 0;
   jet_jf_deta = 0;
   jet_jf_dphi = 0;
   jet_jf_dRFlightDir = 0;
   jet_jf_ntrkAtVx = 0;
   jet_jf_nvtx = 0;
   jet_jf_sig3d = 0;
   jet_jf_nvtx1t = 0;
   jet_jf_n2t = 0;
   jet_jf_VTXsize = 0;
   jet_jf_vtx_chi2 = 0;
   jet_jf_vtx_ndf = 0;
   jet_jf_vtx_ntrk = 0;
   jet_jf_vtx_L3D = 0;
   jet_jf_vtx_sig3D = 0;
   jet_jf_phi = 0;
   jet_jf_theta = 0;
   jet_jfcombnn_pb = 0;
   jet_jfcombnn_pc = 0;
   jet_jfcombnn_pu = 0;
   jet_jfcombnn_llr = 0;
   jet_dl1_pb = 0;
   jet_dl1_pc = 0;
   jet_dl1_pu = 0;
   jet_sv1ip3d = 0;
   jet_mv1 = 0;
   jet_mv1c = 0;
   jet_mv2c00 = 0;
   jet_mv2c10 = 0;
   jet_mv2c20 = 0;
   jet_mv2c100 = 0;
   jet_mv2cl100 = 0;
   jet_mv2m_pu = 0;
   jet_mv2m_pc = 0;
   jet_mv2m_pb = 0;
   jet_mvb = 0;
   jet_exktsubjet_pt = 0;
   jet_exktsubjet_eta = 0;
   jet_exktsubjet_phi = 0;
   jet_exktsubjet_m = 0;
   jet_exktsubjet_ntrk = 0;
   jet_exktsubjet_mv2c20 = 0;
   jet_ExKtbb_Hbb_DoubleMV2c20 = 0;
   jet_ExKtbb_Hbb_SingleMV2c20 = 0;
   jet_ExKtbb_Hbb_MV2Only = 0;
   jet_ExKtbb_Hbb_MV2andJFDRSig = 0;
   jet_ExKtbb_Hbb_MV2andTopos = 0;
   bH1_pt = 0;
   bH1_eta = 0;
   bH1_phi = 0;
   bH1_Lxy = 0;
   bH1_x = 0;
   bH1_y = 0;
   bH1_z = 0;
   bH1_dRjet = 0;
   bH2_pt = 0;
   bH2_eta = 0;
   bH2_phi = 0;
   bH2_Lxy = 0;
   bH2_x = 0;
   bH2_y = 0;
   bH2_z = 0;
   bH2_dRjet = 0;
   bH_pt = 0;
   bH_eta = 0;
   bH_phi = 0;
   bH_Lxy = 0;
   bH_x = 0;
   bH_y = 0;
   bH_z = 0;
   bH_dRjet = 0;
   bH_nBtracks = 0;
   bH_nCtracks = 0;
   cH_pt = 0;
   cH_eta = 0;
   cH_phi = 0;
   cH_Lxy = 0;
   cH_x = 0;
   cH_y = 0;
   cH_z = 0;
   cH_dRjet = 0;
   cH_nCtracks = 0;
   jet_btag_ntrk = 0;
   jet_trk_pt = 0;
   jet_trk_eta = 0;
   jet_trk_theta = 0;
   jet_trk_phi = 0;
   jet_trk_dr = 0;
   jet_trk_assoc_msv = 0;
   jet_trk_chi2 = 0;
   jet_trk_ndf = 0;
   jet_trk_algo = 0;
   jet_trk_orig = 0;
   jet_trk_vtx_X = 0;
   jet_trk_vtx_Y = 0;
   jet_trk_nInnHits = 0;
   jet_trk_nNextToInnHits = 0;
   jet_trk_nBLHits = 0;
   jet_trk_nsharedBLHits = 0;
   jet_trk_nsplitBLHits = 0;
   jet_trk_nPixHits = 0;
   jet_trk_nsharedPixHits = 0;
   jet_trk_nsplitPixHits = 0;
   jet_trk_nSCTHits = 0;
   jet_trk_nsharedSCTHits = 0;
   jet_trk_expectBLayerHit = 0;
   jet_trk_d0 = 0;
   jet_trk_z0 = 0;
   jet_trk_d0_truth = 0;
   jet_trk_z0_truth = 0;
   jet_trk_ip3d_grade = 0;
   jet_trk_ip3d_d0 = 0;
   jet_trk_ip3d_d02D = 0;
   jet_trk_ip3d_z0 = 0;
   jet_trk_ip3d_d0sig = 0;
   jet_trk_ip3d_z0sig = 0;
   jet_trk_ip2d_llr = 0;
   jet_trk_ip3d_llr = 0;
   jet_trk_jf_Vertex = 0;
   jet_sv1_ntrk = 0;
   jet_ip3d_ntrk = 0;
   jet_jf_ntrk = 0;
   jet_width = 0;
   jet_n_trk_sigd0cut = 0;
   jet_trk3_d0sig = 0;
   jet_trk3_z0sig = 0;
   jet_sv_scaled_efc = 0;
   jet_jf_scaled_efc = 0;
   jet_trkjet_pt = 0;
   jet_trkjet_eta = 0;
   jet_trkjet_phi = 0;
   jet_trkjet_m = 0;
   jet_trkjet_ntrk = 0;
   jet_trkjet_mv2c20 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runnb", &runnb, &b_runnb);
   fChain->SetBranchAddress("eventnb", &eventnb, &b_eventnb);
   fChain->SetBranchAddress("mcchan", &mcchan, &b_mcchan);
   fChain->SetBranchAddress("mcwg", &mcwg, &b_mcwg);
   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("coreFlag", &coreFlag, &b_coreFlag);
   fChain->SetBranchAddress("larError", &larError, &b_larError);
   fChain->SetBranchAddress("tileError", &tileError, &b_tileError);
   fChain->SetBranchAddress("nPV", &nPV, &b_nPV);
   fChain->SetBranchAddress("avgmu", &avgmu, &b_avgmu);
   fChain->SetBranchAddress("actmu", &actmu, &b_actmu);
   fChain->SetBranchAddress("PVx", &PVx, &b_PVx);
   fChain->SetBranchAddress("PVy", &PVy, &b_PVy);
   fChain->SetBranchAddress("PVz", &PVz, &b_PVz);
   fChain->SetBranchAddress("truth_PVx", &truth_PVx, &b_truth_PVx);
   fChain->SetBranchAddress("truth_PVy", &truth_PVy, &b_truth_PVy);
   fChain->SetBranchAddress("truth_PVz", &truth_PVz, &b_truth_PVz);
   fChain->SetBranchAddress("truth_LeadJet_pt", &truth_LeadJet_pt, &b_truth_LeadJet_pt);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("nbjets", &nbjets, &b_nbjets);
   fChain->SetBranchAddress("nbjets_q", &nbjets_q, &b_nbjets_q);
   fChain->SetBranchAddress("nbjets_HadI", &nbjets_HadI, &b_nbjets_HadI);
   fChain->SetBranchAddress("nbjets_HadF", &nbjets_HadF, &b_nbjets_HadF);
   fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_pt_orig", &jet_pt_orig, &b_jet_pt_orig);
   fChain->SetBranchAddress("jet_eta_orig", &jet_eta_orig, &b_jet_eta_orig);
   fChain->SetBranchAddress("jet_phi_orig", &jet_phi_orig, &b_jet_phi_orig);
   fChain->SetBranchAddress("jet_E_orig", &jet_E_orig, &b_jet_E_orig);
   fChain->SetBranchAddress("jet_sumtrkS_pt", &jet_sumtrkS_pt, &b_jet_sumtrkS_pt);
   fChain->SetBranchAddress("jet_sumtrkV_pt", &jet_sumtrkV_pt, &b_jet_sumtrkV_pt);
   fChain->SetBranchAddress("jet_sumtrkV_eta", &jet_sumtrkV_eta, &b_jet_sumtrkV_eta);
   fChain->SetBranchAddress("jet_sumtrkV_phi", &jet_sumtrkV_phi, &b_jet_sumtrkV_phi);
   fChain->SetBranchAddress("jet_sumtrk_ntrk", &jet_sumtrk_ntrk, &b_jet_sumtrk_ntrk);
   fChain->SetBranchAddress("jet_E", &jet_E, &b_jet_E);
   fChain->SetBranchAddress("jet_m", &jet_m, &b_jet_m);
   fChain->SetBranchAddress("jet_nConst", &jet_nConst, &b_jet_nConst);
   fChain->SetBranchAddress("jet_truthflav", &jet_truthflav, &b_jet_truthflav);
   fChain->SetBranchAddress("jet_nBHadr", &jet_nBHadr, &b_jet_nBHadr);
   fChain->SetBranchAddress("jet_nCHadr", &jet_nCHadr, &b_jet_nCHadr);
   fChain->SetBranchAddress("jet_nGhostBHadrFromParent", &jet_nGhostBHadrFromParent, &b_jet_nGhostBHadrFromParent);
   fChain->SetBranchAddress("jet_nGhostCHadrFromParent", &jet_nGhostCHadrFromParent, &b_jet_nGhostCHadrFromParent);
   fChain->SetBranchAddress("jet_nGhostCHadrFromParentNotFromB", &jet_nGhostCHadrFromParentNotFromB, &b_jet_nGhostCHadrFromParentNotFromB);
   fChain->SetBranchAddress("jet_nGhostTauFromParent", &jet_nGhostTauFromParent, &b_jet_nGhostTauFromParent);
   fChain->SetBranchAddress("jet_nGhostHBosoFromParent", &jet_nGhostHBosoFromParent, &b_jet_nGhostHBosoFromParent);
   fChain->SetBranchAddress("jet_GhostL_q", &jet_GhostL_q, &b_jet_GhostL_q);
   fChain->SetBranchAddress("jet_GhostL_HadI", &jet_GhostL_HadI, &b_jet_GhostL_HadI);
   fChain->SetBranchAddress("jet_GhostL_HadF", &jet_GhostL_HadF, &b_jet_GhostL_HadF);
   fChain->SetBranchAddress("jet_LabDr_HadF", &jet_LabDr_HadF, &b_jet_LabDr_HadF);
   fChain->SetBranchAddress("jet_aliveAfterOR", &jet_aliveAfterOR, &b_jet_aliveAfterOR);
   fChain->SetBranchAddress("jet_aliveAfterORmu", &jet_aliveAfterORmu, &b_jet_aliveAfterORmu);
   fChain->SetBranchAddress("jet_truthMatch", &jet_truthMatch, &b_jet_truthMatch);
   fChain->SetBranchAddress("jet_isPU", &jet_isPU, &b_jet_isPU);
   fChain->SetBranchAddress("jet_isBadMedium", &jet_isBadMedium, &b_jet_isBadMedium);
   fChain->SetBranchAddress("jet_truthPt", &jet_truthPt, &b_jet_truthPt);
   fChain->SetBranchAddress("jet_dRiso", &jet_dRiso, &b_jet_dRiso);
   fChain->SetBranchAddress("jet_JVT", &jet_JVT, &b_jet_JVT);
   fChain->SetBranchAddress("jet_JVF", &jet_JVF, &b_jet_JVF);
   fChain->SetBranchAddress("jet_dRminToB", &jet_dRminToB, &b_jet_dRminToB);
   fChain->SetBranchAddress("jet_dRminToC", &jet_dRminToC, &b_jet_dRminToC);
   fChain->SetBranchAddress("jet_dRminToT", &jet_dRminToT, &b_jet_dRminToT);
   fChain->SetBranchAddress("jet_ip2d_pb", &jet_ip2d_pb, &b_jet_ip2d_pb);
   fChain->SetBranchAddress("jet_ip2d_pc", &jet_ip2d_pc, &b_jet_ip2d_pc);
   fChain->SetBranchAddress("jet_ip2d_pu", &jet_ip2d_pu, &b_jet_ip2d_pu);
   fChain->SetBranchAddress("jet_ip2d_llr", &jet_ip2d_llr, &b_jet_ip2d_llr);
   fChain->SetBranchAddress("jet_ip3d_pb", &jet_ip3d_pb, &b_jet_ip3d_pb);
   fChain->SetBranchAddress("jet_ip3d_pc", &jet_ip3d_pc, &b_jet_ip3d_pc);
   fChain->SetBranchAddress("jet_ip3d_pu", &jet_ip3d_pu, &b_jet_ip3d_pu);
   fChain->SetBranchAddress("jet_ip3d_llr", &jet_ip3d_llr, &b_jet_ip3d_llr);
   fChain->SetBranchAddress("jet_sv0_sig3d", &jet_sv0_sig3d, &b_jet_sv0_sig3d);
   fChain->SetBranchAddress("jet_sv0_ntrkj", &jet_sv0_ntrkj, &b_jet_sv0_ntrkj);
   fChain->SetBranchAddress("jet_sv0_ntrkv", &jet_sv0_ntrkv, &b_jet_sv0_ntrkv);
   fChain->SetBranchAddress("jet_sv0_n2t", &jet_sv0_n2t, &b_jet_sv0_n2t);
   fChain->SetBranchAddress("jet_sv0_m", &jet_sv0_m, &b_jet_sv0_m);
   fChain->SetBranchAddress("jet_sv0_efc", &jet_sv0_efc, &b_jet_sv0_efc);
   fChain->SetBranchAddress("jet_sv0_normdist", &jet_sv0_normdist, &b_jet_sv0_normdist);
   fChain->SetBranchAddress("jet_sv0_Nvtx", &jet_sv0_Nvtx, &b_jet_sv0_Nvtx);
   fChain->SetBranchAddress("jet_sv0_vtx_x", &jet_sv0_vtx_x, &b_jet_sv0_vtx_x);
   fChain->SetBranchAddress("jet_sv0_vtx_y", &jet_sv0_vtx_y, &b_jet_sv0_vtx_y);
   fChain->SetBranchAddress("jet_sv0_vtx_z", &jet_sv0_vtx_z, &b_jet_sv0_vtx_z);
   fChain->SetBranchAddress("jet_sv1_ntrkj", &jet_sv1_ntrkj, &b_jet_sv1_ntrkj);
   fChain->SetBranchAddress("jet_sv1_ntrkv", &jet_sv1_ntrkv, &b_jet_sv1_ntrkv);
   fChain->SetBranchAddress("jet_sv1_n2t", &jet_sv1_n2t, &b_jet_sv1_n2t);
   fChain->SetBranchAddress("jet_sv1_m", &jet_sv1_m, &b_jet_sv1_m);
   fChain->SetBranchAddress("jet_sv1_efc", &jet_sv1_efc, &b_jet_sv1_efc);
   fChain->SetBranchAddress("jet_sv1_normdist", &jet_sv1_normdist, &b_jet_sv1_normdist);
   fChain->SetBranchAddress("jet_sv1_pb", &jet_sv1_pb, &b_jet_sv1_pb);
   fChain->SetBranchAddress("jet_sv1_pc", &jet_sv1_pc, &b_jet_sv1_pc);
   fChain->SetBranchAddress("jet_sv1_pu", &jet_sv1_pu, &b_jet_sv1_pu);
   fChain->SetBranchAddress("jet_sv1_llr", &jet_sv1_llr, &b_jet_sv1_llr);
   fChain->SetBranchAddress("jet_sv1_Nvtx", &jet_sv1_Nvtx, &b_jet_sv1_Nvtx);
   fChain->SetBranchAddress("jet_sv1_sig3d", &jet_sv1_sig3d, &b_jet_sv1_sig3d);
   fChain->SetBranchAddress("jet_sv1_vtx_x", &jet_sv1_vtx_x, &b_jet_sv1_vtx_x);
   fChain->SetBranchAddress("jet_sv1_vtx_y", &jet_sv1_vtx_y, &b_jet_sv1_vtx_y);
   fChain->SetBranchAddress("jet_sv1_vtx_z", &jet_sv1_vtx_z, &b_jet_sv1_vtx_z);
   fChain->SetBranchAddress("PV_jf_x", &PV_jf_x, &b_PV_jf_x);
   fChain->SetBranchAddress("PV_jf_y", &PV_jf_y, &b_PV_jf_y);
   fChain->SetBranchAddress("PV_jf_z", &PV_jf_z, &b_PV_jf_z);
   fChain->SetBranchAddress("jet_jf_pb", &jet_jf_pb, &b_jet_jf_pb);
   fChain->SetBranchAddress("jet_jf_pc", &jet_jf_pc, &b_jet_jf_pc);
   fChain->SetBranchAddress("jet_jf_pu", &jet_jf_pu, &b_jet_jf_pu);
   fChain->SetBranchAddress("jet_jf_llr", &jet_jf_llr, &b_jet_jf_llr);
   fChain->SetBranchAddress("jet_jf_m", &jet_jf_m, &b_jet_jf_m);
   fChain->SetBranchAddress("jet_jf_mUncorr", &jet_jf_mUncorr, &b_jet_jf_mUncorr);
   fChain->SetBranchAddress("jet_jf_efc", &jet_jf_efc, &b_jet_jf_efc);
   fChain->SetBranchAddress("jet_jf_deta", &jet_jf_deta, &b_jet_jf_deta);
   fChain->SetBranchAddress("jet_jf_dphi", &jet_jf_dphi, &b_jet_jf_dphi);
   fChain->SetBranchAddress("jet_jf_dRFlightDir", &jet_jf_dRFlightDir, &b_jet_jf_dRFlightDir);
   fChain->SetBranchAddress("jet_jf_ntrkAtVx", &jet_jf_ntrkAtVx, &b_jet_jf_ntrkAtVx);
   fChain->SetBranchAddress("jet_jf_nvtx", &jet_jf_nvtx, &b_jet_jf_nvtx);
   fChain->SetBranchAddress("jet_jf_sig3d", &jet_jf_sig3d, &b_jet_jf_sig3d);
   fChain->SetBranchAddress("jet_jf_nvtx1t", &jet_jf_nvtx1t, &b_jet_jf_nvtx1t);
   fChain->SetBranchAddress("jet_jf_n2t", &jet_jf_n2t, &b_jet_jf_n2t);
   fChain->SetBranchAddress("jet_jf_VTXsize", &jet_jf_VTXsize, &b_jet_jf_VTXsize);
   fChain->SetBranchAddress("jet_jf_vtx_chi2", &jet_jf_vtx_chi2, &b_jet_jf_vtx_chi2);
   fChain->SetBranchAddress("jet_jf_vtx_ndf", &jet_jf_vtx_ndf, &b_jet_jf_vtx_ndf);
   fChain->SetBranchAddress("jet_jf_vtx_ntrk", &jet_jf_vtx_ntrk, &b_jet_jf_vtx_ntrk);
   fChain->SetBranchAddress("jet_jf_vtx_L3D", &jet_jf_vtx_L3D, &b_jet_jf_vtx_L3D);
   fChain->SetBranchAddress("jet_jf_vtx_sig3D", &jet_jf_vtx_sig3D, &b_jet_jf_vtx_sig3D);
   fChain->SetBranchAddress("jet_jf_phi", &jet_jf_phi, &b_jet_jf_phi);
   fChain->SetBranchAddress("jet_jf_theta", &jet_jf_theta, &b_jet_jf_theta);
   fChain->SetBranchAddress("jet_jfcombnn_pb", &jet_jfcombnn_pb, &b_jet_jfcombnn_pb);
   fChain->SetBranchAddress("jet_jfcombnn_pc", &jet_jfcombnn_pc, &b_jet_jfcombnn_pc);
   fChain->SetBranchAddress("jet_jfcombnn_pu", &jet_jfcombnn_pu, &b_jet_jfcombnn_pu);
   fChain->SetBranchAddress("jet_jfcombnn_llr", &jet_jfcombnn_llr, &b_jet_jfcombnn_llr);
   fChain->SetBranchAddress("jet_dl1_pb", &jet_dl1_pb, &b_jet_dl1_pb);
   fChain->SetBranchAddress("jet_dl1_pc", &jet_dl1_pc, &b_jet_dl1_pc);
   fChain->SetBranchAddress("jet_dl1_pu", &jet_dl1_pu, &b_jet_dl1_pu);
   fChain->SetBranchAddress("jet_sv1ip3d", &jet_sv1ip3d, &b_jet_sv1ip3d);
   fChain->SetBranchAddress("jet_mv1", &jet_mv1, &b_jet_mv1);
   fChain->SetBranchAddress("jet_mv1c", &jet_mv1c, &b_jet_mv1c);
   fChain->SetBranchAddress("jet_mv2c00", &jet_mv2c00, &b_jet_mv2c00);
   fChain->SetBranchAddress("jet_mv2c10", &jet_mv2c10, &b_jet_mv2c10);
   fChain->SetBranchAddress("jet_mv2c20", &jet_mv2c20, &b_jet_mv2c20);
   fChain->SetBranchAddress("jet_mv2c100", &jet_mv2c100, &b_jet_mv2c100);
   fChain->SetBranchAddress("jet_mv2cl100", &jet_mv2cl100, &b_jet_mv2cl100);
   fChain->SetBranchAddress("jet_mv2m_pu", &jet_mv2m_pu, &b_jet_mv2m_pu);
   fChain->SetBranchAddress("jet_mv2m_pc", &jet_mv2m_pc, &b_jet_mv2m_pc);
   fChain->SetBranchAddress("jet_mv2m_pb", &jet_mv2m_pb, &b_jet_mv2m_pb);
   fChain->SetBranchAddress("jet_mvb", &jet_mvb, &b_jet_mvb);
   fChain->SetBranchAddress("jet_exktsubjet_pt", &jet_exktsubjet_pt, &b_jet_exktsubjet_pt);
   fChain->SetBranchAddress("jet_exktsubjet_eta", &jet_exktsubjet_eta, &b_jet_exktsubjet_eta);
   fChain->SetBranchAddress("jet_exktsubjet_phi", &jet_exktsubjet_phi, &b_jet_exktsubjet_phi);
   fChain->SetBranchAddress("jet_exktsubjet_m", &jet_exktsubjet_m, &b_jet_exktsubjet_m);
   fChain->SetBranchAddress("jet_exktsubjet_ntrk", &jet_exktsubjet_ntrk, &b_jet_exktsubjet_ntrk);
   fChain->SetBranchAddress("jet_exktsubjet_mv2c20", &jet_exktsubjet_mv2c20, &b_jet_exktsubjet_mv2c20);
   fChain->SetBranchAddress("jet_ExKtbb_Hbb_DoubleMV2c20", &jet_ExKtbb_Hbb_DoubleMV2c20, &b_jet_ExKtbb_Hbb_DoubleMV2c20);
   fChain->SetBranchAddress("jet_ExKtbb_Hbb_SingleMV2c20", &jet_ExKtbb_Hbb_SingleMV2c20, &b_jet_ExKtbb_Hbb_SingleMV2c20);
   fChain->SetBranchAddress("jet_ExKtbb_Hbb_MV2Only", &jet_ExKtbb_Hbb_MV2Only, &b_jet_ExKtbb_Hbb_MV2Only);
   fChain->SetBranchAddress("jet_ExKtbb_Hbb_MV2andJFDRSig", &jet_ExKtbb_Hbb_MV2andJFDRSig, &b_jet_ExKtbb_Hbb_MV2andJFDRSig);
   fChain->SetBranchAddress("jet_ExKtbb_Hbb_MV2andTopos", &jet_ExKtbb_Hbb_MV2andTopos, &b_jet_ExKtbb_Hbb_MV2andTopos);
   fChain->SetBranchAddress("bH1_pt", &bH1_pt, &b_bH1_pt);
   fChain->SetBranchAddress("bH1_eta", &bH1_eta, &b_bH1_eta);
   fChain->SetBranchAddress("bH1_phi", &bH1_phi, &b_bH1_phi);
   fChain->SetBranchAddress("bH1_Lxy", &bH1_Lxy, &b_bH1_Lxy);
   fChain->SetBranchAddress("bH1_x", &bH1_x, &b_bH1_x);
   fChain->SetBranchAddress("bH1_y", &bH1_y, &b_bH1_y);
   fChain->SetBranchAddress("bH1_z", &bH1_z, &b_bH1_z);
   fChain->SetBranchAddress("bH1_dRjet", &bH1_dRjet, &b_bH1_dRjet);
   fChain->SetBranchAddress("bH2_pt", &bH2_pt, &b_bH2_pt);
   fChain->SetBranchAddress("bH2_eta", &bH2_eta, &b_bH2_eta);
   fChain->SetBranchAddress("bH2_phi", &bH2_phi, &b_bH2_phi);
   fChain->SetBranchAddress("bH2_Lxy", &bH2_Lxy, &b_bH2_Lxy);
   fChain->SetBranchAddress("bH2_x", &bH2_x, &b_bH2_x);
   fChain->SetBranchAddress("bH2_y", &bH2_y, &b_bH2_y);
   fChain->SetBranchAddress("bH2_z", &bH2_z, &b_bH2_z);
   fChain->SetBranchAddress("bH2_dRjet", &bH2_dRjet, &b_bH2_dRjet);
   fChain->SetBranchAddress("bH_pt", &bH_pt, &b_bH_pt);
   fChain->SetBranchAddress("bH_eta", &bH_eta, &b_bH_eta);
   fChain->SetBranchAddress("bH_phi", &bH_phi, &b_bH_phi);
   fChain->SetBranchAddress("bH_Lxy", &bH_Lxy, &b_bH_Lxy);
   fChain->SetBranchAddress("bH_x", &bH_x, &b_bH_x);
   fChain->SetBranchAddress("bH_y", &bH_y, &b_bH_y);
   fChain->SetBranchAddress("bH_z", &bH_z, &b_bH_z);
   fChain->SetBranchAddress("bH_dRjet", &bH_dRjet, &b_bH_dRjet);
   fChain->SetBranchAddress("bH_nBtracks", &bH_nBtracks, &b_bH_nBtracks);
   fChain->SetBranchAddress("bH_nCtracks", &bH_nCtracks, &b_bH_nCtracks);
   fChain->SetBranchAddress("cH_pt", &cH_pt, &b_cH_pt);
   fChain->SetBranchAddress("cH_eta", &cH_eta, &b_cH_eta);
   fChain->SetBranchAddress("cH_phi", &cH_phi, &b_cH_phi);
   fChain->SetBranchAddress("cH_Lxy", &cH_Lxy, &b_cH_Lxy);
   fChain->SetBranchAddress("cH_x", &cH_x, &b_cH_x);
   fChain->SetBranchAddress("cH_y", &cH_y, &b_cH_y);
   fChain->SetBranchAddress("cH_z", &cH_z, &b_cH_z);
   fChain->SetBranchAddress("cH_dRjet", &cH_dRjet, &b_cH_dRjet);
   fChain->SetBranchAddress("cH_nCtracks", &cH_nCtracks, &b_cH_nCtracks);
   fChain->SetBranchAddress("jet_btag_ntrk", &jet_btag_ntrk, &b_jet_btag_ntrk);
   fChain->SetBranchAddress("jet_trk_pt", &jet_trk_pt, &b_jet_trk_pt);
   fChain->SetBranchAddress("jet_trk_eta", &jet_trk_eta, &b_jet_trk_eta);
   fChain->SetBranchAddress("jet_trk_theta", &jet_trk_theta, &b_jet_trk_theta);
   fChain->SetBranchAddress("jet_trk_phi", &jet_trk_phi, &b_jet_trk_phi);
   fChain->SetBranchAddress("jet_trk_dr", &jet_trk_dr, &b_jet_trk_dr);
   fChain->SetBranchAddress("jet_trk_assoc_msv", &jet_trk_assoc_msv, &b_jet_trk_assoc_msv);
   fChain->SetBranchAddress("jet_trk_chi2", &jet_trk_chi2, &b_jet_trk_chi2);
   fChain->SetBranchAddress("jet_trk_ndf", &jet_trk_ndf, &b_jet_trk_ndf);
   fChain->SetBranchAddress("jet_trk_algo", &jet_trk_algo, &b_jet_trk_algo);
   fChain->SetBranchAddress("jet_trk_orig", &jet_trk_orig, &b_jet_trk_orig);
   fChain->SetBranchAddress("jet_trk_vtx_X", &jet_trk_vtx_X, &b_jet_trk_vtx_X);
   fChain->SetBranchAddress("jet_trk_vtx_Y", &jet_trk_vtx_Y, &b_jet_trk_vtx_Y);
   fChain->SetBranchAddress("jet_trk_nInnHits", &jet_trk_nInnHits, &b_jet_trk_nInnHits);
   fChain->SetBranchAddress("jet_trk_nNextToInnHits", &jet_trk_nNextToInnHits, &b_jet_trk_nNextToInnHits);
   fChain->SetBranchAddress("jet_trk_nBLHits", &jet_trk_nBLHits, &b_jet_trk_nBLHits);
   fChain->SetBranchAddress("jet_trk_nsharedBLHits", &jet_trk_nsharedBLHits, &b_jet_trk_nsharedBLHits);
   fChain->SetBranchAddress("jet_trk_nsplitBLHits", &jet_trk_nsplitBLHits, &b_jet_trk_nsplitBLHits);
   fChain->SetBranchAddress("jet_trk_nPixHits", &jet_trk_nPixHits, &b_jet_trk_nPixHits);
   fChain->SetBranchAddress("jet_trk_nsharedPixHits", &jet_trk_nsharedPixHits, &b_jet_trk_nsharedPixHits);
   fChain->SetBranchAddress("jet_trk_nsplitPixHits", &jet_trk_nsplitPixHits, &b_jet_trk_nsplitPixHits);
   fChain->SetBranchAddress("jet_trk_nSCTHits", &jet_trk_nSCTHits, &b_jet_trk_nSCTHits);
   fChain->SetBranchAddress("jet_trk_nsharedSCTHits", &jet_trk_nsharedSCTHits, &b_jet_trk_nsharedSCTHits);
   fChain->SetBranchAddress("jet_trk_expectBLayerHit", &jet_trk_expectBLayerHit, &b_jet_trk_expectBLayerHit);
   fChain->SetBranchAddress("jet_trk_d0", &jet_trk_d0, &b_jet_trk_d0);
   fChain->SetBranchAddress("jet_trk_z0", &jet_trk_z0, &b_jet_trk_z0);
   fChain->SetBranchAddress("jet_trk_d0_truth", &jet_trk_d0_truth, &b_jet_trk_d0_truth);
   fChain->SetBranchAddress("jet_trk_z0_truth", &jet_trk_z0_truth, &b_jet_trk_z0_truth);
   fChain->SetBranchAddress("jet_trk_ip3d_grade", &jet_trk_ip3d_grade, &b_jet_trk_ip3d_grade);
   fChain->SetBranchAddress("jet_trk_ip3d_d0", &jet_trk_ip3d_d0, &b_jet_trk_ip3d_d0);
   fChain->SetBranchAddress("jet_trk_ip3d_d02D", &jet_trk_ip3d_d02D, &b_jet_trk_ip3d_d02D);
   fChain->SetBranchAddress("jet_trk_ip3d_z0", &jet_trk_ip3d_z0, &b_jet_trk_ip3d_z0);
   fChain->SetBranchAddress("jet_trk_ip3d_d0sig", &jet_trk_ip3d_d0sig, &b_jet_trk_ip3d_d0sig);
   fChain->SetBranchAddress("jet_trk_ip3d_z0sig", &jet_trk_ip3d_z0sig, &b_jet_trk_ip3d_z0sig);
   fChain->SetBranchAddress("jet_trk_ip2d_llr", &jet_trk_ip2d_llr, &b_jet_trk_ip2d_llr);
   fChain->SetBranchAddress("jet_trk_ip3d_llr", &jet_trk_ip3d_llr, &b_jet_trk_ip3d_llr);
   fChain->SetBranchAddress("jet_trk_jf_Vertex", &jet_trk_jf_Vertex, &b_jet_trk_jf_Vertex);
   fChain->SetBranchAddress("jet_sv1_ntrk", &jet_sv1_ntrk, &b_jet_sv1_ntrk);
   fChain->SetBranchAddress("jet_ip3d_ntrk", &jet_ip3d_ntrk, &b_jet_ip3d_ntrk);
   fChain->SetBranchAddress("jet_jf_ntrk", &jet_jf_ntrk, &b_jet_jf_ntrk);
   fChain->SetBranchAddress("jet_width", &jet_width, &b_jet_width);
   fChain->SetBranchAddress("jet_n_trk_sigd0cut", &jet_n_trk_sigd0cut, &b_jet_n_trk_sigd0cut);
   fChain->SetBranchAddress("jet_trk3_d0sig", &jet_trk3_d0sig, &b_jet_trk3_d0sig);
   fChain->SetBranchAddress("jet_trk3_z0sig", &jet_trk3_z0sig, &b_jet_trk3_z0sig);
   fChain->SetBranchAddress("jet_sv_scaled_efc", &jet_sv_scaled_efc, &b_jet_sv_scaled_efc);
   fChain->SetBranchAddress("jet_jf_scaled_efc", &jet_jf_scaled_efc, &b_jet_jf_scaled_efc);
   fChain->SetBranchAddress("jet_trkjet_pt", &jet_trkjet_pt, &b_jet_trkjet_pt);
   fChain->SetBranchAddress("jet_trkjet_eta", &jet_trkjet_eta, &b_jet_trkjet_eta);
   fChain->SetBranchAddress("jet_trkjet_phi", &jet_trkjet_phi, &b_jet_trkjet_phi);
   fChain->SetBranchAddress("jet_trkjet_m", &jet_trkjet_m, &b_jet_trkjet_m);
   fChain->SetBranchAddress("jet_trkjet_ntrk", &jet_trkjet_ntrk, &b_jet_trkjet_ntrk);
   fChain->SetBranchAddress("jet_trkjet_mv2c20", &jet_trkjet_mv2c20, &b_jet_trkjet_mv2c20);
   Notify();
}

Bool_t test::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void test::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t test::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef test_cxx
