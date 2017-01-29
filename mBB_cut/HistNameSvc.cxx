#include "CxAODReader/HistNameSvc.h"

#include "TError.h"

HistNameSvc::HistNameSvc() {
  reset(true);
  m_name.reserve(300);
}

void HistNameSvc::reset(bool resetSample) {
  if (resetSample) {
    set_sample("None");
  }
  set_analysisType(AnalysisType::undefined);
  set_eventFlavour(-1, -1);
  set_nTag(-1);
  set_tagCategory(TagCategory::inclusive);
  set_nJet(-1);
  set_nFatJet(-1);
  set_pTV(-1);
  set_mBB(-1);
  set_mVH(-1);
  set_nBTagTrackJetUnmatched(-1);
  set_nTrackJetInFatJet(-1);
  set_description("SR");
  set_variation("Nominal");
}

std::string HistNameSvc::getFullHistName(const std::string& variable) {

  // TODO cache sub strings for speed up

  m_name.clear();

  // SET IF NOMINAL OR SYS
  if (!m_isNominal) {

    if(!m_doOneSysDir) {
      m_name += "Sys";
      m_name += m_variation;
    } else m_name += "Systematics";

    m_name += "/";
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // STANDARD NAMING
  // TAKEN FROM: https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/VHbbInputsAndStatAnalysis#Sample_naming_conventions
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // SAMPLE_XtagYjet_Zptv_DESCR_DIST
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // SAMPLE: WW, WZ, ZZ, WlvH125, qqZllH125, ggZllH125, qqZvvH125, ggZvvH125, ttbar, stopt, stops, stopWt, Wl, Wcl, Wcc, Wbl, Wbc, Wbb, Zl, Zcl, Zcc, Zbl, Zbc, Zbb, AZhllbbxxx, AZhlvbbxxx, AZhvvbbxxx, HVTllbbxxx, HVTlvbbxxx, HVTvvbbxxx
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Xtag: X = 0, 1, 2, 3 (Xp --> >= X)
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Yjet: Y = 0, 1, 2 (Yp --> >= Y), For boosted analyses: AfatBjet
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Zptv: depends on binning used
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // DESCR_DIST: (Open) list of foreseen names
  // SR for all signal-like regions
  // topcr Run1-like region in 2 lepton
  // topemucr Run1-like region in 2 lepton
  // topaddbjetcr for events with >=1 additional b-tagged track jet in (boosted) 0 lepton searches
  // noaddbjetsr for events with 0 additional b-tagged track jet in (boosted) 0 lepton searches
  // lowmBBcr for low mBB sideband
  // highmBBcr for high mBB sideband
  // mBBcr for low+high mBB sideband
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // SAMPLE NAME
  m_name += m_sample;
  // append event flavour (only used if it's w+jets or z+jets)
  if (m_useEventFlav) {
    appendEventFlavour(m_name);
  }

  // Xtag
  if (m_nTag < 0) m_name += "_0ptag";
  else if (m_nTag == 0) m_name += "_0tag";
  else if (m_nTag == 1) m_name += "_1tag";
  else if (m_nTag == 2) {
    if (m_tagCategory == TagCategory::LL) m_name += "_2lltag";
    if (m_tagCategory == TagCategory::MM) m_name += "_2mmtag";
    if (m_tagCategory == TagCategory::TT) m_name += "_2tttag";
    else m_name += "_2tag";
  }
  else m_name += "_3ptag";


  // Yjet
  // VBF ANALYSIS
  if(m_analysisType == AnalysisType::VBF){
     if (m_nJet == 0) m_name += "0jet";
     else if (m_nJet == 1) m_name += "1jet";
     else if (m_nJet == 2) m_name += "2jet";
     else if (m_nJet == 3) m_name += "3jet";
     else m_name += "4pjet";
  }
  else{ // ALL OTHER VHBB: SM, AZh, HVT
     if (m_nFatJet < 0) { // resolved analyses
    	if (m_nJet == -1) m_name += "0pjet";
    	else if (m_nJet == -2) m_name += "2pjet";
    	else if (m_nJet == -23 ) m_name += "23jet";
    	else if (m_nJet == 0) m_name += "0jet";
    	else if (m_nJet == 1) m_name += "1jet";
    	else if (m_nJet == 2) m_name += "2jet";
    	else if (m_nJet == 3) m_name += "3jet";
    	else if (m_nJet == 4) m_name += "4jet";
    	else m_name += "5pjet";
     } else { // boosted analyses
    	if (m_nFatJet == 0) m_name += "0fat";
    	else if (m_nFatJet >= 1) m_name += "1pfat";
    
    	if (m_nJet < 0) m_name += "0pjet";
    	else if (m_nJet == 0) m_name += "0jet";
    	else if (m_nJet == 1) m_name += "1jet";
    	else m_name += "2pjet";
     }
  }

  // mVH BINNING (only used in some cases for HVT)
  if ( m_mVH < 0 ) m_name += "";
  else if (m_analysisType == AnalysisType::VHres){
    if ( m_mVH < 500e3 ) m_name += "0_500mVH_";
    else m_name += "500mVH_";
  }
  
  // ptv BINNING
  if (m_pTV < 0) m_name += "_0ptv_"; //inclusive
  else{
    if (m_analysisType == AnalysisType::undefined){
      Error("HistNameSvc::getFullHistName()", "Analysis type is undefined! Don't know which pTV binning to use!");
    }
    
    if (m_analysisType == AnalysisType::MVA) {

      if (m_pTV < 150e3) m_name += "_0_150ptv_";
      else m_name += "_150ptv_"; 
    
    } else if ( m_analysisType == AnalysisType::CUT ) {
    
      if (m_pTV < 150e3) m_name += "_0_150ptv_";
      else if (m_pTV < 200e3) m_name += "_150_200ptv_";
      else m_name += "_200ptv_";
    
    } else if ( m_analysisType == AnalysisType::VHres ) {

      if (m_pTV < 500e3) m_name += "_0_500ptv_";
      else m_name += "_500ptv_";
      
    } else if ( m_analysisType == AnalysisType::VBF ) {
      if (m_pTV == 0) m_name += "_0BDT_";
      else if (m_pTV == 1) m_name += "_1BDT_";
      else if(m_pTV == 2) m_name += "_2BDT_";
    }
  }

  //mBB binning
  if (m_mBB < 0) m_name += "_0mbb_"; //inclusive
  else{
    if (m_analysisType == AnalysisType::undefined){
      Error("HistNameSvc::getFullHistName()", "Analysis type is undefined! Don't know which mBB binning to use!");
    }
    
    if (m_analysisType == AnalysisType::MVA) {

      if (m_mBB < 100e3) m_name += "_0_100mbb_";
      else if (m_mBB > 100e3 && m_mBB < 150e3) m_name += "100_150mbb_";
      else m_name += "_150mbb_"; 
    
    }
  }


  m_name += m_description;
  m_name += "_";

  // ADD BTAG, FOR BOOSTED ANALYSES
  if( m_nBTagTrackJetUnmatched < 0 ) m_name += "";
  else if( m_nBTagTrackJetUnmatched == 0 ) m_name += "noaddbjetsr_";
  else if ( m_nBTagTrackJetUnmatched >= 1 ) m_name += "topaddbjetcr_";

  //ADD number of track jets in fat jet
  if( m_nTrackJetInFatJet < 0 ) m_name += "";
  else if ( m_nTrackJetInFatJet == 1 ) m_name += "1subjet_";
  else m_name += "2psubjet_";

  m_name += variable;

  if (!m_isNominal) {
    m_name += "_Sys";
    m_name += m_variation;
  }

  return m_name;
}

std::string HistNameSvc::getEventFlavour(){
  std::string buff;
  appendEventFlavour(buff);
  return buff;
}

std::string HistNameSvc::getFullSample(){
  std::string name = m_sample;
  if (m_useEventFlav) {
    appendEventFlavour(name);
  }
  return name;
}

void HistNameSvc::appendEventFlavour(std::string& buff){
  
  if       (m_jet0flav < 0) return;
  if       (m_jet0flav == 5 && m_jet1flav < 0) buff += "b";
  else if  (m_jet0flav == 4 && m_jet1flav < 0) buff += "c";
  else if  (m_jet0flav == 5 && m_jet1flav == 5)  buff += "bb";
  else if ((m_jet0flav == 5 || m_jet1flav == 5)
        && (m_jet0flav == 4 || m_jet1flav == 4)) buff += "bc";
  else if  (m_jet0flav == 5 || m_jet1flav == 5)  buff += "bl";
  else if  (m_jet0flav == 4 && m_jet1flav == 4)  buff += "cc";
  else if  (m_jet0flav == 4 || m_jet1flav == 4)  buff += "cl";
  else buff += "l";

}

void HistNameSvc::set_sample(const std::string& sample) {
  //std::cout << "Setting sample name (mc): " << sample << std::endl;
  m_sample = sample;

  m_useEventFlav = (m_sample == "W" || m_sample == "Z" || m_sample == "MadWenu" || m_sample == "MadWmunu" || m_sample == "MadWtaunu"
                    || m_sample == "MadZee" || m_sample == "MadZmumu" || m_sample == "MadZtautau" || m_sample == "Zv22" || m_sample == "Wv22" );

}

void HistNameSvc::set_variation(const std::string& variation) {
  m_variation = variation;
  m_isNominal = (variation == "Nominal");
}

void HistNameSvc::set_isNominal() {
  set_variation("Nominal");
}

void HistNameSvc::set_doOneSysDir(bool doOneSysDir) {
  m_doOneSysDir = doOneSysDir;
}
