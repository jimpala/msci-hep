//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 13 12:52:31 2016 by ROOT version 6.06/04
// from TChain bTag_AntiKt4EMTopoJets/
//////////////////////////////////////////////////////////

#ifndef lbmorph_h
#define lbmorph_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>



class lbmorph : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> runnb = {fReader, "runnb"};
   TTreeReaderValue<Int_t> eventnb = {fReader, "eventnb"};
   TTreeReaderValue<Int_t> mcchan = {fReader, "mcchan"};
   TTreeReaderValue<Double_t> mcwg = {fReader, "mcwg"};
   TTreeReaderValue<Int_t> lbn = {fReader, "lbn"};
   TTreeReaderValue<Int_t> coreFlag = {fReader, "coreFlag"};
   TTreeReaderValue<Int_t> larError = {fReader, "larError"};
   TTreeReaderValue<Int_t> tileError = {fReader, "tileError"};
   TTreeReaderValue<Int_t> nPV = {fReader, "nPV"};
   TTreeReaderValue<Double_t> avgmu = {fReader, "avgmu"};
   TTreeReaderValue<Int_t> actmu = {fReader, "actmu"};
   TTreeReaderValue<Double_t> PVx = {fReader, "PVx"};
   TTreeReaderValue<Double_t> PVy = {fReader, "PVy"};
   TTreeReaderValue<Double_t> PVz = {fReader, "PVz"};
   TTreeReaderValue<Double_t> truth_PVx = {fReader, "truth_PVx"};
   TTreeReaderValue<Double_t> truth_PVy = {fReader, "truth_PVy"};
   TTreeReaderValue<Double_t> truth_PVz = {fReader, "truth_PVz"};
   TTreeReaderValue<Float_t> truth_LeadJet_pt = {fReader, "truth_LeadJet_pt"};
   TTreeReaderValue<Int_t> njets = {fReader, "njets"};
   TTreeReaderValue<Int_t> nbjets = {fReader, "nbjets"};
   TTreeReaderValue<Int_t> nbjets_q = {fReader, "nbjets_q"};
   TTreeReaderValue<Int_t> nbjets_HadI = {fReader, "nbjets_HadI"};
   TTreeReaderValue<Int_t> nbjets_HadF = {fReader, "nbjets_HadF"};
   TTreeReaderArray<float> jet_pt = {fReader, "jet_pt"};
   TTreeReaderArray<float> jet_eta = {fReader, "jet_eta"};
   TTreeReaderArray<float> jet_phi = {fReader, "jet_phi"};
   TTreeReaderArray<float> jet_pt_orig = {fReader, "jet_pt_orig"};
   TTreeReaderArray<float> jet_eta_orig = {fReader, "jet_eta_orig"};
   TTreeReaderArray<float> jet_phi_orig = {fReader, "jet_phi_orig"};
   TTreeReaderArray<float> jet_E_orig = {fReader, "jet_E_orig"};
   TTreeReaderArray<float> jet_sumtrkS_pt = {fReader, "jet_sumtrkS_pt"};
   TTreeReaderArray<float> jet_sumtrkV_pt = {fReader, "jet_sumtrkV_pt"};
   TTreeReaderArray<float> jet_sumtrkV_eta = {fReader, "jet_sumtrkV_eta"};
   TTreeReaderArray<float> jet_sumtrkV_phi = {fReader, "jet_sumtrkV_phi"};
   TTreeReaderArray<int> jet_sumtrk_ntrk = {fReader, "jet_sumtrk_ntrk"};
   TTreeReaderArray<float> jet_E = {fReader, "jet_E"};
   TTreeReaderArray<float> jet_m = {fReader, "jet_m"};
   TTreeReaderArray<int> jet_nConst = {fReader, "jet_nConst"};
   TTreeReaderArray<int> jet_truthflav = {fReader, "jet_truthflav"};
   TTreeReaderArray<int> jet_nBHadr = {fReader, "jet_nBHadr"};
   TTreeReaderArray<int> jet_nCHadr = {fReader, "jet_nCHadr"};
   TTreeReaderArray<int> jet_nGhostBHadrFromParent = {fReader, "jet_nGhostBHadrFromParent"};
   TTreeReaderArray<int> jet_nGhostCHadrFromParent = {fReader, "jet_nGhostCHadrFromParent"};
   TTreeReaderArray<int> jet_nGhostCHadrFromParentNotFromB = {fReader, "jet_nGhostCHadrFromParentNotFromB"};
   TTreeReaderArray<int> jet_nGhostTauFromParent = {fReader, "jet_nGhostTauFromParent"};
   TTreeReaderArray<int> jet_nGhostHBosoFromParent = {fReader, "jet_nGhostHBosoFromParent"};
   TTreeReaderArray<int> jet_GhostL_q = {fReader, "jet_GhostL_q"};
   TTreeReaderArray<int> jet_GhostL_HadI = {fReader, "jet_GhostL_HadI"};
   TTreeReaderArray<int> jet_GhostL_HadF = {fReader, "jet_GhostL_HadF"};
   TTreeReaderArray<int> jet_LabDr_HadF = {fReader, "jet_LabDr_HadF"};
   TTreeReaderArray<int> jet_aliveAfterOR = {fReader, "jet_aliveAfterOR"};
   TTreeReaderArray<int> jet_aliveAfterORmu = {fReader, "jet_aliveAfterORmu"};
   TTreeReaderArray<int> jet_truthMatch = {fReader, "jet_truthMatch"};
   TTreeReaderArray<int> jet_isPU = {fReader, "jet_isPU"};
   TTreeReaderArray<int> jet_isBadMedium = {fReader, "jet_isBadMedium"};
   TTreeReaderArray<float> jet_truthPt = {fReader, "jet_truthPt"};
   TTreeReaderArray<float> jet_dRiso = {fReader, "jet_dRiso"};
   TTreeReaderArray<float> jet_JVT = {fReader, "jet_JVT"};
   TTreeReaderArray<float> jet_JVF = {fReader, "jet_JVF"};
   TTreeReaderArray<float> jet_dRminToB = {fReader, "jet_dRminToB"};
   TTreeReaderArray<float> jet_dRminToC = {fReader, "jet_dRminToC"};
   TTreeReaderArray<float> jet_dRminToT = {fReader, "jet_dRminToT"};
   TTreeReaderArray<float> jet_ip2d_pb = {fReader, "jet_ip2d_pb"};
   TTreeReaderArray<float> jet_ip2d_pc = {fReader, "jet_ip2d_pc"};
   TTreeReaderArray<float> jet_ip2d_pu = {fReader, "jet_ip2d_pu"};
   TTreeReaderArray<float> jet_ip2d_llr = {fReader, "jet_ip2d_llr"};
   TTreeReaderArray<float> jet_ip3d_pb = {fReader, "jet_ip3d_pb"};
   TTreeReaderArray<float> jet_ip3d_pc = {fReader, "jet_ip3d_pc"};
   TTreeReaderArray<float> jet_ip3d_pu = {fReader, "jet_ip3d_pu"};
   TTreeReaderArray<float> jet_ip3d_llr = {fReader, "jet_ip3d_llr"};
   TTreeReaderArray<float> jet_sv0_sig3d = {fReader, "jet_sv0_sig3d"};
   TTreeReaderArray<int> jet_sv0_ntrkj = {fReader, "jet_sv0_ntrkj"};
   TTreeReaderArray<int> jet_sv0_ntrkv = {fReader, "jet_sv0_ntrkv"};
   TTreeReaderArray<int> jet_sv0_n2t = {fReader, "jet_sv0_n2t"};
   TTreeReaderArray<float> jet_sv0_m = {fReader, "jet_sv0_m"};
   TTreeReaderArray<float> jet_sv0_efc = {fReader, "jet_sv0_efc"};
   TTreeReaderArray<float> jet_sv0_normdist = {fReader, "jet_sv0_normdist"};
   TTreeReaderArray<int> jet_sv0_Nvtx = {fReader, "jet_sv0_Nvtx"};
   TTreeReaderArray<vector<float>> jet_sv0_vtx_x = {fReader, "jet_sv0_vtx_x"};
   TTreeReaderArray<vector<float>> jet_sv0_vtx_y = {fReader, "jet_sv0_vtx_y"};
   TTreeReaderArray<vector<float>> jet_sv0_vtx_z = {fReader, "jet_sv0_vtx_z"};
   TTreeReaderArray<int> jet_sv1_ntrkj = {fReader, "jet_sv1_ntrkj"};
   TTreeReaderArray<int> jet_sv1_ntrkv = {fReader, "jet_sv1_ntrkv"};
   TTreeReaderArray<int> jet_sv1_n2t = {fReader, "jet_sv1_n2t"};
   TTreeReaderArray<float> jet_sv1_m = {fReader, "jet_sv1_m"};
   TTreeReaderArray<float> jet_sv1_efc = {fReader, "jet_sv1_efc"};
   TTreeReaderArray<float> jet_sv1_normdist = {fReader, "jet_sv1_normdist"};
   TTreeReaderArray<float> jet_sv1_pb = {fReader, "jet_sv1_pb"};
   TTreeReaderArray<float> jet_sv1_pc = {fReader, "jet_sv1_pc"};
   TTreeReaderArray<float> jet_sv1_pu = {fReader, "jet_sv1_pu"};
   TTreeReaderArray<float> jet_sv1_llr = {fReader, "jet_sv1_llr"};
   TTreeReaderArray<int> jet_sv1_Nvtx = {fReader, "jet_sv1_Nvtx"};
   TTreeReaderArray<float> jet_sv1_sig3d = {fReader, "jet_sv1_sig3d"};
   TTreeReaderArray<vector<float>> jet_sv1_vtx_x = {fReader, "jet_sv1_vtx_x"};
   TTreeReaderArray<vector<float>> jet_sv1_vtx_y = {fReader, "jet_sv1_vtx_y"};
   TTreeReaderArray<vector<float>> jet_sv1_vtx_z = {fReader, "jet_sv1_vtx_z"};
   TTreeReaderValue<Float_t> PV_jf_x = {fReader, "PV_jf_x"};
   TTreeReaderValue<Float_t> PV_jf_y = {fReader, "PV_jf_y"};
   TTreeReaderValue<Float_t> PV_jf_z = {fReader, "PV_jf_z"};
   TTreeReaderArray<float> jet_jf_pb = {fReader, "jet_jf_pb"};
   TTreeReaderArray<float> jet_jf_pc = {fReader, "jet_jf_pc"};
   TTreeReaderArray<float> jet_jf_pu = {fReader, "jet_jf_pu"};
   TTreeReaderArray<float> jet_jf_llr = {fReader, "jet_jf_llr"};
   TTreeReaderArray<float> jet_jf_m = {fReader, "jet_jf_m"};
   TTreeReaderArray<float> jet_jf_mUncorr = {fReader, "jet_jf_mUncorr"};
   TTreeReaderArray<float> jet_jf_efc = {fReader, "jet_jf_efc"};
   TTreeReaderArray<float> jet_jf_deta = {fReader, "jet_jf_deta"};
   TTreeReaderArray<float> jet_jf_dphi = {fReader, "jet_jf_dphi"};
   TTreeReaderArray<float> jet_jf_dRFlightDir = {fReader, "jet_jf_dRFlightDir"};
   TTreeReaderArray<float> jet_jf_ntrkAtVx = {fReader, "jet_jf_ntrkAtVx"};
   TTreeReaderArray<int> jet_jf_nvtx = {fReader, "jet_jf_nvtx"};
   TTreeReaderArray<float> jet_jf_sig3d = {fReader, "jet_jf_sig3d"};
   TTreeReaderArray<int> jet_jf_nvtx1t = {fReader, "jet_jf_nvtx1t"};
   TTreeReaderArray<int> jet_jf_n2t = {fReader, "jet_jf_n2t"};
   TTreeReaderArray<int> jet_jf_VTXsize = {fReader, "jet_jf_VTXsize"};
   TTreeReaderArray<vector<float>> jet_jf_vtx_chi2 = {fReader, "jet_jf_vtx_chi2"};
   TTreeReaderArray<vector<float>> jet_jf_vtx_ndf = {fReader, "jet_jf_vtx_ndf"};
   TTreeReaderArray<vector<int>> jet_jf_vtx_ntrk = {fReader, "jet_jf_vtx_ntrk"};
   TTreeReaderArray<vector<float>> jet_jf_vtx_L3D = {fReader, "jet_jf_vtx_L3D"};
   TTreeReaderArray<vector<float>> jet_jf_vtx_sig3D = {fReader, "jet_jf_vtx_sig3D"};
   TTreeReaderArray<float> jet_jf_phi = {fReader, "jet_jf_phi"};
   TTreeReaderArray<float> jet_jf_theta = {fReader, "jet_jf_theta"};
   TTreeReaderArray<float> jet_jfcombnn_pb = {fReader, "jet_jfcombnn_pb"};
   TTreeReaderArray<float> jet_jfcombnn_pc = {fReader, "jet_jfcombnn_pc"};
   TTreeReaderArray<float> jet_jfcombnn_pu = {fReader, "jet_jfcombnn_pu"};
   TTreeReaderArray<float> jet_jfcombnn_llr = {fReader, "jet_jfcombnn_llr"};
   TTreeReaderArray<float> jet_dl1_pb = {fReader, "jet_dl1_pb"};
   TTreeReaderArray<float> jet_dl1_pc = {fReader, "jet_dl1_pc"};
   TTreeReaderArray<float> jet_dl1_pu = {fReader, "jet_dl1_pu"};
   TTreeReaderArray<double> jet_sv1ip3d = {fReader, "jet_sv1ip3d"};
   TTreeReaderArray<double> jet_mv1 = {fReader, "jet_mv1"};
   TTreeReaderArray<double> jet_mv1c = {fReader, "jet_mv1c"};
   TTreeReaderArray<double> jet_mv2c00 = {fReader, "jet_mv2c00"};
   TTreeReaderArray<double> jet_mv2c10 = {fReader, "jet_mv2c10"};
   TTreeReaderArray<double> jet_mv2c20 = {fReader, "jet_mv2c20"};
   TTreeReaderArray<double> jet_mv2c100 = {fReader, "jet_mv2c100"};
   TTreeReaderArray<double> jet_mv2cl100 = {fReader, "jet_mv2cl100"};
   TTreeReaderArray<double> jet_mv2m_pu = {fReader, "jet_mv2m_pu"};
   TTreeReaderArray<double> jet_mv2m_pc = {fReader, "jet_mv2m_pc"};
   TTreeReaderArray<double> jet_mv2m_pb = {fReader, "jet_mv2m_pb"};
   TTreeReaderArray<double> jet_mvb = {fReader, "jet_mvb"};
   TTreeReaderArray<vector<float>> jet_exktsubjet_pt = {fReader, "jet_exktsubjet_pt"};
   TTreeReaderArray<vector<float>> jet_exktsubjet_eta = {fReader, "jet_exktsubjet_eta"};
   TTreeReaderArray<vector<float>> jet_exktsubjet_phi = {fReader, "jet_exktsubjet_phi"};
   TTreeReaderArray<vector<float>> jet_exktsubjet_m = {fReader, "jet_exktsubjet_m"};
   TTreeReaderArray<vector<int>> jet_exktsubjet_ntrk = {fReader, "jet_exktsubjet_ntrk"};
   TTreeReaderArray<vector<float>> jet_exktsubjet_mv2c20 = {fReader, "jet_exktsubjet_mv2c20"};
   TTreeReaderArray<double> jet_ExKtbb_Hbb_DoubleMV2c20 = {fReader, "jet_ExKtbb_Hbb_DoubleMV2c20"};
   TTreeReaderArray<double> jet_ExKtbb_Hbb_SingleMV2c20 = {fReader, "jet_ExKtbb_Hbb_SingleMV2c20"};
   TTreeReaderArray<double> jet_ExKtbb_Hbb_MV2Only = {fReader, "jet_ExKtbb_Hbb_MV2Only"};
   TTreeReaderArray<double> jet_ExKtbb_Hbb_MV2andJFDRSig = {fReader, "jet_ExKtbb_Hbb_MV2andJFDRSig"};
   TTreeReaderArray<double> jet_ExKtbb_Hbb_MV2andTopos = {fReader, "jet_ExKtbb_Hbb_MV2andTopos"};
   TTreeReaderArray<float> bH1_pt = {fReader, "bH1_pt"};
   TTreeReaderArray<float> bH1_eta = {fReader, "bH1_eta"};
   TTreeReaderArray<float> bH1_phi = {fReader, "bH1_phi"};
   TTreeReaderArray<float> bH1_Lxy = {fReader, "bH1_Lxy"};
   TTreeReaderArray<float> bH1_x = {fReader, "bH1_x"};
   TTreeReaderArray<float> bH1_y = {fReader, "bH1_y"};
   TTreeReaderArray<float> bH1_z = {fReader, "bH1_z"};
   TTreeReaderArray<float> bH1_dRjet = {fReader, "bH1_dRjet"};
   TTreeReaderArray<float> bH2_pt = {fReader, "bH2_pt"};
   TTreeReaderArray<float> bH2_eta = {fReader, "bH2_eta"};
   TTreeReaderArray<float> bH2_phi = {fReader, "bH2_phi"};
   TTreeReaderArray<float> bH2_Lxy = {fReader, "bH2_Lxy"};
   TTreeReaderArray<float> bH2_x = {fReader, "bH2_x"};
   TTreeReaderArray<float> bH2_y = {fReader, "bH2_y"};
   TTreeReaderArray<float> bH2_z = {fReader, "bH2_z"};
   TTreeReaderArray<float> bH2_dRjet = {fReader, "bH2_dRjet"};
   TTreeReaderArray<float> bH_pt = {fReader, "bH_pt"};
   TTreeReaderArray<float> bH_eta = {fReader, "bH_eta"};
   TTreeReaderArray<float> bH_phi = {fReader, "bH_phi"};
   TTreeReaderArray<float> bH_Lxy = {fReader, "bH_Lxy"};
   TTreeReaderArray<float> bH_x = {fReader, "bH_x"};
   TTreeReaderArray<float> bH_y = {fReader, "bH_y"};
   TTreeReaderArray<float> bH_z = {fReader, "bH_z"};
   TTreeReaderArray<float> bH_dRjet = {fReader, "bH_dRjet"};
   TTreeReaderArray<int> bH_nBtracks = {fReader, "bH_nBtracks"};
   TTreeReaderArray<int> bH_nCtracks = {fReader, "bH_nCtracks"};
   TTreeReaderArray<float> cH_pt = {fReader, "cH_pt"};
   TTreeReaderArray<float> cH_eta = {fReader, "cH_eta"};
   TTreeReaderArray<float> cH_phi = {fReader, "cH_phi"};
   TTreeReaderArray<float> cH_Lxy = {fReader, "cH_Lxy"};
   TTreeReaderArray<float> cH_x = {fReader, "cH_x"};
   TTreeReaderArray<float> cH_y = {fReader, "cH_y"};
   TTreeReaderArray<float> cH_z = {fReader, "cH_z"};
   TTreeReaderArray<float> cH_dRjet = {fReader, "cH_dRjet"};
   TTreeReaderArray<int> cH_nCtracks = {fReader, "cH_nCtracks"};
   TTreeReaderArray<int> jet_btag_ntrk = {fReader, "jet_btag_ntrk"};
   TTreeReaderArray<vector<float>> jet_trk_pt = {fReader, "jet_trk_pt"};
   TTreeReaderArray<vector<float>> jet_trk_eta = {fReader, "jet_trk_eta"};
   TTreeReaderArray<vector<float>> jet_trk_theta = {fReader, "jet_trk_theta"};
   TTreeReaderArray<vector<float>> jet_trk_phi = {fReader, "jet_trk_phi"};
   TTreeReaderArray<vector<float>> jet_trk_dr = {fReader, "jet_trk_dr"};
   TTreeReaderArray<vector<int>> jet_trk_assoc_msv = {fReader, "jet_trk_assoc_msv"};
   TTreeReaderArray<vector<float>> jet_trk_chi2 = {fReader, "jet_trk_chi2"};
   TTreeReaderArray<vector<float>> jet_trk_ndf = {fReader, "jet_trk_ndf"};
   TTreeReaderArray<vector<int>> jet_trk_algo = {fReader, "jet_trk_algo"};
   TTreeReaderArray<vector<int>> jet_trk_orig = {fReader, "jet_trk_orig"};
   TTreeReaderArray<vector<float>> jet_trk_vtx_X = {fReader, "jet_trk_vtx_X"};
   TTreeReaderArray<vector<float>> jet_trk_vtx_Y = {fReader, "jet_trk_vtx_Y"};
   TTreeReaderArray<vector<int>> jet_trk_nInnHits = {fReader, "jet_trk_nInnHits"};
   TTreeReaderArray<vector<int>> jet_trk_nNextToInnHits = {fReader, "jet_trk_nNextToInnHits"};
   TTreeReaderArray<vector<int>> jet_trk_nBLHits = {fReader, "jet_trk_nBLHits"};
   TTreeReaderArray<vector<int>> jet_trk_nsharedBLHits = {fReader, "jet_trk_nsharedBLHits"};
   TTreeReaderArray<vector<int>> jet_trk_nsplitBLHits = {fReader, "jet_trk_nsplitBLHits"};
   TTreeReaderArray<vector<int>> jet_trk_nPixHits = {fReader, "jet_trk_nPixHits"};
   TTreeReaderArray<vector<int>> jet_trk_nsharedPixHits = {fReader, "jet_trk_nsharedPixHits"};
   TTreeReaderArray<vector<int>> jet_trk_nsplitPixHits = {fReader, "jet_trk_nsplitPixHits"};
   TTreeReaderArray<vector<int>> jet_trk_nSCTHits = {fReader, "jet_trk_nSCTHits"};
   TTreeReaderArray<vector<int>> jet_trk_nsharedSCTHits = {fReader, "jet_trk_nsharedSCTHits"};
   TTreeReaderArray<vector<int>> jet_trk_expectBLayerHit = {fReader, "jet_trk_expectBLayerHit"};
   TTreeReaderArray<vector<float>> jet_trk_d0 = {fReader, "jet_trk_d0"};
   TTreeReaderArray<vector<float>> jet_trk_z0 = {fReader, "jet_trk_z0"};
   TTreeReaderArray<vector<float>> jet_trk_d0_truth = {fReader, "jet_trk_d0_truth"};
   TTreeReaderArray<vector<float>> jet_trk_z0_truth = {fReader, "jet_trk_z0_truth"};
   TTreeReaderArray<vector<int>> jet_trk_ip3d_grade = {fReader, "jet_trk_ip3d_grade"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_d0 = {fReader, "jet_trk_ip3d_d0"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_d02D = {fReader, "jet_trk_ip3d_d02D"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_z0 = {fReader, "jet_trk_ip3d_z0"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_d0sig = {fReader, "jet_trk_ip3d_d0sig"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_z0sig = {fReader, "jet_trk_ip3d_z0sig"};
   TTreeReaderArray<vector<float>> jet_trk_ip2d_llr = {fReader, "jet_trk_ip2d_llr"};
   TTreeReaderArray<vector<float>> jet_trk_ip3d_llr = {fReader, "jet_trk_ip3d_llr"};
   TTreeReaderArray<vector<int>> jet_trk_jf_Vertex = {fReader, "jet_trk_jf_Vertex"};
   TTreeReaderArray<int> jet_sv1_ntrk = {fReader, "jet_sv1_ntrk"};
   TTreeReaderArray<int> jet_ip3d_ntrk = {fReader, "jet_ip3d_ntrk"};
   TTreeReaderArray<int> jet_jf_ntrk = {fReader, "jet_jf_ntrk"};
   TTreeReaderArray<float> jet_width = {fReader, "jet_width"};
   TTreeReaderArray<int> jet_n_trk_sigd0cut = {fReader, "jet_n_trk_sigd0cut"};
   TTreeReaderArray<float> jet_trk3_d0sig = {fReader, "jet_trk3_d0sig"};
   TTreeReaderArray<float> jet_trk3_z0sig = {fReader, "jet_trk3_z0sig"};
   TTreeReaderArray<float> jet_sv_scaled_efc = {fReader, "jet_sv_scaled_efc"};
   TTreeReaderArray<float> jet_jf_scaled_efc = {fReader, "jet_jf_scaled_efc"};
   TTreeReaderArray<vector<float>> jet_trkjet_pt = {fReader, "jet_trkjet_pt"};
   TTreeReaderArray<vector<float>> jet_trkjet_eta = {fReader, "jet_trkjet_eta"};
   TTreeReaderArray<vector<float>> jet_trkjet_phi = {fReader, "jet_trkjet_phi"};
   TTreeReaderArray<vector<float>> jet_trkjet_m = {fReader, "jet_trkjet_m"};
   TTreeReaderArray<vector<int>> jet_trkjet_ntrk = {fReader, "jet_trkjet_ntrk"};
   TTreeReaderArray<vector<double>> jet_trkjet_mv2c20 = {fReader, "jet_trkjet_mv2c20"};


   lbmorph(TTree * /*tree*/ =0) { }
   virtual ~lbmorph() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(lbmorph,0);

};

#endif

#ifdef lbmorph_cxx
void lbmorph::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t lbmorph::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef lbmorph_cxx
