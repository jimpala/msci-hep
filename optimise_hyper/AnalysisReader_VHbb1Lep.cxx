#include <type_traits>
#include <EventLoop/Worker.h>
#include "TSystem.h"

#include "CxAODReader_VHbb/AnalysisReader_VHbb.h"
#include <CxAODReader_VHbb/AnalysisReader_VHbb1Lep.h>

#define length(array) (sizeof(array) / sizeof(*(array)))

AnalysisReader_VHbb1Lep::AnalysisReader_VHbb1Lep () :
  AnalysisReader_VHbb()
{
}

AnalysisReader_VHbb1Lep::~AnalysisReader_VHbb1Lep ()
{
}


EL::StatusCode AnalysisReader_VHbb1Lep::initializeSelection ()
{

   AnalysisReader_VHbb::initializeSelection ();

    m_eventSelection = new VHbb1lepEvtSelection();
    //m_fillFunction   = std::bind(&AnalysisReader_VHbb1Lep::fill_1Lep, this);
    m_fillFunction   = std::bind(&AnalysisReader_VHbb1Lep::run_1Lep_analysis, this);
    if(m_model == Model::HVT) ((VHbb1lepEvtSelection*)m_eventSelection)->SetModel("HVT");
    ((VHbb1lepEvtSelection*)m_eventSelection)->SetAnalysisType("m_analysisType");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode AnalysisReader_VHbb1Lep::run_1Lep_analysis ()
{

  if(m_debug) std::cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << std::endl;
  if(m_debug) std::cout << " >>>>> Starting run_1Lep_analysis " << std::endl;
  if(m_debug) std::cout << " @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ " << std::endl;

  // TEMPORARY FIX OF THE GRL AT READER LEVEL
  // ATTENTION ! ONLY APPLIED TO SM VH
  if ( m_model == Model::MVA || m_model == Model::SM ) {
    if ( !m_isMC && m_eventInfo->runNumber() == 303079 ) {
      bool doRemoveEvent = AnalysisReader_VHbb::removeUnwantedLB();
      if (doRemoveEvent) {
	//std::cout << "Going to remove event, run nb " << m_eventInfo->runNumber() << " LB nb " << m_eventInfo->lumiBlock() << std::endl;
	return EL::StatusCode::SUCCESS;
      }
    }
  }
  //*********************************************************//
  //                     READ CONFIGURATION                  //
  //*********************************************************//




  // set defaults
  bool doMuonInJetCorrection         = true;
  bool doMbbRescaling                = false; // is called mbbwindow by other channels
  bool doBlindingData                = false;
  bool doBlindingMC                  = false;

  // easy tree options
  bool doMJEtree                     = false;
  bool elQCDonly                     = false;
  bool noweightQCD                   = false;
  // plotting options
  bool doInputPlots                  = true;
  bool doBasicPlots                  = false;
  bool doExtendedPlots               = false;
  bool doTLVPlots                    = false;
  bool doPreTagPlots                 = false;
  // blinding window
  double mbbResolvedLowEdge          = 110e3;
  double mbbResolvedHighEdge         = 140e3;
  // Replace Line // <<<---- SJ01 Revert Point
  //double mbbResolvedLowEdge          = 0; //110e3;
  //double mbbResolvedHighEdge         = 100000000000e3; //140e3;
  //              // <<<---- SJ01 Revert Point
  double mbbMergedLowEdge            =  75e3;
  double mbbMergedHighEdge           = 145e3;

  // misc options
  bool printEventWeightsForCutFlow   = false;

  // read in parameters from config and overwrite, if exist
  m_config->getif<bool>("doMuonInJetCorrection",       doMuonInJetCorrection);
  m_config->getif<bool>("doMbbRescaling",              doMbbRescaling);
  m_config->getif<bool>("doBlindingData",              doBlindingData);
  m_config->getif<bool>("doBlindingMC",                doBlindingMC);
  m_config->getif<bool>("doInputPlots",                doInputPlots);
  m_config->getif<bool>("doBasicPlots",                doBasicPlots);
  m_config->getif<bool>("doTLVPlots",                  doTLVPlots);
  m_config->getif<bool>("doExtendedPlots",                  doExtendedPlots);
  m_config->getif<bool>("doPreTagPlots",               doPreTagPlots);
  m_config->getif<bool>("doMJEtree",                   doMJEtree);
  m_config->getif<bool>("elQCDonly",                   elQCDonly);
  m_config->getif<bool>("noweightQCD",                 noweightQCD);
  m_config->getif<bool>("printEventWeightsForCutFlow", printEventWeightsForCutFlow);
  m_config->getif<double>("mbbResolvedLowEdge",        mbbResolvedLowEdge);
  m_config->getif<double>("mbbResolvedHighEdge",       mbbResolvedHighEdge);
  m_config->getif<double>("mbbMergedLowEdge",          mbbMergedLowEdge);
  m_config->getif<double>("mbbMergedHighEdge",         mbbMergedHighEdge);

  //*********************************************************//
  //                     EVENT INITIALISATION                //
  //*********************************************************//

   // initialize eventFlag - all cuts are initialized to false by default
  unsigned long eventFlagResolved = 0;
  unsigned long eventFlagMerged   = 0;

  ResultVHbb1lep selectionResult = ((VHbb1lepEvtSelection*)m_eventSelection)->result();

  // reset physics metadata
  m_physicsMeta         = PhysicsMetadata();
  m_physicsMeta.channel = PhysicsMetadata::Channel::OneLep;
  //Set the Analysis type for histograms
  if(m_model == Model::HVT){
    if(m_debug) std::cout << " >>>>> Setting HistNameSvc::AnalysisType::VHres" << std::endl;
    m_histNameSvc->set_analysisType(HistNameSvc::AnalysisType::VHres);
    //m_histNameSvc->set_doHVTAnalysis(true);
    //m_histNameSvc->set_channel("lvJ");
  }else if(m_model == Model::SM){
    m_histNameSvc->set_analysisType(HistNameSvc::AnalysisType::CUT);
  }else if(m_model == Model::MVA){
    m_histNameSvc->set_analysisType(HistNameSvc::AnalysisType::MVA);
  }
  if(m_debug) std::cout << " >>>>> Event Initialisation complete " << std::endl;
  if(m_model == Model::HVT && m_debug) std::cout << " >>>>> Running HVT model " << std::endl;

  //if(m_debug) std::cout << " >>>>> HistNameSvc m_variation : " << m_histNameSvc->get_variation() << std::endl;

  //*********************************************************//
  //      INIT ALL VARIABLES FROM THE START                  //
  //*********************************************************//

  const xAOD::Electron  *el                 = selectionResult.el;
  const xAOD::Muon      *mu                 = selectionResult.mu;
  // Replace Line // <<<---- SJ01 Revert Point
  bool isE  = el ;
  bool isMu = mu ;
  //              // <<<---- SJ01 Revert Point
  const xAOD::MissingET *met                = selectionResult.met;
  std::vector<const xAOD::Jet*> signalJets  = selectionResult.signalJets;
  std::vector<const xAOD::Jet*> forwardJets = selectionResult.forwardJets;
  std::vector<const xAOD::Jet*> fatJets     = selectionResult.fatJets;
  std::vector<const xAOD::Jet*> trackJets   = selectionResult.trackJets;
  std::vector<const xAOD::TauJet*> taus     = selectionResult.taus;
  std::vector<const xAOD::Jet*> selectedJets;
  std::vector<const xAOD::Jet*> associatedTrackJets;
  //std::vector<const xAOD::Jet*> unassociatedTrackJets;

  //HVT anakysis performed Mu-FatJet OR due to muon in jet four momentum correction
  //4-vector duplication
  if(m_model == Model::HVT){
    std::vector<const xAOD::Muon*> Muons = {mu};
    fatJets = ApplyMuonFatJetOR(Muons, fatJets);
    if(m_debug) std::cout << " >>>>> # Fat Jets : " << fatJets.size() << std::endl;
  }

  int nSignalJet  = signalJets.size();
  int nForwardJet = forwardJets.size();
  int nJet        = nSignalJet + nForwardJet;
  int nFatJet     = fatJets.size();
  setevent_nJets(signalJets, forwardJets);

  // **Definition** :  forward jets
  TLorentzVector fwdjet1Vec, fwdjet2Vec, fwdjet3Vec;
  if (nForwardJet >= 1) fwdjet1Vec = forwardJets.at(0)->p4();
  if (nForwardJet >= 2) fwdjet2Vec = forwardJets.at(1)->p4();
  if (nForwardJet >= 3) fwdjet3Vec = forwardJets.at(2)->p4();

  // **Definition** :  signal jets
  TLorentzVector sigjet1Vec, sigjet2Vec, sigjet3Vec;
  if (nSignalJet >= 1) sigjet1Vec = signalJets.at(0)->p4();
  if (nSignalJet >= 2) sigjet2Vec = signalJets.at(1)->p4();
  if (nSignalJet >= 3) sigjet3Vec = signalJets.at(2)->p4();

  // **Definition** : define jet pair/triplet for anti-QCD cut
  TLorentzVector j1VecPresel, j2VecPresel, j3VecPresel;
  if (nSignalJet >= 1) j1VecPresel = signalJets.at(0)->p4();
  if (nSignalJet >= 2) j2VecPresel = signalJets.at(1)->p4();
  if (nSignalJet >= 3) j3VecPresel = signalJets.at(2)->p4();
  else if (nForwardJet >= 1) j3VecPresel = forwardJets.at(0)->p4();

  // **Definition** :  fat jet
  TLorentzVector fatjet1Vec, fatjet2Vec, fatjet3Vec;
  if (nFatJet >= 1) fatjet1Vec = fatJets.at(0)->p4();
  if (nFatJet >= 2) fatjet2Vec = fatJets.at(1)->p4();
  if (nFatJet >= 3) fatjet3Vec = fatJets.at(2)->p4();

  if(m_debug) std::cout << " >>>>> Formed Jet 4-Vectors " << std::endl;

  // **Definition** : b-tagging**
  if(m_doTruthTagging){
    compute_TRF_tagging(signalJets);
  }
  else{
    compute_btagging();
  }
  compute_fatjetTags(trackJets, fatJets, &associatedTrackJets/*, &unassociatedTrackJets*/);

  if(m_debug) std::cout << " >>>>> Calculated B-Jet Information " << std::endl;
  if(fatJets.size() >= 1 && m_debug) std::cout << " >>>>> Calculated nAddTags : " << Props::nAddBTags.get(fatJets.at(0)) << std::endl;

  int tagcatExcl = -1;
  tagjet_selection(signalJets, forwardJets, selectedJets, tagcatExcl);
  m_physicsMeta.nTags = tagcatExcl;
  setevent_flavour(selectedJets); // default from resolved analysis; is overwritten later
  int nSelectedJet = selectedJets.size();

  // **Definition**: define b-jet system
  TLorentzVector HVecJet, j1VecSel, j2VecSel, j3VecSel, bbjVec;
  if (nSelectedJet >= 1) j1VecSel   = selectedJets.at(0)->p4();
  if (nSelectedJet >= 2) j2VecSel   = selectedJets.at(1)->p4();
  if (nSelectedJet >= 3) j3VecSel   = selectedJets.at(2)->p4();
  if (nSelectedJet >= 2) HVecJet    = j1VecSel + j2VecSel;
  if (nSelectedJet >= 3) bbjVec     = HVecJet + j3VecSel ;

  // **Definition**: b-jet corrected vectors
  TLorentzVector HVecJetCorr, j1VecCorr, j2VecCorr, j3VecCorr, bbjVecCorr;
  j1VecCorr = j1VecSel; // fall back
  j2VecCorr = j2VecSel;
  j3VecCorr = j3VecSel;
  if ((nSelectedJet >= 1) && BTagProps::isTagged.get(selectedJets.at(0))) {
    EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(selectedJets.at(0),j1VecCorr,false,m_jetCorrType));
  }
  if ((nSelectedJet >= 2) && BTagProps::isTagged.get(selectedJets.at(1))) {
    EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(selectedJets.at(1),j2VecCorr,false,m_jetCorrType));
  }
  if ((nSelectedJet >= 3) && BTagProps::isTagged.get(selectedJets.at(2))) {
    EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(selectedJets.at(2),j3VecCorr,false,m_jetCorrType));
  }
  if (nSelectedJet >= 2) HVecJetCorr = j1VecCorr + j2VecCorr;
  if (nSelectedJet >= 3) bbjVecCorr  = HVecJetCorr + j3VecCorr; // should use corrected 3rd jet?

  // **Definition**: b-jet corrected fat jets
  TLorentzVector fj1VecCorr, fj2VecCorr, fj3VecCorr;
  fj1VecCorr = fatjet1Vec; // fall back
  fj2VecCorr = fatjet2Vec;
  fj3VecCorr = fatjet3Vec;
  // if ((nFatJet >= 1) && Props::nBTagsAll.get(fatJets.at(0))) {
  //   EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(fatJets.at(0),fj1VecCorr,false,m_fatJetCorrType));
  // }
  // if ((nFatJet >= 2) && Props::nBTagsAll.get(fatJets.at(1))) {
  //   EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(fatJets.at(1),fj2VecCorr,false,m_fatJetCorrType));
  // }
  // if ((nFatJet >= 3) && Props::nBTagsAll.get(fatJets.at(2))) {
  //   EL_CHECK("AnalysisReader_VHbb::setJetVariables() ",getMuonInJetCorrTLV(fatJets.at(2),fj3VecCorr,false,m_fatJetCorrType));
  // }

  if(m_debug) std::cout << " >>>>> MuonInJet FatJet correction complete" << std::endl;

  // **Definition**: mbb rescaling
  TLorentzVector j1VecRW, j2VecRW, HVecJetRW;
  j1VecRW = j1VecCorr; // baseline
  j2VecRW = j2VecCorr;
  HVecJetRW = HVecJetCorr;
  if (doMbbRescaling && (HVecJetCorr.M() > mbbResolvedLowEdge) && (HVecJetCorr.M() < mbbResolvedHighEdge)) {
    rescale_jets(HVecJetCorr.M(), j1VecRW, j2VecRW);
  }
  if (nSelectedJet >= 2) HVecJetRW = j1VecRW + j2VecRW;

  TLorentzVector fatJetRW;
  fatJetRW = fj1VecCorr;
  if (doMbbRescaling && (fj1VecCorr.M() > mbbMergedLowEdge) && (fj1VecCorr.M() < mbbMergedHighEdge) && m_model != Model::HVT) {
    rescale_fatjet(fj1VecCorr.M(), fatJetRW);
  }

  if(m_debug) std::cout << " >>>>> FatJet mBB rescaling complete" << std::endl;

   // **Definition** : met
  TLorentzVector metVec, metVecJetCorr, metVecJetRW, metVecFJCorr, metVecFJRW;
  if (met) {
    metVec.SetPtEtaPhiM(met->met(), 0, met->phi(), 0);
    if (nSelectedJet >= 2) metVecJetCorr = getMETCorrTLV(met, {&j1VecSel, &j2VecSel}, {&j1VecCorr, &j2VecCorr});
    if (nSelectedJet >= 2) metVecJetRW   = getMETCorrTLV(met, {&j1VecSel, &j2VecSel}, {&j1VecRW, &j2VecRW});
    if (nFatJet >= 1)      metVecFJCorr  = getMETCorrTLV(met, {&fatjet1Vec}, {&fj1VecCorr});
    if (nFatJet >= 1)      metVecFJRW    = getMETCorrTLV(met, {&fatjet1Vec}, {&fatJetRW});
  }

  // **Definition** : lepton
  TLorentzVector lepVec;
  int nLooseEl = 0;
  int nSigEl   = 0;
  int nLooseMu = 0;
  int nSigMu   = 0;
  if (el)  {
    lepVec = el->p4();
    if (Props::isVHLooseElectron.get(el) == 1) nLooseEl = 1;
    if (Props::isWHSignalElectron.get(el) == 1) nSigEl = 1;
  }
  else if (mu) {
    lepVec = mu->p4();
    if (Props::isVHLooseMuon.get(mu) == 1) nLooseMu = 1;
    if (Props::isWHSignalMuon.get(mu) == 1) nSigMu = 1;
  } // Replace Line // <<<----- SJ01 Revert Point
  else {
    Error("run_1Lep_analysis()", "Missing lepton!");
    return EL::StatusCode::FAILURE;
  }
  //                // <<<---- SJ01 Revert Point

  // **Definition** : neutrino using W mass constraint
  TLorentzVector nuVec        = getNeutrinoTLV(metVec, lepVec, true);
  TLorentzVector nuVecJetCorr = getNeutrinoTLV(metVecJetCorr, lepVec, true);
  TLorentzVector nuVecJetRW   = getNeutrinoTLV(metVecJetRW, lepVec, true);
  TLorentzVector nuVecFJCorr  = getNeutrinoTLV(metVecFJCorr, lepVec, true);
  TLorentzVector nuVecFJRW    = getNeutrinoTLV(metVecFJRW, lepVec, true);

  // **Definition** : W
  TLorentzVector lepVecT;
  lepVecT.SetPtEtaPhiM(lepVec.Pt(), 0, lepVec.Phi(), 0);
  TLorentzVector WVecT       = lepVecT + metVec;
  TLorentzVector WVec        = lepVec + nuVec;
  TLorentzVector WVecJetCorr = lepVec + nuVecJetCorr;
  TLorentzVector WVecJetRW   = lepVec + nuVecJetRW;
  TLorentzVector WVecFJCorr  = lepVec + nuVecFJCorr;
  TLorentzVector WVecFJRW    = lepVec + nuVecFJRW;

  // **Definition** : VH resonance
  TLorentzVector VHVecJet,  VHVecJetCorr, VHVecJetRW, VHVecFat, VHVecFatCorr, VHVecFatRW;
  if (nSelectedJet >= 2) VHVecJet     = WVec + HVecJet;
  if (nSelectedJet >= 2) VHVecJetCorr = WVecJetCorr + HVecJetCorr;
  if (nSelectedJet >= 2) VHVecJetRW   = WVecJetRW + HVecJetCorr;
  if (nFatJet >= 1)      VHVecFat     = WVec + fatjet1Vec;
  if (nFatJet >= 1)      VHVecFatCorr = WVecFJCorr + fj1VecCorr;
  if (nFatJet >= 1)      VHVecFatRW   = WVecFJRW + fatJetRW;

  if(m_debug) std::cout << " >>>>> Formulated VH system 4-Vector" << std::endl;

  //*********************************************************//
  //      DECISION TO USE MERGED OR RESOLVED ANALYSIS        //
  //*********************************************************//
  bool passMerged   = (nFatJet                >= 1);
  bool passResolved = (m_physicsMeta.nSigJet  >= 2);

  if(m_debug) std::cout << " >>>>> Resolved/Merged Analysis Decision " << std::endl;
  if(m_debug) std::cout << "       passMerged : " << passMerged << std::endl;
  if(m_debug) std::cout << "       passResolved : " << passResolved << std::endl;
  if(m_debug) std::cout << "       m_analysisStrategy : " << m_analysisStrategy << std::endl;

  // Possibility to turn off merged analysis
  if ( m_analysisStrategy == "Resolved" ) {
    if ( passResolved ) m_physicsMeta.regime = PhysicsMetadata::Regime::resolved;
  }
  else if ( m_analysisStrategy == "Merged" ) {
    if ( passMerged ) m_physicsMeta.regime = PhysicsMetadata::Regime::merged;
  }
  else if ( m_analysisStrategy == "RecyclePtV" ) {
    // Give priority to merged above 500 GeV
    if ( WVecT.Pt()/1.e3 > 500. ) {
      if ( passMerged ) m_physicsMeta.regime = PhysicsMetadata::Regime::merged;
      else if ( passResolved ) m_physicsMeta.regime = PhysicsMetadata::Regime::resolved;
    }
    else {
      if ( passResolved ) m_physicsMeta.regime = PhysicsMetadata::Regime::resolved;
      else if ( passMerged )  m_physicsMeta.regime = PhysicsMetadata::Regime::merged;
    }
  }
  else if ( m_analysisStrategy == "SimpleMerge500" ) {
    // Only merged above 500 GeV
    if ( WVecT.Pt()/1.e3 > 500. ) {
      if ( passMerged ) m_physicsMeta.regime = PhysicsMetadata::Regime::merged;
    }
    else {
      if ( passResolved ) m_physicsMeta.regime = PhysicsMetadata::Regime::resolved;
    }
  }
  else if ( m_analysisStrategy == "PriorityResolved" ) {
    if ( passResolved ) m_physicsMeta.regime = PhysicsMetadata::Regime::resolved;
    else if ( passMerged ) m_physicsMeta.regime = PhysicsMetadata::Regime::merged;
  }

  bool isResolved = (m_physicsMeta.regime == PhysicsMetadata::Regime::resolved);
  bool isMerged   = (m_physicsMeta.regime == PhysicsMetadata::Regime::merged);

  if(m_debug) std::cout << " >>>>> Resolved/Merged Analysis Decision Complete" << std::endl;
  if(m_debug) std::cout << "       isMerged : " << isMerged << std::endl;
  if(m_debug) std::cout << "       isResolved : " << isResolved << std::endl;

  //*********************************************************//
  //                    CALCULATE WEIGHTS                    //
  //*********************************************************//

  // **Weight**: Lepton
  double leptonSF = 1.;
  if (m_isMC) {
    leptonSF = Props::leptonSF.get(m_eventInfo);
  }
  m_weight *= leptonSF;

  // **Weight** : Trigger
  double triggerSF = 1.;
  bool isTriggered =  pass1LepTrigger(triggerSF,selectionResult);
  // if (m_isMC) m_weight *= triggerSF; <- is done already inside pass1LepTrigger
  //if(triggerSF==0)std::cout<<"El Mu "<<el<<" "<<mu<<std::endl;
  // **Weight** : b-tagging
  float btagWeight = 1.; // for later use
  //btagWeight is truth_tag_weight _with_ SFs if m_doTruthTagging (getEfficiency() includes SFs)
  if (m_isMC && isResolved) {
    // We set the bTagTool's jet author to the authorName argument passed to the computeBTagSFWeight now just in case
    btagWeight = computeBTagSFWeight(signalJets, m_jetReader->getContainerName());
  } else if ( m_isMC && isMerged ) {
    btagWeight = computeBTagSFWeight(associatedTrackJets, m_trackJetReader->getContainerName());
  }
  m_weight  *= btagWeight;

  if (m_isMC && m_doTruthTagging) BTagProps::truthTagEventWeight.set(m_eventInfo,btagWeight);

  // **Weight** : Pileup
  float puWeight = 1.;
  if (m_isMC) puWeight = Props::PileupweightRecalc.get(m_eventInfo);
  if(!m_config->get<bool>("applyPUWeight")) m_weightSysts.push_back({"_withPU",(float)(puWeight)});

  double ff_weight = 1.0;
  // **Weight** : FakeFactor method for QCD
  if(m_doQCD && el && nSigEl==1){
    double etcone = Props::topoetcone20.get(el);
    double ptcone = Props::ptvarcone20.get(el);
     double d0sigbl = Props::d0sigBL.get(el);
     bool mediumid =  Props::isMediumLH.get(el);
     bool tightid =  Props::isTightLH.get(el);
     m_FakeFactor_el->set_parameters(WVecT.Pt()/1000.,lepVec.Pt()/1000.,fabs(lepVec.Eta()),fabs(lepVec.DeltaPhi(metVec)),metVec.Pt()/1000.,etcone/1000.,d0sigbl,mediumid,tightid);
     //double fake_factor = m_FakeFactor_el->get_fakefactor(m_currentVar);    
     if(m_currentVar=="MJ_Mu_METstr")ff_weight = m_FakeFactor_el->get_weight("Nominal");
     else ff_weight = m_FakeFactor_el->get_weight(m_currentVar);

     
     if(!((ptcone/lepVec.Pt())<0.06))return EL::StatusCode::SUCCESS;


     if(!noweightQCD){
       m_weight *= ff_weight;
       if(m_isMC)m_weight *= -1;
     }
     else {
       if(!tightid)return EL::StatusCode::SUCCESS;
       if((etcone/lepVec.Pt())<0.06)return EL::StatusCode::SUCCESS;
     }
     //if(m_weight!=0&&WVecT.Pt()/1000. > 150&&m_currentVar=="Nominal")std::cout<<"----------------"<<std::endl;     
     //if(m_weight!=0&&WVecT.Pt()/1000. > 150)std::cout<<m_currentVar<<" "<<WVecT.Pt()/1000.<<" "<<lepVec.Pt()/1000.<<" "<<fabs(lepVec.Eta())<<" "<<fabs(lepVec.DeltaPhi(metVec))<<"       "<<metVec.Pt()/1000.<<"         "<<etcone/1000.<<" "<<d0sigbl<<" "<<mediumid<<" "<<tightid<<" "<<ff_weight<<std::endl;
     //std::cout<<"El: "<<"current "<< m_currentVar<<" weight "<<ff_weight<<std::endl;
     //if(m_weight!=0&&WVecT.Pt()/1000. > 150)std::cout<<m_currentVar<<" El  "<<WVecT.Pt()/1000.<<" "<<lepVec.Pt()/1000.<<" "<<fabs(lepVec.Eta())<<" "<<fabs(lepVec.DeltaPhi(metVec))<<"       "<<metVec.Pt()/1000.<<"         "<<etcone/1000.<<" "<<d0sigbl<<" "<<mediumid<<" "<<tightid<<" "<<ff_weight<<std::endl; 
  }
  if(m_doQCD && mu && nSigMu==1){
    double ptcone = Props::ptvarcone30.get(mu);
    double d0sigbl = Props::d0sigBL.get(mu);
    bool mediumid =  Props::isMedium.get(mu);
    bool tightid =  Props::isTight.get(mu);
    m_FakeFactor_mu->set_parameters(WVecT.Pt()/1000.,lepVec.Pt()/1000.,fabs(lepVec.Eta()),fabs(lepVec.DeltaPhi(metVec)),metVec.Pt()/1000.,ptcone/1000.,d0sigbl,mediumid,tightid);
    //double fake_factor = m_FakeFactor_mu->get_fakefactor(m_currentVar);    
    if(m_currentVar=="MJ_El_EWK")ff_weight = m_FakeFactor_mu->get_weight("MJ_El_EWK");//Nominal only now    
    else ff_weight = m_FakeFactor_mu->get_weight("Nominal");//Nominal only now

    if(!noweightQCD){
      m_weight *= ff_weight;
      if(m_currentVar=="MJ_Mu_METstr")m_weight *= get_sys_metstrmu(WVecT.M()/1000.);
      if(m_isMC)m_weight *= -1;
    }
    else{
      if((ptcone/lepVec.Pt())<0.06)return EL::StatusCode::SUCCESS;
    }
    // if(m_weight!=0&&WVecT.Pt()/1000. > 150)std::cout<<m_currentVar<<" Mu  "<<WVecT.Pt()/1000.<<" "<<lepVec.Pt()/1000.<<" "<<fabs(lepVec.Eta())<<" "<<fabs(lepVec.DeltaPhi(metVec))<<"       "<<metVec.Pt()/1000.<<"         "<<ptcone/1000.<<" "<<d0sigbl<<" "<<mediumid<<" "<<tightid<<" "<<ff_weight<<std::endl;

      //if(elQCDonly)m_weight=0;
    if(elQCDonly)return EL::StatusCode::SUCCESS;
  }
  if(m_debug) std::cout << " >>>>> Applied WEights" << std::endl;


  //-------------------------
  // TTbar systematics histos
  //--------------------------
  //NOTE :: mVH/1e3 !!!!!!!!!!!!!!!!!!!
  if ( isMerged && m_histNameSvc -> get_sample() == "ttbar" && m_currentVar == "Nominal" ) {
    float  mVH = doMuonInJetCorrection ? VHVecFatCorr.M() : VHVecFatRW.M();
    fillTTbarSystslep(m_physicsMeta.nTags, Props::nAddBTags.get(fatJets.at(0)), mVH/1e3);
  }

  //-----------------------------
  // VJets Systematic Histograms
  //-----------------------------
  //NOTE :: mBB is in MeV because CorrsAndSysts uses MeV !!!!!!!!!!!!!!!!!!!
  if ( isMerged && (m_histNameSvc -> get_sample() == "W" ||
		    m_histNameSvc -> get_sample() == "Z" ) &&
       m_currentVar == "Nominal") {
    double mBB = doMuonInJetCorrection ? fj1VecCorr.M() : fatJetRW.M();
    float  mVH = doMuonInJetCorrection ? VHVecFatCorr.M() : VHVecFatRW.M();
    fillVjetsSystslep( mBB, mVH/1e3);
  }

  //*********************************************************//
  //                  RESOLVED BITFLAG / CUTFLOW             //
  //*********************************************************//

  if(m_debug) std::cout << " >>>>> Resolved BitFlag" << std::endl;

  // C0: All events (CxAOD)
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::AllCxAOD);

  // C1: trigger
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::Trigger, isTriggered);

  // C2: 1 signal lepton
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::LooseLeptons, nLooseEl + nLooseMu == 1);

  // C2: 1 Loose lepton
  //updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons, (nLooseEl + nLooseMu == 1));
  //updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons);

  // C3: 1 signal lepton
  //if(m_doQCD)updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons, (nSigEl + nLooseMu == 1));
  //if(m_doQCD)updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons, (nSigEl + nSigMu== 1));
  //if(m_doQCD)updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons, (nSigEl == 1));
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SignalLeptons, (nSigEl + nSigMu == 1));

  // C4: njets
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::AtLeast2Jets, nJet >= 2);

  // C5: nSignalJets
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::AtLeast2SigJets, nSignalJet >= 2);

  // C6: pt > 45
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::Pt45, j1VecSel.Pt() > 45e3);
  
  // C7, C8 MET mTW cut
  if(WVecT.Pt() > 150e3){
    if( nSigEl == 1 && nSigMu == 0 ) updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 30e3);
    else if( nSigEl == 0 && nSigMu == 1 ) updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 0);
    updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW, WVecT.M() > 0e3); // No mTW cut at high pTV                                      
  }
  else if(WVecT.Pt() < 150e3){
    if( nSigEl == 1 && nSigMu == 0 ){
      if( nJet == 2 ) {
  	updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 30e3);        
  	updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW, WVecT.M() > 40e3); // 30 GeV mTW cut for low  WpT, 2jet electron channel            
	
      }
      else if ( nJet == 3 ) {
  	updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 20e3);
        updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW, WVecT.M() > 20e3); // 20 GeV mTW cut for low  WpT, 3 jet electron channel           
      }
      else{
  	updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 0e3);
  	updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW, WVecT.M() > 0e3);         
      }
    }
    else if( nSigEl == 0 && nSigMu == 1 ){
      updateFlag(eventFlagResolved, OneLeptonResolvedCuts::MET, metVec.Pt() > 0e3);
      updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW, WVecT.M() > 0e3); // cuts for muon channel are still to be finalised                     
    }
    else {
      Error("run_1lep()", "Invalid lepton type");
      return EL::StatusCode::FAILURE;
    }
  }

  // C9: 3rd b-jet veto
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::Veto3bjet, tagcatExcl < 3);

  // mbb corrected
  bool is_inside_mbbCorr = (HVecJetCorr.M() > mbbResolvedLowEdge) && (HVecJetCorr.M() < mbbResolvedHighEdge);
  //updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mbbCorr, is_inside_mbbCorr);
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mbbCorr);

  // pTV
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::pTV, WVecT.Pt()/1.e3 > 150);


  // dRBB cut in Cut-based
  if(WVecT.Pt() > 150e3 && WVecT.Pt() <= 200e3)updateFlag(eventFlagResolved, OneLeptonResolvedCuts::dRBB, j1VecSel.DeltaR(j2VecSel) < 1.8);
  else if(WVecT.Pt() > 200e3)updateFlag(eventFlagResolved, OneLeptonResolvedCuts::dRBB, j1VecSel.DeltaR(j2VecSel) < 1.2);

  // additional mTW cut in Cut-based
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::mTW_add,  WVecT.M() < 120e3);


  // dphi
  double mindPhi1 = fabs(j1VecPresel.DeltaPhi(metVec));
  double mindPhi2 = fabs(j2VecPresel.DeltaPhi(metVec));
  double mindPhi3 = 1000;
  if (nJet >= 3) mindPhi3 = fabs(j3VecPresel.DeltaPhi(metVec));
  double mindPhi = std::min(mindPhi1, std::min(mindPhi2, mindPhi3));

  // for SR flags, require all cuts up to C10
  bool passAllResolved = passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV, { });
  if(m_model == Model::SM) passAllResolved = passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::mTW_add, { });
  bool passPreTagResolved = passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::mTW, { });
  // C10: SR_0tag_2jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_0tag_2jet , passAllResolved && (tagcatExcl == 0) && (nJet == 2));  
  // C11: SR_0tag_3jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_0tag_3jet , passAllResolved && (tagcatExcl == 0) && (nJet == 3));  
  // C12: SR_0tag_4pjet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_0tag_4pjet , passAllResolved && (tagcatExcl == 0) && (nJet >= 4));  
  // C13: SR_1tag_2jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_1tag_2jet , passAllResolved && (tagcatExcl == 1) && (nJet == 2));  
  // C14: SR_1tag_3jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_1tag_3jet , passAllResolved && (tagcatExcl == 1) && (nJet == 3));  
  // C15: SR_1tag_4pjet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_1tag_4pjet , passAllResolved && (tagcatExcl == 1) && (nJet >= 4));  
  // C16: SR_2tag_2jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_2tag_2jet , passAllResolved && (tagcatExcl == 2) && (nJet == 2));  
  // C17: SR_2tag_3jet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_2tag_3jet , passAllResolved && (tagcatExcl == 2) && (nJet == 3));  
  // C18: SR_2tag_4pjet
  updateFlag(eventFlagResolved, OneLeptonResolvedCuts::SR_2tag_4pjet , passAllResolved && (tagcatExcl == 2) && (nJet >= 4));  

  //             // <<<---- SJ01 Revert Point

  //*********************************************************//
  //                  MERGED BITFLAG / CUTFLOW               //
  //*********************************************************//

  if(m_debug) std::cout << " >>>>> Merged BitFlag" << std::endl;

  // M0: All events (CxAOD)
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::AllCxAOD);

  // M1: trigger
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::Trigger, isTriggered);

  // M2: 1 signal lepton
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::Leptons, ((nLooseEl + nLooseMu == 1) && (nSigEl + nSigMu == 1)));

  // M3: MET cut
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::MET, metVec.Pt() > 100e3);

  // M4: nFatjets
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::AtLeast1FatJet, nFatJet >= 1);

  // M5: corrected mass fatjet
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::mbbCorr, (fj1VecCorr.M() > mbbMergedLowEdge) && (fj1VecCorr.M() < mbbMergedHighEdge) );

  // M6: number of associated track jets
  if(m_model == Model::SM)  updateFlag(eventFlagMerged, OneLeptonMergedCuts::AtLeast2TrackJets, associatedTrackJets.size() >= 2);
  else if(m_model == Model::HVT) updateFlag(eventFlagMerged, OneLeptonMergedCuts::AtLeast2TrackJets, associatedTrackJets.size() >= 1);

  // M7: pTV > 500
  if( m_analysisStrategy == "SimpleMerge500"){
    updateFlag(eventFlagMerged, OneLeptonMergedCuts::pTV, WVecT.Pt()/1.e3 > 500.);
  }else if( m_analysisStrategy == "Merged"){
    updateFlag(eventFlagMerged, OneLeptonMergedCuts::pTV, WVecT.Pt()/1.e3 > 0.0);
  }

  // for SR flags, require all cuts up to M7
 
  bool passAllMerged = passAllCutsUpTo(eventFlagMerged, OneLeptonMergedCuts::pTV, { });
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::SR_0tag_1pfat0pjets,  passAllMerged && (Props::nBTags.get(fatJets.at(0)) == 0)); // M8
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::SR_1tag_1pfat0pjets,  passAllMerged && (Props::nBTags.get(fatJets.at(0)) == 1)); // M9
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::SR_2tag_1pfat0pjets,  passAllMerged && (Props::nBTags.get(fatJets.at(0)) == 2)); // M10
  updateFlag(eventFlagMerged, OneLeptonMergedCuts::SR_3ptag_1pfat0pjets, passAllMerged && (Props::nBTags.get(fatJets.at(0)) == 3)); // M11

  //*********************************************************//
  //         EVENT CATEGORIZATION BASED ON BIT FLAG          //
  //*********************************************************//

  if(m_debug) std::cout << " >>>>> Event Categoristion Start" << std::endl;

  m_histNameSvc->set_description("");

  // leptons
  m_histNameSvc->set_pTV(WVecT.Pt());

  // here the difference is made between merged and resolved in histnames
  if ( isResolved ) {
    setevent_flavour(selectedJets);
    m_histNameSvc->set_nTag(tagcatExcl);
    m_histNameSvc->set_nJet(m_physicsMeta.nJets);
    //m_histNameSvc->set_nJet(m_physicsMeta.nJets);
    if( m_doMergeJetBins ) m_histNameSvc->set_nJet( m_physicsMeta.nJets >= 2 ? -2 : m_physicsMeta.nJets);
    else m_histNameSvc->set_nJet( m_physicsMeta.nJets );
    //m_histNameSvc->set_Resolved(isResolved);
  } else if (isMerged) {
    if (m_config->get<string>("truthLabeling") == "TrackJetCone") {
      setevent_flavour(associatedTrackJets);
    } else if (m_config->get<string>("truthLabeling") == "TrackJetGhostAssHadrons") {
      setevent_flavourGhost(associatedTrackJets);
    } else if (m_config->get<string>("truthLabeling") == "FatJetGhostAssHadrons") {
      setevent_flavourGhost(fatJets);
    } else if (m_config->get<string>("truthLabeling") == "TrackJetHybrid") {
      setevent_flavour(associatedTrackJets);
      // if there is no tag for the second track (b2Flav < 0) or there is no
      // second track, tag it with ghost associated hadrons
      // NB: if we do not want to resolve Wl into two tags, replace the if
      //     statement with
      //       " if ((m_physicsMeta.b1Flav == 5 || m_physicsMeta.b1Flav == 4) && m_physicsMeta.b2Flav < 0) "
      if (m_physicsMeta.b2Flav < 0) {
	setevent_flavourGhost(associatedTrackJets);
      }
    } else if (m_config->get<string>("truthLabeling") == "FatJetHybrid") {
      setevent_flavour(associatedTrackJets);
      // same argument with Wl as in the TrackJetHybrid case (see above)
      if (m_physicsMeta.b2Flav < 0) {
	setevent_flavourGhost(fatJets);
      }
    } else {
      Error("run_1Lep_analysis()", "truthLabeling '%s' is not implemented",
            m_config->get<string>("truthLabeling").c_str());
    } // end of if branches for different truthLabeling options

    m_histNameSvc->set_nTag(Props::nBTags.get(fatJets.at(0)));
    m_histNameSvc->set_nFatJet(nFatJet);
    m_histNameSvc->set_nBTagTrackJetUnmatched(
        Props::nAddBTags.get(fatJets.at(0)));
  } // end of if ( isMerged )

  m_histNameSvc->set_eventFlavour(m_physicsMeta.b1Flav, m_physicsMeta.b2Flav);

  // Region
  if ( isResolved ) {
    //if ( passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV, { OneLeptonResolvedCuts::mbbCorr } ) ) { // all signal cuts; ignore mbb
    if ( passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV) ) { 
      //if (passSpecificCuts(eventFlagResolved, { OneLeptonResolvedCuts::mbbCorr })) m_histNameSvc->set_description("SR");
       m_histNameSvc->set_description("SR");
      // else if( m_model == Model::SM) m_histNameSvc->set_description("mBBcr");
      // else if( m_model == Model::HVT ){
      // 	std::string CR_Name ç∂= (HVecJetCorr.M() < mbbResolvedLowEdge) ? "lowmBBcr" : "highmBBcr";
      // 	m_histNameSvc->set_description(CR_Name);
      // }
      if(m_debug) std::cout << " >>>>> EventCategory : " << m_histNameSvc->getFullHistName("mBB") << std::endl;
    }
  } else if ( isMerged ) {

    if ( passAllCutsUpTo(eventFlagMerged, OneLeptonMergedCuts::pTV, { OneLeptonMergedCuts::mbbCorr } ) ) { // all signal cuts; ignore mbb
      if (passSpecificCuts(eventFlagMerged, { OneLeptonMergedCuts::mbbCorr })) m_histNameSvc->set_description("SR");
      else{
        if( doMuonInJetCorrection ){
	  std::string CR_Name = fj1VecCorr.M() < mbbMergedLowEdge ? "lowmBBcr" : "highmBBcr";
          fj1VecCorr.M() < mbbMergedLowEdge ? m_physicsMeta.mbbSideBandMerged = PhysicsMetadata::MbbSideBandMerged::Low : m_physicsMeta.mbbSideBandMerged = PhysicsMetadata::MbbSideBandMerged::High;
          m_histNameSvc->set_description(CR_Name);
        }else if( doMbbRescaling ){
	  std::string CR_Name = fatJetRW.M() < mbbMergedLowEdge ? "lowmBBcr" : "highmBBcr";
          fatJetRW.M() < mbbMergedLowEdge ? m_physicsMeta.mbbSideBandMerged = PhysicsMetadata::MbbSideBandMerged::Low : m_physicsMeta.mbbSideBandMerged = PhysicsMetadata::MbbSideBandMerged::High;
          m_histNameSvc->set_description(CR_Name);
        }else{
	  std::string CR_Name = fatjet1Vec.M() < mbbMergedLowEdge ? "lowmBBcr" : "highmBBcr";
          m_histNameSvc->set_description(CR_Name);
        }
      }
      if(m_debug) std::cout << " >>>>> EventCategory : " << m_histNameSvc->getFullHistName("mVH") << std::endl;
    }

  }

  //if(m_debug) std::cout << " >>>>> EventCategory : " << m_histNameSvc->getFullHistName("mVH") << std::endl;

  // Check if event in blinded region
  bool isBlindingRegion = (isResolved && passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV, {} ) && (m_histNameSvc->get_nTag() == 1))
                       || (isResolved && passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV, {} ) && (m_histNameSvc->get_nTag() == 2))
                       || (isMerged   && passAllCutsUpTo(eventFlagMerged,   OneLeptonMergedCuts::pTV,   {} ) && (m_histNameSvc->get_nTag() == 1))
                       || (isMerged   && passAllCutsUpTo(eventFlagMerged,   OneLeptonMergedCuts::pTV,   {} ) && (m_histNameSvc->get_nTag() == 2));

  m_doBlinding = isBlindingRegion && ((m_isMC && doBlindingMC) || (!m_isMC && doBlindingData));

  //*********************************************************//
  //                  CORRS AND SYS                          //
  // This part needs some props from histnamesvc             //
  // -> don't call earlier                                   //
  // This part changes weights                               //
  // -> don't fill histograms before                         //
  //*********************************************************//

  //TODO:
  //     1) Apply Model systematics for boosted HVT analysis

  if(m_debug) std::cout << " >>>>> CorrsAndSysts Applied" << std::endl;

  if (m_isMC && isResolved && ((m_csCorrections.size() != 0) || (m_csVariations.size() != 0))){
    // Check quantities (truth, reco, missing, ...) --> not well defined yet
      double cs_dr       = fabs(j1VecCorr.DeltaR(j2VecCorr));
      double cs_dphi     = fabs(j1VecCorr.DeltaPhi(j2VecCorr));
      double cs_vpt      = WVecT.Pt();
      double cs_mbb      = HVecJetCorr.M();
      double cs_truthPt  = WVecT.Pt(); // dummy - to change
      double cs_ptb1     = j1VecCorr.Pt();
      double cs_ptb2     = j2VecCorr.Pt();
      double cs_met      = metVec.Pt();
      double cs_njet     = m_physicsMeta.nJets;
      double cs_ntag     = tagcatExcl;
      double cs_avgTopPt = WVecT.Pt(); // dummy - to change
      float nnlo_ttbarPt = 0;
      float nnlo_topPt = 0;
      applyCS(cs_vpt, cs_mbb, cs_truthPt, cs_dphi, cs_dr, cs_ptb1, cs_ptb2, cs_met, cs_avgTopPt, cs_njet, cs_ntag);
      truthVariablesNNLO(nnlo_ttbarPt,nnlo_topPt);
      NNLORW(nnlo_ttbarPt, nnlo_topPt);
  }

  //*********************************************************//
  //                       MVA TREE                          //
  //*********************************************************//

  if(m_debug) std::cout << " >>>>> MVA Tree" << std::endl;


  float Mtop        = calculateMtop(lepVec, metVec, j1VecCorr, j2VecCorr);
  float dYWH        = calculatedYWH(lepVec, metVec, j1VecCorr, j2VecCorr);

  // reset MVA tree variables
  m_tree->Reset();
  m_tree->SetVariation(m_currentVar);

  m_tree->sample      = m_histNameSvc->getFullSample();
  m_tree->EventWeight = m_weight;
  m_tree->EventNumber = m_eventInfo->eventNumber();
  m_tree->nJ          = nJet;
  m_tree->dRBB        = j1VecCorr.DeltaR(j2VecCorr);
  m_tree->dPhiBB      = j1VecCorr.DeltaPhi(j2VecCorr);
  m_tree->dEtaBB      = fabs(j1VecCorr.Eta() - j2VecCorr.Eta());
  m_tree->mBB    = HVecJetCorr.M();   
  m_tree->mBB_corr    = HVecJetCorr.M(); 
  m_tree->pTB1        = j1VecCorr.Pt();  
  m_tree->pTB2        = j2VecCorr.Pt();
  m_tree->MET         = metVec.Pt();
  m_tree->mTW         = WVecT.M();  
  m_tree->Mtop        = Mtop;  
  m_tree->pTV         = WVecT.Pt();  
  m_tree->dYWH        = dYWH;
  m_tree->pTL         = lepVec.Pt();
  m_tree->etaL        = lepVec.Eta();
  //  m_tree->dPhiVBB     = fabs(WVecT.DeltaPhi(HVecJetCorr));
  m_tree->dPhiVBB     = fabs(WVecT.DeltaPhi(HVecJet));
  m_tree->dPhiLBmin   = std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr)));
  m_tree->dEtaWH      = fabs(WVec.Eta() - HVecJet.Eta());
  m_tree->MtopFullMETShift = calculateMtop(lepVec, metVec, j1VecCorr, j2VecCorr);
  m_tree->dPhiLMET    = fabs(lepVec.DeltaPhi(metVec));
  
  m_tree->j1Pt        = j1VecCorr.Pt() / 1e3;
  m_tree->j2Pt        = j2VecCorr.Pt() / 1e3;
  m_tree->met         = metVec.Pt() / 1e3;
  m_tree->WMt         = WVecT.M() / 1e3;
  m_tree->mtop        = Mtop / 1e3;
  m_tree->WPt         = WVecT.Pt() / 1e3;
  m_tree->jjM         = HVecJetCorr.M()/1e3;
  m_tree->jjdR        = j1VecCorr.DeltaR(j2VecCorr);
  m_tree->WjjdPhi     = fabs(WVecT.DeltaPhi(HVecJet));
  m_tree->ljmindPhi   = std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr)));


  m_tree->nTag        = tagcatExcl;
  m_tree->PUWeight    = puWeight;
  if(m_isMC)m_tree->MCWeight    = Props::MCEventWeight.get(m_eventInfo);
  else m_tree->MCWeight    = 1.0;
  // m_tree->CSweight    = PU;
  m_tree->btagWeight  = btagWeight;
  m_tree->triggerSF   =	triggerSF; 
  m_tree->leptonSF    =	leptonSF;  
  m_tree->QCDWeight   =	ff_weight;  
  
  if(HVecJetCorr.M()>175e3 && nJet==2)m_tree->mBB    = 175e3; 
  else if(HVecJetCorr.M()>350e3 && nJet==3)m_tree->mBB    = 350e3; 
  
  if(HVecJetCorr.M()>175e3 && nJet==2)m_tree->mBB_corr    = 175e3; 
  else if(HVecJetCorr.M()>350e3 && nJet==3)m_tree->mBB_corr    = 350e3; 

  if(j1VecCorr.Pt()>350e3)m_tree->pTB1    = 350e3; 
  if(j2VecCorr.Pt()>180e3)m_tree->pTB2    = 180e3; 
  if(metVec.Pt()>320e3)m_tree->MET         = 320e3;
  if(WVecT.M()>215e3) m_tree->mTW = 215e3;       
  if(WVecT.Pt()>450e3) m_tree->pTV         = 450e3;
  if(Mtop>1000e3)  m_tree->Mtop        = 1000e3;
  //m_tree->dYWH        = 
  //signalJets.at(0)->auxdata<int>("nrMuonInJet")
  // m_tree->MV1cB1 not set
  // m_tree->MV1cB2 not set

  if (nJet >= 3) {
    if(j3VecSel.Pt()>300e3)m_tree->pTJ3      = 300e3;
    else m_tree->pTJ3      = j3VecSel.Pt();
    if( bbjVecCorr.M()>1000e3)m_tree->mBBJ      = 1000e3;
    else m_tree->mBBJ      = bbjVecCorr.M();
    if(j3VecSel.Pt()>300e3)m_tree->j3Pt      = 300;
    else m_tree->j3Pt      = j3VecSel.Pt() / 1e3;
    if( bbjVecCorr.M()>1000e3)m_tree->jjjM      = 1000;
    else m_tree->jjjM      = bbjVecCorr.M() / 1e3;
  }

  std::vector<double> values{m_tree->dRBB, m_tree->mBB, m_tree->dPhiVBB, m_tree->dPhiLBmin, m_tree->pTV, m_tree->pTB1, m_tree->pTB2, m_tree->mTW, m_tree->Mtop, m_tree->dEtaWH, m_tree->MET};
  std::vector<float> bdt = m_mva->evaluateClassification(values);

  m_tree->ReadMVA(); 

  //  std::cout<<"BDT"<<m_tree->BDT<<std::endl;
  // std::cout<<"BDT_WZ"<<m_tree->BDT_WZ<<std::endl;
 
  // fill MVA tree; makes sure nothing is overwritten later
  if (m_histNameSvc->get_isNominal() && (m_histNameSvc->get_description() == "SR") ) {
    m_tree->Fill();
  }

  //*********************************************************//
  //                       EASY TREE                         //
  //*********************************************************//

  if(m_debug) std::cout << " >>>>> EASY TREE" << std::endl;

  // EasyTree reset
  m_etree->Reset();
  m_etree->SetVariation(m_currentVar);

  if ((nSelectedJet >= 2 || nFatJet >= 1)) {

    // event info
    m_etree->SetBranchAndValue<std::string>("Description", m_histNameSvc->get_description(), "");
    m_etree->SetBranchAndValue<std::string>("Sample", m_histNameSvc->getFullSample(), "");
    m_etree->SetBranchAndValue<std::string>("EventFlavor", m_histNameSvc->getEventFlavour(), "");
    m_etree->SetBranchAndValue<int>("EventNumber",   m_eventInfo->eventNumber(), -1);
    m_etree->SetBranchAndValue<float>("mu",          Props::averageInteractionsPerCrossingRecalc.get(m_eventInfo), -1.);

    // weights
    m_etree->SetBranchAndValue<float>("EventWeight", m_weight, -1.);
    m_etree->SetBranchAndValue<float>("PUWeight",    puWeight, -1.);
    m_etree->SetBranchAndValue<float>("BTagSF",      btagWeight, -1.);
    m_etree->SetBranchAndValue<float>("TriggerSF",   triggerSF , -1.);
    m_etree->SetBranchAndValue<float>("LeptonSF",    leptonSF,-1.);

    // objects counts
    m_etree->SetBranchAndValue<int>("nJets",         nJet, -1);
    m_etree->SetBranchAndValue<int>("nFats",         nFatJet, -1);
    m_etree->SetBranchAndValue<int>("nTaus",         taus.size(), -1);
    m_etree->SetBranchAndValue<int>("nTags",         m_physicsMeta.nTags, -1);
    m_etree->SetBranchAndValue<int>("nElectrons",    nSigEl, -1);
    m_etree->SetBranchAndValue<int>("nMuons",        nSigMu, -1);

    // event categorisation
//     m_etree->SetBranchAndValue<int>("isMerged",      isMerged, -1);
//     m_etree->SetBranchAndValue<int>("isResolved",    isResolved, -1);
    m_etree->SetBranchAndValue<int>("EventRegime",   static_cast<int>(m_physicsMeta.regime), -1);
    m_etree->SetBranchAndValue<unsigned long>("eventFlagResolved/l", eventFlagResolved, 0); // "/l" in the end to determine type
    m_etree->SetBranchAndValue<unsigned long>("eventFlagMerged/l", eventFlagMerged, 0);

    // general 1-lepton quantities
    m_etree->SetBranchAndValue<float>("lPt",         lepVec.Pt()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("lEta",        lepVec.Eta(), -10.);
    m_etree->SetBranchAndValue<float>("lPhi",        lepVec.Phi(), -10.);
    m_etree->SetBranchAndValue<float>("lM",          lepVec.M()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("WMt",         WVecT.M()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("WPt",         WVecT.Pt()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("WPhi",        WVecT.Phi(), -10);
    m_etree->SetBranchAndValue<float>("WM",          WVecT.M()/1e3, -1);
    m_etree->SetBranchAndValue<float>("met",         metVec.Pt()/1e3, -1.);
    float j1MV2c20 = -1.;
    if (nSelectedJet > 0) j1MV2c20 = Props::MV2c20.get(selectedJets.at(0));
    m_etree->SetBranchAndValue<float>("j1Pt",        j1VecCorr.Pt()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("j1Eta",       j1VecCorr.Eta(), -10.);
    m_etree->SetBranchAndValue<float>("j1Phi",       j1VecCorr.Phi(), -10.);
    m_etree->SetBranchAndValue<float>("j1M",         j1VecCorr.M()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("j1MV2c20",    j1MV2c20, -1.);
    m_etree->SetBranchAndValue<int>("j1Flav",        m_physicsMeta.b1Flav, -1);
    float j2MV2c20 = -1.;
    if (nSelectedJet > 1) j2MV2c20 = Props::MV2c20.get(selectedJets.at(1));
    m_etree->SetBranchAndValue<float>("j2Pt",        j2VecCorr.Pt()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("j2Eta",       j2VecCorr.Eta(), -10.);
    m_etree->SetBranchAndValue<float>("j2Phi",       j2VecCorr.Phi(), -10.);
    m_etree->SetBranchAndValue<float>("j2M",         j2VecCorr.M()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("j2MV2c20",    j2MV2c20, -1.);
    m_etree->SetBranchAndValue<int>("j2Flav",        m_physicsMeta.b2Flav, -1);
    m_etree->SetBranchAndValue<float>("jjPt",        HVecJetCorr.Pt()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("jjEta",       HVecJetCorr.Eta(), -10.);
    m_etree->SetBranchAndValue<float>("jjPhi",       HVecJetCorr.Phi(), -10.);
    m_etree->SetBranchAndValue<float>("jjM",         HVecJetCorr.M()/1e3, -1.);
    m_etree->SetBranchAndValue<float>("jjdR",        j1VecCorr.DeltaR(j2VecCorr), -1.);
    m_etree->SetBranchAndValue<float>("jjdPhi",      j1VecCorr.DeltaPhi(j2VecCorr), -10.);
    m_etree->SetBranchAndValue<float>("jjdEta",      fabs(j1VecCorr.Eta() - j2VecCorr.Eta()), -1.);
    m_etree->SetBranchAndValue<float>("WjjdPhi",     fabs(WVecT.DeltaPhi(HVecJetCorr)), -1.);
    m_etree->SetBranchAndValue<float>("ljmindPhi",   std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr))), -1.);
    m_etree->SetBranchAndValue<float>("WjjM",        VHVecJetCorr.M()/1e3, -1.);

    // 3-jet events
    if (nSelectedJet > 2){
      m_etree->SetBranchAndValue<float>("j3Pt",        j3VecCorr.Pt()/1e3, -1.);
      m_etree->SetBranchAndValue<float>("j3Eta",       j3VecCorr.Eta(), -10.);
      m_etree->SetBranchAndValue<float>("j3Phi",       j3VecCorr.Phi(), -10.);
      m_etree->SetBranchAndValue<float>("j3M",         j3VecCorr.M()/1e3, -1.);
      m_etree->SetBranchAndValue<float>("jjjM",        bbjVecCorr.M()/1e3, -1.);
    }

    // fat jets
    if (nFatJet >= 1){
      int J1nTags = -1;
      if (nFatJet > 0) J1nTags = Props::nBTagsAll.get(fatJets.at(0));
      m_etree->SetBranchAndValue<float>("J1Pt",        fj1VecCorr.Pt()/1e3, -1.);
      m_etree->SetBranchAndValue<float>("J1Eta",       fj1VecCorr.Eta(), -10.);
      m_etree->SetBranchAndValue<float>("J1Phi",       fj1VecCorr.Phi(), -10.);
      m_etree->SetBranchAndValue<float>("J1M",         fj1VecCorr.M()/1e3, -1.);
      m_etree->SetBranchAndValue<int>("J1nTags",       J1nTags, -1);
      m_etree->SetBranchAndValue<float>("WJdPhi",      fabs(WVecT.DeltaPhi(fj1VecCorr)), -1.);
      m_etree->SetBranchAndValue<float>("WJM",         VHVecFatCorr.M()/1e3, -1.);
    }

    if (doMJEtree){ // MJ study variables - save if told to in config file
      float ptCone30 = -5;
      float ptCone20 = -5;
      float topoCone20 = -5;
      if (nSigMu == 1){
        ptCone30 = Props::ptvarcone30.get(mu);
        topoCone20 = Props::topoetcone20.get(mu);
      }
      else if (nSigEl == 1){
        ptCone20 = Props::ptvarcone20.get(el);
        topoCone20 = Props::topoetcone20.get(el);
      }

      m_etree->SetBranchAndValue<float>("topoetcone20", topoCone20, -5);
      m_etree->SetBranchAndValue<float>("ptvarcone30", ptCone30, -5);
      m_etree->SetBranchAndValue<float>("ptvarcone20", ptCone20, -5);
      m_etree->SetBranchAndValue<float>("lmetdPhi", fabs(lepVec.DeltaPhi(metVec)), -99);
      m_etree->SetBranchAndValue<double>("jjmindPhi", mindPhi, -99);
      m_etree->SetBranchAndValue<float>("jmetdPhi", fabs(j1VecCorr.DeltaPhi(metVec)), -99);
      m_etree->SetBranchAndValue<float>("jldPhi", fabs(j1VecCorr.DeltaPhi(lepVec)), -99);
    }

    if (m_histNameSvc->get_isNominal()
        && (passAllCutsUpTo(eventFlagMerged, OneLeptonMergedCuts::pTV, { OneLeptonMergedCuts::pTV, OneLeptonMergedCuts::mbbCorr } )
        || passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::pTV, { OneLeptonResolvedCuts::pTV, OneLeptonResolvedCuts::mbbCorr } ))
//         && passSpecificCuts(eventFlagMerged, { OneLeptonMergedCuts::AtLeast1FatJet, OneLeptonMergedCuts::AtLeast2TrackJets } )
//         && (Props::nBTags.get(fatJets.at(0)) >= 1)
       ) {
      m_etree->Fill();
    }
  }


  //*********************************************************//
  //                    HISTOGRAMS                           //
  //*********************************************************//
  //test
  if(m_doQCD)m_histNameSvc->set_sample("multijet");
   

  if(m_doQCD)m_histNameSvc->set_sample("multijet");
   

  if(m_debug) std::cout << " >>>>> Histogram Formation" << std::endl;

  fill_1lepResolvedCutFlow(eventFlagResolved);
  fill_1lepMergedCutFlow(eventFlagMerged);

  if(tagcatExcl < 1 || nJet > 3){
    return EL::StatusCode::SUCCESS;
  }
  
  // fill cutflow histogram
  
  // print events for cutflow comparison
  if (m_isMC && printEventWeightsForCutFlow && passAllCutsUpTo(eventFlagResolved, OneLeptonResolvedCuts::Pt45) && m_histNameSvc->get_isNominal()) {
    printf("Run: %7u  ,  Event: %8llu  -  PU = %5.3f  ,  Lepton = %5.3f  ,  Trigger = %5.3f ,  BTag = %5.3f\n",
           m_eventInfo->runNumber(), m_eventInfo->eventNumber(), puWeight, leptonSF, triggerSF, btagWeight);
  }  // weight comparison for cutflow challenge

  if (!m_doBlinding && (m_histNameSvc->get_description() != "")) {

    if (doInputPlots) {
      if(passSpecificCuts(eventFlagResolved, {OneLeptonResolvedCuts::dRBB,OneLeptonResolvedCuts::mTW_add})){
	if (isResolved && doMbbRescaling) {
	  // m_histSvc->BookFillHist("mVH", 600, 0, 6000,   VHVecJetRW.M() / 1e3, m_weight);
	  m_histSvc->BookFillHist("mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
	  // Included Line // <<<---- SJ01 Revert Point
	  if(isE)m_histSvc->BookFillHist("El_mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
	  if(isMu)m_histSvc->BookFillHist("Mu_mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
	  //               // <<<---- SJ01 Revert Point
	}
	if (isResolved && !doMbbRescaling) {
	  //m_histSvc->BookFillHist("mVH", 600, 0, 6000,   VHVecJetCorr.M() / 1e3, m_weight);
	  m_histSvc->BookFillHist("mBB", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	  // Included Line // <<<---- SJ01 Revert Point
	  if(isE)m_histSvc->BookFillHist("El_mBB", 500, 0, 500,   HVecJetCorr.M() / 1e3, m_weight);
	  if(isMu)m_histSvc->BookFillHist("Mu_mBB", 500, 0, 500,   HVecJetCorr.M() / 1e3, m_weight);
	  //               // <<<---- SJ01 Revert Point
	}
	if (isMerged && doMbbRescaling) {
	  //m_histSvc->BookFillHist("mVH", 600, 0, 6000,   VHVecFatRW.M() / 1e3, m_weight);
	  m_histSvc->BookFillHist("mBB", 500, 0, 500,    fatJetRW.M() / 1e3, m_weight);
	}
	if (isMerged && !doMbbRescaling) {
	  //m_histSvc->BookFillHist("mVH", 600, 0, 6000,   VHVecFatCorr.M() / 1e3, m_weight);
	  m_histSvc->BookFillHist("mBB", 500, 0, 500,    fj1VecCorr.M() / 1e3, m_weight);
	}
      }
      if (isResolved && !doMbbRescaling) {
	m_histSvc->BookFillHist("Mu_mva",  //Name of variable in output histogram
				1000, // Number of bins
				-1,  // xmin
				1, // xmax
				m_tree->BDT,  // Value to fill histogram
				m_weight);   //Weight to fill histogram with
  m_histSvc->BookFillHist("mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
  m_histSvc->BookFillHist("pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
  m_histSvc->BookFillHist("mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);

  m_histSvc->BookFillHist("mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);


	if(isE){
	  m_histSvc->BookFillHist("El_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    m_histSvc->BookFillHist("El_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
    m_histSvc->BookFillHist("El_pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
    m_histSvc->BookFillHist("El_mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);
	  

	}	
	if(isMu){
	  m_histSvc->BookFillHist("Mu_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    m_histSvc->BookFillHist("Mu_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
    m_histSvc->BookFillHist("Mu_pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
    m_histSvc->BookFillHist("Mu_mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	  
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);
	  

	}
      }
      if (isResolved && doMbbRescaling) {
	m_histSvc->BookFillHist("Mu_mva", 1000, -1, 1,    m_tree->BDT, m_weight);
  m_histSvc->BookFillHist("mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
  m_histSvc->BookFillHist("pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
  m_histSvc->BookFillHist("mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	
  m_histSvc->BookFillHist("mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
  m_histSvc->BookFillHist("mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);
	

	if(isE){
	  m_histSvc->BookFillHist("El_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    m_histSvc->BookFillHist("El_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
    m_histSvc->BookFillHist("El_pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
    m_histSvc->BookFillHist("El_mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	  
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("El_mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);
	  
	}	
	if(isMu){
	  m_histSvc->BookFillHist("Mu_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    m_histSvc->BookFillHist("Mu_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
    m_histSvc->BookFillHist("Mu_pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
    m_histSvc->BookFillHist("Mu_mBBMvacut", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
	  
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_sherpa221", 1000, -1, 1,    m_tree->BDT_sherpa221, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_mBBcut_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_mBBcut", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_mBBcut, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_truthall", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_truthall, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221_systTTbar", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221_systTTbar, m_weight);
    m_histSvc->BookFillHist("Mu_mva_WZ_sherpa221", 1000, -1, 1,    m_tree->BDT_WZ_sherpa221, m_weight);

	}
      }
    }


    // if((m_eventInfo->eventNumber % 2 )==0){
    //   m_histSvc->BookFillHist("even_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    // }
    // else {
    //   m_histSvc->BookFillHist("odd_mva", 1000, -1, 1,     m_tree->BDT, m_weight);
    // }

    if (doBasicPlots) {

      // event quantities
      m_histSvc->BookFillHist("MET",     100,  0,  1000, m_tree->MET / 1e3, m_weight); 
      m_histSvc->BookFillHist("Mtop",     500,  0, 500, m_tree->Mtop / 1e3, m_weight);
      m_histSvc->BookFillHist("dYWH",     100,  0, 6, m_tree->dYWH, m_weight);
      m_histSvc->BookFillHist("dPhiLBmin", 100, 0., 6.,   std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr))), m_weight);
      m_histSvc->BookFillHist("dRBB",      100, 0., 6.,   j1VecCorr.DeltaR(j2VecCorr), m_weight);
      m_histSvc->BookFillHist("dPhiVBB",   100, 0., 3.15, fabs(WVecT.DeltaPhi(HVecJetCorr)), m_weight);
      if(nSelectedJet >= 3)m_histSvc->BookFillHist("mBBJ", 500, 0, 500, m_tree->mBBJ / 1e3, m_weight);
      

      //B-Tagging Variables for AntiKt4EMtopo dR = 0.4
      if (nSelectedJet >= 1){
	m_histSvc->BookFillHist("j1VecCorrPt",  600, 0, 3000,  m_tree->pTB1 / 1e3, m_weight);
	//m_histSvc->BookFillHist("MV2c10B1", 100, -1, 1, Props::MV2c10.get(selectedJets.at(0)), m_weight);
	//m_histSvc->BookFillHist("MV2c00B1", 100, -1, 1, Props::MV2c00.get(selectedJets.at(0)), m_weight);
	//m_histSvc->BookFillHist("MV2c100B1", 100, -1, 1, Props::MV2c100.get(selectedJets.at(0)), m_weight);
      }
      if (nSelectedJet >= 2){
	m_histSvc->BookFillHist("j2VecCorrPt",  600, 0, 3000,  m_tree->pTB2 / 1e3, m_weight);
	//m_histSvc->BookFillHist("MV2c10B2", 100, -1, 1, Props::MV2c10.get(selectedJets.at(1)), m_weight);
	//m_histSvc->BookFillHist("MV2c00B2", 100, -1, 1, Props::MV2c00.get(selectedJets.at(1)), m_weight);
	//m_histSvc->BookFillHist("MV2c100B2", 100, -1, 1, Props::MV2c100.get(selectedJets.at(1)), m_weight);
      }
      if (nSelectedJet >= 3){
	m_histSvc->BookFillHist("j3VecCorrPt",  600, 0, 3000,  m_tree->pTJ3 / 1e3, m_weight);
	//m_histSvc->BookFillHist("MV2c10B3", 100, -1, 1, Props::MV2c10.get(selectedJets.at(2)), m_weight);
	//m_histSvc->BookFillHist("MV2c00B2", 100, -1, 1, Props::MV2c00.get(selectedJets.at(1)), m_weight);
	//m_histSvc->BookFillHist("MV2c100B2", 100, -1, 1, Props::MV2c100.get(selectedJets.at(1)), m_weight);
      }
      
      //Lepton Vector quantities
      //fill_TLV("lepVec",      lepVec      , m_weight, isE, isMu);
      
      //Jet quantities
      if(isResolved && !doExtendedPlots ){
      	  //fill_TLV("j1VecCorr",   j1VecCorr   , m_weight, isE, isMu);
      	  //fill_TLV("j2VecCorr",   j2VecCorr   , m_weight, isE, isMu);
      	  //if (nSelectedJet >= 3) fill_TLV("j3VecCorr",   j3VecCorr   , m_weight, isE, isMu);
      }else if(isMerged && !doExtendedPlots){
      	//fill_TLV("HVecFatCorr",  fj1VecCorr  , m_weight, isE, isMu);
      	//if(associatedTrackJets.size() >= 1) fill_TLV("TrkJet1", associatedTrackJets.at(0)->p4(), m_weight, isE, isMu);
      	//if(associatedTrackJets.size() >= 2) fill_TLV("TrkJet2", associatedTrackJets.at(1)->p4(), m_weight, isE, isMu);
      }

      if(isMerged){
	//1D histogram of MV2c00, MV2c10 & MV2c100 for each trackjet
	if(associatedTrackJets.size() >= 1) {
	  m_histSvc->BookFillHist("TrkMV2c10B1", 100, -1, 1,  Props::MV2c10.get(associatedTrackJets.at(0)), m_weight);
	  m_histSvc->BookFillHist("TrkMV2c00B1", 100, -1, 1,  Props::MV2c00.get(associatedTrackJets.at(0)), m_weight);
	  m_histSvc->BookFillHist("TrkMV2c100B1", 100, -1, 1, Props::MV2c100.get(associatedTrackJets.at(0)), m_weight);
	  //2D histogram of MV2c00, MV2c10 & MV2c100 for each trackjet
	  m_histSvc->BookFillHist("TrkMV2c00B1-TrkMV2c100B1", 100, -1, 1, 100, -1, 1, Props::MV2c00.get(associatedTrackJets.at(0)), Props::MV2c100.get(associatedTrackJets.at(0)), m_weight);
	}
	if(associatedTrackJets.size() >= 2){
	  m_histSvc->BookFillHist("TrkMV2c10B2", 100, -1, 1,  Props::MV2c10.get(associatedTrackJets.at(1)), m_weight);
	  m_histSvc->BookFillHist("TrkMV2c00B2", 100, -1, 1,  Props::MV2c00.get(associatedTrackJets.at(1)), m_weight);
	  m_histSvc->BookFillHist("TrkMV2c100B2", 100, -1, 1, Props::MV2c100.get(associatedTrackJets.at(1)), m_weight);
	  //2D histogram of MV2c00, MV2c10 & MV2c100 for each trackjet
	  m_histSvc->BookFillHist("TrkMV2c00B2-TrkMV2c100B2", 100, -1, 1, 100, -1, 1, Props::MV2c00.get(associatedTrackJets.at(1)), Props::MV2c100.get(associatedTrackJets.at(1)), m_weight);
	}
      }

      // Included Line // <<<---- SJ01 Revert Point
      if(isE){
	m_histSvc->BookFillHist("El_MET",     100,  0,  1000, m_tree->MET / 1e3, m_weight); 
	m_histSvc->BookFillHist("El_Mtop",     500,  0, 500, m_tree->Mtop / 1e3, m_weight);
	m_histSvc->BookFillHist("El_dYWH",     100,  0, 6, m_tree->dYWH, m_weight);
        m_histSvc->BookFillHist("El_dRBB",      100, 0., 6.,   j1VecCorr.DeltaR(j2VecCorr), m_weight);
	m_histSvc->BookFillHist("El_dPhiLBmin", 100, 0., 6.,   std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr))), m_weight);
	m_histSvc->BookFillHist("El_dPhiVBB",   100, 0., 3.15, fabs(WVecT.DeltaPhi(HVecJetCorr)), m_weight);
	m_histSvc->BookFillHist("El_j1VecCorrPt",  600, 0, 3000,  m_tree->pTB1 / 1e3, m_weight);
	m_histSvc->BookFillHist("El_j2VecCorrPt",  600, 0, 3000,  m_tree->pTB2 / 1e3, m_weight);
	m_histSvc->BookFillHist("El_j3VecCorrPt",  600, 0, 3000,  m_tree->pTJ3 / 1e3, m_weight);
	if(nSelectedJet >= 3)m_histSvc->BookFillHist("El_mBBJ", 500, 0, 500, m_tree->mBBJ / 1e3, m_weight);
      }
      if(isMu){
	m_histSvc->BookFillHist("Mu_MET",     100,  0,  1000, m_tree->MET / 1e3, m_weight); 
	m_histSvc->BookFillHist("Mu_Mtop",     500,  0, 500, m_tree->Mtop / 1e3, m_weight);
	m_histSvc->BookFillHist("Mu_dYWH",     100,  0, 6, m_tree->dYWH, m_weight);
        m_histSvc->BookFillHist("Mu_dRBB",      100, 0., 6.,   j1VecCorr.DeltaR(j2VecCorr), m_weight);
	m_histSvc->BookFillHist("Mu_dPhiLBmin", 100, 0., 6.,   std::min(fabs(lepVec.DeltaPhi(j1VecCorr)), fabs(lepVec.DeltaPhi(j2VecCorr))), m_weight);
	m_histSvc->BookFillHist("Mu_dPhiVBB",   100, 0., 3.15, fabs(WVecT.DeltaPhi(HVecJetCorr)), m_weight);
	m_histSvc->BookFillHist("Mu_j1VecCorrPt",  600, 0, 3000,  m_tree->pTB1 / 1e3, m_weight);
	m_histSvc->BookFillHist("Mu_j2VecCorrPt",  600, 0, 3000,  m_tree->pTB2 / 1e3, m_weight);
	m_histSvc->BookFillHist("Mu_j3VecCorrPt",  600, 0, 3000,  m_tree->pTJ3 / 1e3, m_weight);

	if(nSelectedJet >= 3)m_histSvc->BookFillHist("Mu_mBBJ", 500, 0, 500, m_tree->mBBJ / 1e3, m_weight);
      }
      //               // <<<---- SJ01 Revert Point

    }

    if (doExtendedPlots) {

    if(isE){
      m_histSvc->BookFillHist("El_ptvarcone20_pT", 100,  0,  0.5, Props::ptvarcone20.get(el)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("El_ptvarcone20", 100,  0,  5000, Props::ptvarcone20.get(el), m_weight);
      m_histSvc->BookFillHist("El_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(el)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("El_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(el), m_weight);
    }
    if(isMu){
      m_histSvc->BookFillHist("Mu_ptvarcone30_pT", 100,  0,  0.5, Props::ptvarcone30.get(mu)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("Mu_ptvarcone30", 100,  0,  5000, Props::ptvarcone30.get(mu), m_weight);
      m_histSvc->BookFillHist("Mu_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(mu)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("Mu_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(mu), m_weight);
    }
    fill_TLV("lepVec",      lepVec      , m_weight, isE, isMu);
    
  //     //Plot the average number of  interactions per crossing for PU.
  //     m_histSvc->BookFillHist("AverMu", 61, -0.5, 60.5, Props::averageInteractionsPerCrossing.get(m_eventInfo), m_weight);
      
  //     // W system
       

  //     // dijet system (Higgs)
  //     if (isResolved) {
  //        // jets
  //       fill_TLV("j1VecCorr",   j1VecCorr   , m_weight, isE, isMu);
  //       fill_TLV("j2VecCorr",   j2VecCorr   , m_weight, isE, isMu);
  // 	if (nSelectedJet >= 1)fill_TLV("j3VecCorr",   j3VecCorr   , m_weight, isE, isMu);
  //       fill_TLV("HVecJetCorr",  HVecJetCorr , m_weight, isE, isMu);
  // 	if(isE)m_histSvc->BookFillHist("dPhiVBB_El",   100, 0., 3.15, fabs(WVecT.DeltaPhi(HVecJetCorr)), m_weight);
  // 	if(isMu)m_histSvc->BookFillHist("dPhiVBB_Mu",   100, 0., 3.15, fabs(WVecT.DeltaPhi(HVecJetCorr)), m_weight);

  //     }
  //     // fat jet (Higgs)
  //     if (isMerged) {
  //       //fill_TLV("HVecFatCorr",  fj1VecCorr  , m_weight, isE, isMu);
  //       m_histSvc->BookFillHist("dPhiVFatCorr", 100, 0., 3.15,   fabs(WVecT.DeltaPhi(fj1VecCorr)), m_weight);
  //     }

  //     // W system
  //     fill_TLV("nuVec",       nuVec       , m_weight, isE, isMu);
  //     fill_TLV("WVec",        WVec        , m_weight, isE, isMu);

  //     // jets
  //     // if (nSelectedJet >= 1) m_histSvc->BookFillHist("MV2c20B1", 100, -1, 1, Props::MV2c20.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2) m_histSvc->BookFillHist("MV2c20B2", 100, -1, 1, Props::MV2c20.get(selectedJets.at(1)), m_weight);
  //     // // Included Line // <<<---- SJ01 Revert Point
  //     // if (nSelectedJet >= 1) m_histSvc->BookFillHist("MV2c10B1", 100, -1, 1, Props::MV2c10.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2) m_histSvc->BookFillHist("MV2c10B2", 100, -1, 1, Props::MV2c10.get(selectedJets.at(1)), m_weight);
  //     // if (nSelectedJet >= 3) m_histSvc->BookFillHist("MV2c10B3", 100, -1, 1, Props::MV2c10.get(selectedJets.at(2)), m_weight);

  //     // if (nSelectedJet >= 1 && isE) m_histSvc->BookFillHist("MV2c20B1_El", 100, -1, 1, Props::MV2c20.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2 && isE) m_histSvc->BookFillHist("MV2c20B2_El", 100, -1, 1, Props::MV2c20.get(selectedJets.at(1)), m_weight);
  //     // if (nSelectedJet >= 1 && isE) m_histSvc->BookFillHist("MV2c10B1_El", 100, -1, 1, Props::MV2c10.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2 && isE) m_histSvc->BookFillHist("MV2c10B2_El", 100, -1, 1, Props::MV2c10.get(selectedJets.at(1)), m_weight);
  //     // if (nSelectedJet >= 3 && isE) m_histSvc->BookFillHist("MV2c10B3_El", 100, -1, 1, Props::MV2c10.get(selectedJets.at(2)), m_weight);

  //     //   if (nSelectedJet >= 1 && isMu) m_histSvc->BookFillHist("MV2c20B1_Mu", 100, -1, 1, Props::MV2c20.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2 && isMu) m_histSvc->BookFillHist("MV2c20B2_Mu", 100, -1, 1, Props::MV2c20.get(selectedJets.at(1)), m_weight);
  //     // if (nSelectedJet >= 1 && isMu) m_histSvc->BookFillHist("MV2c10B1_Mu", 100, -1, 1, Props::MV2c10.get(selectedJets.at(0)), m_weight);
  //     // if (nSelectedJet >= 2 && isMu) m_histSvc->BookFillHist("MV2c10B2_Mu", 100, -1, 1, Props::MV2c10.get(selectedJets.at(1)), m_weight);
  //     // if (nSelectedJet >= 3 && isMu) m_histSvc->BookFillHist("MV2c10B3_Mu", 100, -1, 1, Props::MV2c10.get(selectedJets.at(2)), m_weight);
  //     //               // <<<---- SJ01 Revert Point

  //     // dijet system (Higgs)
  //     m_histSvc->BookFillHist("dPhiBB",    100, 0., 3.15, fabs(j1VecCorr.DeltaPhi(j2VecCorr)), m_weight);
  //     m_histSvc->BookFillHist("dEtaBB",    100, 0., 6.,   fabs(j1VecCorr.Eta() - j2VecCorr.Eta()), m_weight);
  //     // Included Line // <<<---- SJ01 Revert Point
  //     if(isE){
  // 	m_histSvc->BookFillHist("El_ptvarcone20_pT", 100,  0,  0.5, Props::ptvarcone20.get(el)/lepVec.Pt(), m_weight);
  // 	m_histSvc->BookFillHist("El_ptvarcone20", 100,  0,  5000, Props::ptvarcone20.get(el), m_weight);
  // 	m_histSvc->BookFillHist("El_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(el)/lepVec.Pt(), m_weight);
  // 	m_histSvc->BookFillHist("El_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(el), m_weight); 
  // 	m_histSvc->BookFillHist("El_dPhiBB",    100, 0., 3.15, fabs(j1VecCorr.DeltaPhi(j2VecCorr)), m_weight);
  // 	m_histSvc->BookFillHist("El_dEtaBB",    100, 0., 6.,   fabs(j1VecCorr.Eta() - j2VecCorr.Eta()), m_weight);
  //     }
  //     if(isMu){
  // 	m_histSvc->BookFillHist("Mu_ptvarcone30_pT", 100,  0,  0.5, Props::ptvarcone30.get(mu)/lepVec.Pt(), m_weight);
  // 	m_histSvc->BookFillHist("Mu_ptvarcone30", 100,  0,  5000, Props::ptvarcone30.get(mu), m_weight);
  // 	m_histSvc->BookFillHist("Mu_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(mu)/lepVec.Pt(), m_weight);
  // 	m_histSvc->BookFillHist("Mu_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(mu), m_weight);  
  // 	m_histSvc->BookFillHist("Mu_dPhiBB",    100, 0., 3.15, fabs(j1VecCorr.DeltaPhi(j2VecCorr)), m_weight);
  // 	m_histSvc->BookFillHist("Mu_dEtaBB",    100, 0., 6.,   fabs(j1VecCorr.Eta() - j2VecCorr.Eta()), m_weight);
  //     }
  //     //               // <<<---- SJ01 Revert Point

  //     // fat jet (Higgs)
  //     m_histSvc->BookFillHist("dPhiLFatCorr", 100, 0., 3.15, fabs(lepVec.DeltaPhi(fj1VecCorr)), m_weight);
  //   }

  //   if (doTLVPlots) {

  //     fill_TLV("fwdjet1Vec", fwdjet1Vec, m_weight, isE, isMu);
  //     fill_TLV("fwdjet2Vec", fwdjet2Vec, m_weight, isE, isMu);
  //     fill_TLV("fwdjet3Vec", fwdjet3Vec, m_weight, isE, isMu);

  //     fill_TLV("sigjet1Vec", sigjet1Vec, m_weight, isE, isMu);
  //     fill_TLV("sigjet2Vec", sigjet2Vec, m_weight, isE, isMu);
  //     fill_TLV("sigjet3Vec", sigjet3Vec, m_weight, isE, isMu);

  //     fill_TLV("fatjet1Vec", fatjet1Vec, m_weight, isE, isMu);
  //     fill_TLV("fatjet2Vec", fatjet2Vec, m_weight, isE, isMu);
  //     fill_TLV("fatjet3Vec", fatjet3Vec, m_weight, isE, isMu);

  //     fill_TLV("j1VecSel",    j1VecSel    , m_weight, isE, isMu);
  //     fill_TLV("j2VecSel",    j2VecSel    , m_weight, isE, isMu);
  //     fill_TLV("j3VecSel",    j3VecSel    , m_weight, isE, isMu);
  //     fill_TLV("bbjVec",      bbjVec      , m_weight, isE, isMu);

  //     // fill_TLV("j1VecCorr",   j1VecCorr   , m_weight, isE, isMu);
  //     // fill_TLV("j2VecCorr",   j2VecCorr   , m_weight, isE, isMu);
  //     fill_TLV("j3VecCorr",   j3VecCorr   , m_weight, isE, isMu);
  //     fill_TLV("bbjVecCorr",  bbjVecCorr  , m_weight, isE, isMu);

  //     // fill_TLV("fj1VecCorr",  fj1VecCorr  , m_weight, isE, isMu);
  //     fill_TLV("fj2VecCorr",  fj2VecCorr  , m_weight, isE, isMu);
  //     fill_TLV("fj3VecCorr",  fj3VecCorr  , m_weight, isE, isMu);

  //     fill_TLV("metVec",      metVec      , m_weight, isE, isMu);
  //     fill_TLV("metVecJetRW", metVecJetRW , m_weight, isE, isMu);
  //     fill_TLV("metVecFJRW",  metVecFJRW  , m_weight, isE, isMu);

  //     fill_TLV("j1VecRW",     j1VecRW     , m_weight, isE, isMu);
  //     fill_TLV("j2VecRW",     j2VecRW     , m_weight, isE, isMu);
  //     fill_TLV("fatJetRW",    fatJetRW    , m_weight, isE, isMu);

  //     // fill_TLV("lepVec",      lepVec      , m_weight, isE, isMu);
  //     fill_TLV("lepVecT",     lepVecT     , m_weight, isE, isMu);
  //     fill_TLV("WVecT",       WVecT       , m_weight, isE, isMu);
  //     // fill_TLV("nuVec",       nuVec       , m_weight, isE, isMu);

  //     fill_TLV("HVecJet",      HVecJet     , m_weight, isE, isMu);
  //     // fill_TLV("HVecJetCorr",  HVecJetCorr , m_weight, isE, isMu);
  //     fill_TLV("HVecJetRW",    HVecJetRW   , m_weight, isE, isMu);

  //     fill_TLV("VHVecJet",     VHVecJet     , m_weight, isE, isMu);
  //     fill_TLV("VHVecJetCorr", VHVecJetCorr , m_weight, isE, isMu);
  //     fill_TLV("VHVecJetRW",   VHVecJetRW   , m_weight, isE, isMu);
  //     fill_TLV("VHVecFat",     VHVecFat     , m_weight, isE, isMu);
  //     fill_TLV("VHVecFatCorr", VHVecFatCorr , m_weight, isE, isMu);
  //     fill_TLV("VHVecFatRW",   VHVecFatRW   , m_weight, isE, isMu);

  //   }

    }

  if(passPreTagResolved && doPreTagPlots){
    m_histNameSvc->set_description("PreTag");
    m_histNameSvc->set_nTag(-1);	
    
    if (isResolved && doMbbRescaling) {
      m_histSvc->BookFillHist("mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
      if(isE){
  	m_histSvc->BookFillHist("El_mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
      }
      if(isMu){
  	m_histSvc->BookFillHist("Mu_mBB", 500, 0, 500,    HVecJetRW.M() / 1e3, m_weight);
      }
    }
    if (isResolved && !doMbbRescaling) {
      m_histSvc->BookFillHist("mBB", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
      if(isE){
  	m_histSvc->BookFillHist("El_mBB", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
      }
      if(isMu){
  	m_histSvc->BookFillHist("Mu_mBB", 500, 0, 500,    HVecJetCorr.M() / 1e3, m_weight);
      }
    }
    if (isMerged && doMbbRescaling) {
      m_histSvc->BookFillHist("mVH", 500, 0, 5000,   VHVecFatRW.M() / 1e3, m_weight);
      m_histSvc->BookFillHist("mBB", 500, 0, 500,    fatJetRW.M() / 1e3, m_weight);
    }
    if (isMerged && !doMbbRescaling) {
      m_histSvc->BookFillHist("mVH", 500, 0, 5000,   VHVecFatCorr.M() / 1e3, m_weight);
      m_histSvc->BookFillHist("mBB", 500, 0, 500,    fj1VecCorr.M() / 1e3, m_weight);
    }

    m_histSvc->BookFillHist("MET",     100,  0,  1000, m_tree->MET / 1e3, m_weight); // replace
    m_histSvc->BookFillHist("mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
    m_histSvc->BookFillHist("pTV",     200,  0, 2000, m_tree->pTV / 1e3, m_weight);
    if (nSelectedJet >= 1) m_histSvc->BookFillHist("MV2c10B1", 100, -1, 1, Props::MV2c10.get(selectedJets.at(0)), m_weight);
    if (nSelectedJet >= 2) m_histSvc->BookFillHist("MV2c10B2", 100, -1, 1, Props::MV2c10.get(selectedJets.at(1)), m_weight);
    fill_TLV("lepVec",      lepVec      , m_weight, isE, isMu);

    if(isResolved && !doExtendedPlots ){
      fill_TLV("j1VecSel",   j1VecSel   , m_weight, isE, isMu);
      fill_TLV("j2VecSel",   j2VecSel   , m_weight, isE, isMu);
    }
    if(isE){
      m_histSvc->BookFillHist("El_MET",     100,  0,  500, m_tree->MET / 1e3, m_weight); // replace
      m_histSvc->BookFillHist("El_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
      m_histSvc->BookFillHist("El_pTV",     200,  0,  500, m_tree->pTV / 1e3, m_weight);
      m_histSvc->BookFillHist("El_ptvarcone20_pT", 100,  0,  0.5, Props::ptvarcone20.get(el)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("El_ptvarcone20", 100,  0,  5000, Props::ptvarcone20.get(el), m_weight);
      m_histSvc->BookFillHist("El_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(el)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("El_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(el), m_weight);
    }
    if(isMu){
      m_histSvc->BookFillHist("Mu_MET",     100,  0,  500, m_tree->MET / 1e3, m_weight); // replace
      m_histSvc->BookFillHist("Mu_mTW",     100,  0,  500, m_tree->mTW / 1e3, m_weight);
      m_histSvc->BookFillHist("Mu_pTV",     200,  0,  500, m_tree->pTV / 1e3, m_weight);
      m_histSvc->BookFillHist("Mu_ptvarcone30_pT", 100,  0,  0.5, Props::ptvarcone30.get(mu)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("Mu_ptvarcone30", 100,  0,  5000, Props::ptvarcone30.get(mu), m_weight);
      m_histSvc->BookFillHist("Mu_topoetcone20_pT", 100,  0,  0.5, Props::topoetcone20.get(mu)/lepVec.Pt(), m_weight);
      m_histSvc->BookFillHist("Mu_topoetcone20", 100,  0,  5000, Props::topoetcone20.get(mu), m_weight);
    }
    m_histNameSvc->set_nJet(-1);
    m_histSvc->BookFillHist("nJet",     20,  0,  20, nJet, m_weight); // replace	
    m_histSvc->BookFillHist("nSig",     20,  0,  20, nSignalJet, m_weight); // replace	
    m_histSvc->BookFillHist("nFor",     20,  0,  20, nForwardJet, m_weight); // replace	
  }
  }
  
  if(m_debug) std::cout << " >>>>> Formed Histogram Inputs" << std::endl;

  return EL::StatusCode::SUCCESS;
} // run_1Lep_analysis

std::vector<const xAOD::Jet*> AnalysisReader_VHbb1Lep::ApplyMuonFatJetOR(std::vector<const xAOD::Muon*> Muons, std::vector<const xAOD::Jet*> fatjets){

  if(m_debug) std::cout << " >>>>> ApplyMuonFatJetOR" << std::endl;
  if(m_debug) std::cout << "       FatJet : " << fatjets.size() << std::endl;
  if(m_debug) std::cout << "       Muon : " << Muons.size() << std::endl;
  //----------------------------
  // Large R Jet Higgs candidate
  //----------------------------
  //Check muon - largeRJet OR
  if (Muons.size() > 0 && fatjets.size() > 0) {
    //Return vector
    std::vector<const xAOD::Jet*> ValidFatJets;

    // require dR(jet,muon) > 1.2
    std::vector<const xAOD::Jet*>::iterator iter = fatjets.begin();
    std::vector<const xAOD::Jet*>::iterator iterEnd = fatjets.end();
    int counter = 0;
    for ( ; iter != iterEnd; iter++) {
      if(!(*iter)) continue;
      if(m_debug) std::cout << "    >> FatJet # " << ++counter << std::endl;

      //Loop over the muons
      bool MuFatJetOverlap = false;
      int mcounter = 0;
      for(const xAOD::Muon * muon : Muons){
	if(!muon) continue;
	if(m_debug) std::cout << "    >> Muon # " << ++mcounter << std::endl;
        TLorentzVector lepVec = muon->p4();
	if(m_debug) std::cout << "       Checking Muon-Fat Overlap "<< std::endl;
        if ( ((*iter)->p4()).DeltaR(lepVec) < 1.2) {
	  if(m_debug) std::cout << "    >> Mu-FatJet OR " << std::endl;
	  MuFatJetOverlap = true;
	  break;
	}
      }

      //Check fatjet overlap with all muons
      if(MuFatJetOverlap){
	if(m_debug) std::cout << "       Skipping fatjet as Mu-FatJet OR " << std::endl;
	//iter = fatjets->erase(iter);
	//if(m_debug) std::cout << "       Erased fatjet " << std::endl;
      }else{
	//if(m_debug) std::cout << "    >> Incrementing fatjet iter " << std::endl;
	//++iter;
	if(m_debug) std::cout << "    >> Adding fatjet " << std::endl;
	ValidFatJets.push_back((*iter));
      }
    }
    if(m_debug) std::cout << " >>>>> ApplyMuonFatJetOR finished" << std::endl;

    return ValidFatJets;
  }else{
    //Return the same as the input
    return fatjets;
  }

}//End of ApplyMuonFatJetOR()

//Setup the CorrsAndSysts object m_corrsAndSyst for the current event
EL::StatusCode AnalysisReader_VHbb1Lep::SetupCorrsAndSyst(std::string currentVar, bool isCR = false){

  //Determine from m_histNameSvc->getFullHistName() th sample {ttbar, V+b,...} and number of tags {0tag, 1tag,...}
  //std::string EventClass = m_histNameSvc->getFullHistName("mVH"); //Leave entry blank as we only want sample & tag categorisation
  std::string EventClass = m_histNameSvc->get_sample();
  CAS::SysVar VarUp = CAS::Up;
  CAS::SysBin Bin = CAS::Any;
  if( m_histNameSvc -> get_sample() == "ttbar" /*EventClass.find("TTbar") != std::string::npos*/ ){
    m_evtType          = m_corrsAndSysts->GetEventType("TTbar");
    //Determine the number of tags & additional tags
    if( isCR ){

      m_detailevtType    = m_corrsAndSysts->GetDetailEventType("TopCR");
      m_corrsAndSysts->GetSystFromName((TString)currentVar, m_Systematic, Bin, VarUp);

    }else if( !isCR ){

      m_detailevtType    = m_corrsAndSysts->GetDetailEventType("TopSR");
      m_corrsAndSysts->GetSystFromName((TString)currentVar, m_Systematic, Bin, VarUp);

    }else{
      Info("AnalysisReader_VHbb1Lep::SetupCorrsAndSysts()", "Invalid ttbar CR definition for TTbar systematic");
      return EL::StatusCode::FAILURE;
    }

  }else if( EventClass == "W" || EventClass == "Z" ){

    //Set event type, systematic name and void detaileventType.
    //SR vs CR determined by mBB
    int flavour = m_physicsMeta.b1Flav + m_physicsMeta.b2Flav;
    if( flavour == 10 || flavour == 9 || flavour == 4 ) EventClass += "b";
    else if( flavour == 8 || flavour == 3)              EventClass += "c";
    else                                                EventClass += "l";
    m_evtType          = m_corrsAndSysts->GetEventType(EventClass);
    m_detailevtType    = CAS::NODETAILNAME;
    m_corrsAndSysts->GetSystFromName((TString)currentVar, m_Systematic, Bin, VarUp);

  }else{
    Info("AnalysisReader_VHbb1Lep::SetupCorrsAndSysts()", "Invalid sample, no systematics applied.");
    return EL::StatusCode::FAILURE;
  }

  return EL::StatusCode::SUCCESS;

}//End of SetupCorrsAndSysts()

// mVH is in GeV
void AnalysisReader_VHbb1Lep::fillVjetsSystslep(double Mbb, float mVH) {

  //Scale factor (weight)
  float weight_MG5_She = 0.1;
  if(m_debug) std::cout << "<fillVjetsSystslep()>    Adding MODEL___VJets systematics" << std::endl;

  //Load the correct systematic
  if( m_analysisType == "0lep" ){
    SetupCorrsAndSyst("SysVJets_0lep_shape_mVH");
    weight_MG5_She = m_corrsAndSysts->Get_SystematicWeight(m_evtType, mVH, Mbb, 0, 0, 0, 0, 0, 0, 0, 0, m_detailevtType, m_Systematic, CAS::Up, CAS::Any);
  }else if( m_analysisType == "1lep" ){
    SetupCorrsAndSyst("SysVJets_1lep_shape_mVH");
    weight_MG5_She = m_corrsAndSysts->Get_SystematicWeight(m_evtType, mVH, Mbb, 0, 0, 0, 0, 0, 0, 0, 0, m_detailevtType, m_Systematic, CAS::Up, CAS::Any);
  }else if( m_analysisType == "2lep" ){
    SetupCorrsAndSyst("SysVJets_2lep_shape_mVH");
    weight_MG5_She = m_corrsAndSysts->Get_SystematicWeight(m_evtType, mVH, Mbb, 0, 0, 0, 0, 0, 0, 0, 0, m_detailevtType, m_Systematic, CAS::Up, CAS::Any);
  }

  //Get_Systematics() return scale+1
  weight_MG5_She = weight_MG5_She - 1;
  //if(m_debug) std::cout << "<fillVjetsSystslep()>    weight_M5G5_She : " << weight_MG5_She << std::endl;
  float min = 0.1;
  if (weight_MG5_She < min) weight_MG5_She = min;
  m_weightSysts.push_back({ "MODEL_Vjets_MadGraph__1up", weight_MG5_She });
}

//mVH in GeV
void AnalysisReader_VHbb1Lep::fillTTbarSystslep(int nTag, bool isCR, float mVH) {

  std::vector<std::string> variations;
  variations.push_back("Pow_aMC");
  variations.push_back("Her_Pyt");
  variations.push_back("rHi_rLo");

  //Determine from m_histNameSvc->getFullHistName() th sample {ttbar, V+b,...} and number of tags {0tag, 1tag,...}
  std::string EventClass = m_histNameSvc->getFullHistName(""); //Leave entry blank as we only want sample & tag categorisation
  std::string channel;
  if( m_analysisType == "0lep" ) { channel = "_0lep";}
  else if( m_analysisType == "1lep" ) { channel = "_1lep"; }
  else if( m_analysisType == "2lep" ) { channel = "_2lep"; }

  float weight = 0;
  //Scale factor (weight)
  //Load the correct systematic
  for(std::string shapeComp : variations)
    {
      SetupCorrsAndSyst("SysTTbar_shape_mVH_"+shapeComp+channel, isCR);
      weight = m_corrsAndSysts->Get_SystematicWeight(m_evtType, mVH, 0, 0, 0, 0, 0, 0, 0, 0, nTag, m_detailevtType, m_Systematic, CAS::Up, CAS::Any) - 1; //Subract 1 to account for relative ratio
      float min = 0.1;
      // cut rad in half
      //    if ( shapeComp == "rHi_rLo" ) weight = 0.5 * (weight - 1) + 1;
      if (weight < min) weight = min;

      if ( shapeComp == "Pow_aMC" ) { m_weightSysts.push_back({ "MODEL_TTbar_aMcAtNlo__1up", weight }); }
      else if (shapeComp == "Her_Pyt" ) { m_weightSysts.push_back({ "MODEL_TTbar_Herwig__1up", weight }); }
      else if ( shapeComp == "rHi_rLo" ) {
	m_weightSysts.push_back({ "MODEL_TTbar_rad__1up", (1+((weight-1)/(weight+1))) });
	m_weightSysts.push_back({ "MODEL_TTbar_rad__1down", (1-((weight-1)/(weight+1))) });
      }
    }
}// End of fillTTbarSystslep

EL::StatusCode AnalysisReader_VHbb1Lep::fill_1lepResolvedCutFlow (unsigned long int eventFlag) {
  // systematics variations are skipped via histNameSvc
  std::string dir = "CutFlow/Nominal/";

  static std::string cuts[24] = { "AllCxAOD", "Trigger", "LooseLeptons", "SignalLeptons", "AtLeast2Jets", 
                                  "AtLeast2SigJets", "Pt45", "MET", "mTW", "Veto3bjet", "pTV",
                                  "SR_0tag_2jet","SR_0tag_3jet", "SR_0tag_4pjet", "SR_1tag_2jet",
                                  "SR_1tag_3jet", "SR_1tag_4pjet", "SR_2tag_2jet", "SR_2tag_3jet", 
                                  "SR_2tag_4pjet", };

  // Cuts to exclude from cutflow
  std::vector<unsigned int> excludeCuts = { } ; //OneLeptonResolvedCuts::tauVeto};

  // Loop over cuts
  for (unsigned int i = OneLeptonResolvedCuts::AllCxAOD; i <= OneLeptonResolvedCuts::SR_2tag_4pjet; ++i) {
    // Skip excluded cuts
    if ( std::find(excludeCuts.begin(), excludeCuts.end(), i) != excludeCuts.end() ) continue;

    // all cuts up to Pt45 should be applied sequentially
    if ( i <= OneLeptonResolvedCuts::pTV ) {
      if ( !passAllCutsUpTo(eventFlag, i, { } ) ) continue;
    } else {
      if ( !(passAllCutsUpTo(eventFlag, OneLeptonResolvedCuts::pTV, {}) && passSpecificCuts(eventFlag, {i})) ) continue;
    }
    std::string label = cuts[i];
    m_histSvc->BookFillCutHist(dir + "CutsResolved", length(cuts), cuts, label, m_weight);
    // Removed Line // <<<---- SJ01 Revert Point
    m_histSvc->BookFillCutHist(dir + "CutsResolvedNoWeight", length(cuts), cuts, label, 1);
    //if(isE){
    //  m_histSvc->BookFillCutHist(dir + "CutsResolved_el", length(cuts), cuts, label, m_weight);
    //  m_histSvc->BookFillCutHist(dir + "CutsResolvedNoWeight_el", length(cuts), cuts, label, 1);
    //}
    //if(isMu){
    //  m_histSvc->BookFillCutHist(dir + "CutsResolved_mu", length(cuts), cuts, label, m_weight);
    //  m_histSvc->BookFillCutHist(dir + "CutsResolvedNoWeight_mu", length(cuts), cuts, label, 1);
    //}
    //              // <<<---- SJ01 Revert Point
  }

  return EL::StatusCode::SUCCESS;
} // fill_1lepResolvedCutFlow

EL::StatusCode AnalysisReader_VHbb1Lep::fill_1lepMergedCutFlow (unsigned long int eventFlag) {
  // systematics variations are skipped via histNameSvc
  std::string dir = "CutFlow/Nominal/";

  static std::string cuts [12] = { "AllCxAOD", "Trigger", "Leptons", "MET", "AtLeast1FatJet",
                                   "mbbCorr", "AtLeast2TrackJets", "pTV",
                                   "SR_0tag_1pfat0pjets", "SR_1tag_1pfat0pjets",
                                   "SR_2tag_1pfat0pjets", "SR_3ptag_1pfat0pjets", };

  // Cuts to exclude from cutflow
  std::vector<unsigned int> excludeCuts = { } ;

  // Loop over cuts
  for (unsigned int i = OneLeptonMergedCuts::AllCxAOD; i <= OneLeptonMergedCuts::SR_3ptag_1pfat0pjets; ++i) {
    // Skip excluded cuts
    if ( std::find(excludeCuts.begin(), excludeCuts.end(), i) != excludeCuts.end() ) continue;

    if ( i <= OneLeptonMergedCuts::pTV ) {
      if ( !passAllCutsUpTo(eventFlag, i, { } ) ) continue;
    } else {
      if ( !(passAllCutsUpTo(eventFlag, OneLeptonMergedCuts::pTV, {}) && passSpecificCuts(eventFlag, {i})) ) continue;
    }
    std::string label = cuts[i];
    m_histSvc->BookFillCutHist(dir + "CutsMerged", length(cuts), cuts, label, m_weight);
  }

  return EL::StatusCode::SUCCESS;
} // fill_1lepMergedCutFlow

bool AnalysisReader_VHbb1Lep::pass1LepTrigger(double& triggerSF_nominal, ResultVHbb1lep selectionResult) {
  const xAOD::Electron  *el = selectionResult.el;
  const xAOD::Muon      *mu = selectionResult.mu;
  const xAOD::MissingET *met = selectionResult.met;

  if (!m_triggerTool->getTriggerDecision(m_eventInfo, triggerSF_nominal, el, 0, mu, 0, met, 0, m_randomRunNumber, "Nominal")) return false;

  if (m_isMC) m_weight *= triggerSF_nominal;

  // handle systematics
  if (m_isMC && (m_currentVar == "Nominal")) {
    for (size_t i = 0; i < m_triggerSystList.size(); i++) {
      // not computing useless systematics
      if (el && (m_triggerSystList.at(i).find("MUON_EFF_Trig") != std::string::npos)) continue;
      if (mu && (m_triggerSystList.at(i).find("ELECTRON_EFF_Trig") != std::string::npos)) continue;

      // get decision + weight
      double triggerSF = 1.;
      
      if (!m_triggerTool->getTriggerDecision(m_eventInfo, triggerSF, el, 0, mu, 0, met, 0, m_randomRunNumber, m_triggerSystList.at(i))) return EL::StatusCode::SUCCESS;
      //if(triggerSF_nominal==0)std::cout<<"El Mu "<<el<<" "<<mu<<std::endl;
      if (triggerSF_nominal > 0) m_weightSysts.push_back({ m_triggerSystList.at(i), (float)(triggerSF / triggerSF_nominal) });
      else Error("pass1LepTrigger()", "Nominal trigger SF=0!, The systematics will not be generated.");
    }
  }
  return true;
} // pass1LepTrigger

float  AnalysisReader_VHbb1Lep::calculateMtop(TLorentzVector &lepton, TLorentzVector &MET, TLorentzVector &b_jet1, TLorentzVector &b_jet2){

  //  bool NeuPz_im;
  float min_mtop,NeuPz_1,NeuPz_2;
  int METShift=1;
  TLorentzVector neu_tlv_1,neu_tlv_2;
  float mw = 80385;
  double tmp = mw*mw + 2.*lepton.Px()*MET.Px() + 2.*lepton.Py()*MET.Py();
  double METcorr = MET.Pt();
  if(tmp*tmp-pow(2.*lepton.Pt()*MET.Pt(),2)<0){
    //    NeuPz_im = true;
    if(METShift>0){
      METcorr = 0.5*mw*mw/(lepton.Pt()-lepton.Px()*cos(MET.Phi())-lepton.Py()*sin(MET.Phi()));
      double newtmp =mw*mw + 2.*lepton.Px()*METcorr*cos(MET.Phi()) + 2.*lepton.Py()*METcorr*sin(MET.Phi());
      NeuPz_1 = lepton.Pz()*newtmp/2./lepton.Pt()/lepton.Pt();
      NeuPz_2 = NeuPz_1;
    }
    else {
      NeuPz_1 = (lepton.Pz()*tmp)/2./lepton.Pt()/lepton.Pt();
      NeuPz_2 = NeuPz_1;
    }
  }
  else {
    //    NeuPz_im = false;
    NeuPz_1 = (lepton.Pz()*tmp + lepton.E()*sqrt(tmp*tmp - pow(2.*lepton.Pt()*MET.Pt(),2)))/2./lepton.Pt()/lepton.Pt();
    NeuPz_2 = (lepton.Pz()*tmp - lepton.E()*sqrt(tmp*tmp - pow(2.*lepton.Pt()*MET.Pt(),2)))/2./lepton.Pt()/lepton.Pt();
  }

  neu_tlv_1.SetPxPyPzE(METcorr*cos(MET.Phi()),METcorr*sin(MET.Phi()),NeuPz_1,sqrt(pow(METcorr,2) + pow(NeuPz_1,2)));
  neu_tlv_2.SetPxPyPzE(METcorr*cos(MET.Phi()),METcorr*sin(MET.Phi()),NeuPz_2,sqrt(pow(METcorr,2) + pow(NeuPz_2,2)));

  float mtop11 = (b_jet1 + lepton + neu_tlv_1).M();
  float mtop12 = (b_jet1 + lepton + neu_tlv_2).M();
  float mtop21 = (b_jet2 + lepton + neu_tlv_1).M();
  float mtop22 = (b_jet2 + lepton + neu_tlv_2).M();
  min_mtop = mtop11;
  if(min_mtop > mtop12)
    min_mtop = mtop12;
  if(min_mtop > mtop21)
    min_mtop = mtop21;
  if(min_mtop > mtop22)
    min_mtop = mtop22;

  return min_mtop;

}

float  AnalysisReader_VHbb1Lep:: calculatedYWH(TLorentzVector &lepton, TLorentzVector &MET, TLorentzVector &b_jet1, TLorentzVector &b_jet2){

  float min_dBeta,NeuPz_1,NeuPz_2;
  float mw = 80385;
  TLorentzVector neu_tlv, Wz1, Wz2, Hz;
  double tmp = mw*mw + 2.*lepton.Px()*MET.Px() + 2.*lepton.Py()*MET.Py();

  if(tmp*tmp-pow(2.*lepton.Pt()*MET.Pt(),2)<0){
    NeuPz_1 = (lepton.Pz()*tmp)/2./lepton.Pt()/lepton.Pt();
    NeuPz_2 = NeuPz_1;
  }
  else {
    NeuPz_1 = (lepton.Pz()*tmp + lepton.E()*sqrt(tmp*tmp - pow(2.*lepton.Pt()*MET.Pt(),2)))/2./lepton.Pt()/lepton.Pt();
    NeuPz_2 = (lepton.Pz()*tmp - lepton.E()*sqrt(tmp*tmp - pow(2.*lepton.Pt()*MET.Pt(),2)))/2./lepton.Pt()/lepton.Pt();
  }

  Wz1.SetPxPyPzE(0, 0, lepton.Pz() + NeuPz_1, std::sqrt(mw*mw + std::pow(lepton.Pz() + NeuPz_1, 2)));
  Wz2.SetPxPyPzE(0, 0, lepton.Pz() + NeuPz_2, std::sqrt(mw*mw + std::pow(lepton.Pz() + NeuPz_2, 2)));
  Hz.SetPxPyPzE(0, 0, b_jet1.Pz() + b_jet2.Pz(), std::sqrt( (b_jet1 + b_jet2).M2() + std::pow(b_jet1.Pz() + b_jet2.Pz(), 2)));

  float dBeta1 = std::fabs(Wz1.Beta() - Hz.Beta());
  float dBeta2 = std::fabs(Wz1.Beta() - Hz.Beta());

  if(dBeta1 < dBeta2)
    neu_tlv.SetPxPyPzE(MET.Px(),MET.Py(),NeuPz_1,sqrt(pow(MET.Px(),2) + pow(MET.Py(),2) + pow(NeuPz_1,2)));
  else
    neu_tlv.SetPxPyPzE(MET.Px(),MET.Py(),NeuPz_2,sqrt(pow(MET.Px(),2) + pow(MET.Py(),2) + pow(NeuPz_2,2)));


  float dYWH = std::fabs( (lepton + neu_tlv).Rapidity() - (b_jet1 + b_jet2).Rapidity() );

  return dYWH;
}

float  AnalysisReader_VHbb1Lep:: get_sys_metstrmu(float mTW){

  return 0.513+0.00318*mTW;

}
