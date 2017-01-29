#ifndef HistNameSvc_h
#define HistNameSvc_h

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

class HistNameSvc {

public:

  enum class AnalysisType {
    undefined = 0,
    CUT,
    MVA,
    VBF,
    VHres
  };

  enum class TagCategory {
    inclusive = -1,
    LL,
    MM,
    TT
  };

protected:

  // buffer for the full output name
  std::string m_name;

  std::string m_sample;
  AnalysisType m_analysisType;
  bool m_useEventFlav;
  int m_jet0flav;
  int m_jet1flav;
  int m_nTag;
  TagCategory m_tagCategory;
  int m_nJet;
  int m_nFatJet;
  float m_pTV; // MeV
  float m_mBB; // MeV
  std::string m_description;
  std::string m_variation;
  bool m_isNominal;
  float m_mVH; // MeV
  int m_nBTagTrackJetUnmatched;
  int m_nTrackJetInFatJet;
  bool m_doOneSysDir;

  void appendEventFlavour(std::string& buff);

public:

  HistNameSvc();
  ~HistNameSvc() {};

  std::map<int, std::string> m_samples;

  virtual void reset(bool resetSample = false);

  virtual std::string getFullHistName(const std::string& variable);

  std::string getEventFlavour();
  std::string getFullSample();

  bool get_isNominal() { return m_isNominal; }

  virtual void set_sample(const std::string& sample);
  std::string get_sample(){ return m_sample;  }
  void set_analysisType(AnalysisType type) { m_analysisType = type; }
  AnalysisType get_analysisType() { return m_analysisType; }
  void set_eventFlavour(int jet0flav, int jet1flav) { m_jet0flav = jet0flav; m_jet1flav = jet1flav; }
  void get_eventFlavour(int& jet0flav, int& jet1flav) { jet0flav = m_jet0flav; jet1flav = m_jet1flav; }
  void set_nTag(int nTag) { m_nTag = nTag; }
  int  get_nTag() { return m_nTag; }
  void set_tagCategory(TagCategory category) { m_tagCategory = category; }
  void set_nJet(int nJet) { m_nJet = nJet; }
  int get_nJet() { return m_nJet; }
  void set_nFatJet(int nFatJet) { m_nFatJet = nFatJet; }
  void set_pTV(float pTV) { m_pTV = pTV; }
  float get_pTV() { return m_pTV; }
  void set_mBB(float mBB) { m_mBB = mBB; }
  float get_mBB() { return m_mBB; }
  void set_description(const std::string& description) { m_description = description; }
  void set_variation(const std::string& variation);
  void set_isNominal();
  std::string get_description() { return m_description; }
  void set_useEventFlav(bool useEventFlav) { m_useEventFlav = useEventFlav; }
  int get_useEventFlav() { return m_useEventFlav; }
  void set_nBTagTrackJetUnmatched(int n) { m_nBTagTrackJetUnmatched = n; }
  void set_nTrackJetInFatJet(int n) { m_nTrackJetInFatJet = n; }
  void set_mVH(float mVH) { m_mVH = mVH; }
  void set_doOneSysDir(bool doOneSysDir);

};

#endif
