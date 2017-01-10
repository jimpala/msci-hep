
#include "Config.h"
#include "PlotMaker.h"

#include <iostream>

// This macro make Data/MC comparison plots taking limit input files as input
// The macro assumes the following naming conventions:
// 
// Input file name convention:
//   LimitHistograms.<xx>bb.<Name>.<Z>TeV.<Group>.<version>.<Signal>[.trafoN].root
//     <xx> = vv, lv or ll
//     <Name> = LHCP2013
//     <Z> = 7 or 8
//     <Group> = Institute name
//     <version> = version number (e.g. v.1.0)
//     <Signal> = name of the signal histograms. e.g. WlvH125 for one lepton signal at mH=125
//     [trafoN] = optional: name of the file with BDT transformed
//
// Histogram name convention:
//   [sys/]<Sample>_<region>_<var>[_sys]
//     <Sample> = name of the sample. e.g. ttbar
//     <region> = name of the region. e.g. 2tag2jet
//     <var>    = name of the variable. e.g. mBB
//     [sys]    = name of the systematic variation
//
//
// The tool provide the following features:
//   - Plots: data/MC prefit plots
//   - Yield tables (.txt and .tex formats)
//   - Systematics tables (Normalization)
//   - Detailed systematic plot (one plot per nuisance parameter for chosen variables)
//   - Binned sensitivity calculation
//   - Beamer slides to be compiled in latex

void makePlots() {

  Config config;

  // should we overwrite existing plots or only do missing ones?
  config.setOverwrite(true);

  ////////////////
  /// General info
  config.setAnalysis( Config::OneLepton,  // Type = ZeroLepton, OneLepton or TwoLepton: Appears on the plots
		      "Internal",         // Status: Appear on the plots next to ATLAS label
		      "LHCP2013",         // Name = "LHCP2013": only useful for the above naming convention
		      "13",                // Center of mass energy: Appears on the plots
		      "13.18",             // Integrated luminosity: Appears on the plots
		      "125",              // Mass of the generated higgs boson: only useful for the above naming convention
		      "UCL",             // Name of your institute: only usefule for the above naming convention
		      "v2.1");            // Input file version: only useful for the above naming convention


  // Overwrite default analysis title from Config::getAnalysisTitleDefault()
  //config.setAnalysisTitle("#nu#nubb");

  /////////
  /// Input 

  // path to the input file: can be local or on eos
  //config.setInputPath("/unix/atlas3/abell/Batch+Local_MergeSamplesExecuteCode");
  config.setInputPath("/unix/atlasvhbb/jpyne/CxAODFramework_311016/FullSyst_nJET");
  // Since your input file name might not follow the conventions above,
  // it can be overwritten using the line below
  // Note: specify the name WITHOUT ".root" at the end
  config.setInputFile("output_BDT_v1");

  // If input histograms are stored in a subdirectory of the input file, specify below
  // config.setInputTDirectory("OneLepton");



  //////////
  /// Output

  // path to output plots
  config.setOutputPath("./plotsBDTv2/");

  // BDT transformations:
  // config.addBDTTransformation(""); // no transformation
  // config.addBDTTransformation("trafoF", // name to be reffered to later
  //  			      12,       // Algorithm: 12 is trafo F in Daniel's tool
  //  			      4.5, 4.5);  // Transformation parameters
  config.addBDTTransformation("trafoD",
          6,
          10, 10);


  // Note:
  // If you have previously transformed your input file:
  //   Let the input file with transformed BDTs be named the same as the original file 
  //   with the extention .<trafoName>.root instead of .root
  //   In this case just declare the transformation providing only a value for trafoName:
  // config.addBDTTransformation("trafo6");



  /////////////////////
  /// Blinding the data

  // Blinding using threshold on S/(S+B)
  // config.setThresholdBlinding(0.02);

  // We can also set explicit blinding on specific variable and regions
  // config.addWindowBlinding("mBB",               // variable name to blind: should match addVariable() below 
  // 			   "2tag2jet_vpt0_120", // region in which to blind: should match names in the input file
  // 			   "",                  // BDT transformation as declared above: only relevant for BDT outputs
  // 			   100000,              // Lower limit
  // 			   149999);             // Upper limit
  // config.addWindowBlinding("mBB", "2tag3jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2tag2jet_vpt120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2tag3jet_vpt120", "", 100000, 149999);

  // config.addWindowBlinding("mBB", "2lltag2jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2lltag3jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2lltag2jet_vpt120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2lltag3jet_vpt120", "", 100000, 149999);

  // config.addWindowBlinding("mBB", "2mmtag2jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2mmtag3jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2mmtag2jet_vpt120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2mmtag3jet_vpt120", "", 100000, 149999);

  // config.addWindowBlinding("mBB", "2tttag2jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2tttag3jet_vpt0_120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2tttag2jet_vpt120", "", 100000, 149999);
  // config.addWindowBlinding("mBB", "2tttag3jet_vpt120", "", 100000, 149999);

  // cut based blinding
  //   config.addWindowBlinding("mjj", "2lltag2jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag3jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag2jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag3jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag2jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag3jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag2jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag3jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag2jet_vpt200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2lltag3jet_vpt200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag2jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag3jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag2jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag3jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag2jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag3jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag2jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag3jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag2jet_vpt200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2mmtag3jet_vpt200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag2jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag3jet_vpt0_90", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag2jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag3jet_vpt90_120", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag2jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag3jet_vpt120_160", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag2jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag3jet_vpt160_200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag2jet_vpt200", "", 100000, 149999);
  //   config.addWindowBlinding("mjj", "2tttag3jet_vpt200", "", 100000, 149999);
  


  ///////////
  /// Samples
 
  // declare the samples
  config.addDataSample("data",       // name: as it appears in the histogram names
		       "Data",       // title: as it should appear in the legend 
		       1);           // color: this is marker(line+fill) color for data(MC) 
  config.addSignalSample("qqZllH125", "ZH 125", kRed);
  config.addSignalSample("ggZllH125", "ZH 125", kRed);
  config.addSignalSample("qqZvvH125", "ZH 125", kRed); 
  config.addSignalSample("ggZvvH125", "ZH 125", kRed); 
  config.addSignalSample("WlvH125", "WH 125", kRed + 1);
  config.addBackgroundSample("ttbar", "ttbar", kOrange);
  config.addBackgroundSample("stops", "s+t chan", kOrange - 1); // Two samples sharing the same title appear only once in the legend
  config.addBackgroundSample("stopt", "s+t chan", kOrange - 1);
  config.addBackgroundSample("stopWt", "Wt", kYellow - 7);
  config.addBackgroundSample("Wbb", "W+bb", kGreen + 3);
  config.addBackgroundSample("Wbc", "W+bb", kGreen + 3);
  config.addBackgroundSample("Wbl", "W+bl", kGreen + 2);
  config.addBackgroundSample("Wcl", "W+cl", kGreen - 6);
  config.addBackgroundSample("Wcc", "W+cc", kGreen + 1);
  config.addBackgroundSample("Wl", "W+l", kGreen - 9);
  config.addBackgroundSample("Zbl", "Z+bl", kAzure + 1);
  config.addBackgroundSample("Zbb", "Z+bb", kAzure + 2);
  config.addBackgroundSample("Zbc", "Z+bc", kAzure + 2);
  config.addBackgroundSample("Zcl", "Z+cl", kAzure - 8);
  config.addBackgroundSample("Zcc", "Z+cc", kAzure - 4);
  config.addBackgroundSample("Zl", "Z+l", kAzure - 9);
  //  config.addBackgroundSample("multijetEl", "multijet", kPink + 1);
  //  config.addBackgroundSample("multijetMu", "multijet", kPink + 1);
  config.addBackgroundSample("WW", "WW", kGray + 3);
  config.addBackgroundSample("WZ", "WZ", kGray);
  config.addBackgroundSample("ZZ", "ZZ", kGray + 1);
 
  // One can assign scale factors per sample and per region by reading in from a text file.
  // To do so, create a text file myScaleFactors.txt with lines in the format:
  //     SampleName RegionRegExp scaleFactor
  // where SampleName should match the name specified above (first argument to addBackgroundSample() or addSignalSample())
  //       RegionRegExp is a regular expression that should match all regions where the SF is to be applied (see TRegexp in ROOT)
  // then, uncomment the line next to these comments.
  // Note: all scale factors are reset to 1 before reading the text file.
  //       in the text file one can specify SFs for a subset of the samples to be overwritten
  // config.readScaleFactors("myScaleFactors.txt");

  // Some pre-prepared set of scale factors are available and submitted to tag numbers
  // The complete set is available in the scaleFactors/ subdirectory of this package with the naming convention <tag>.txt
  
  std::string scaleFactorsTag = "SF_1L_nJ_0108";
  config.readScaleFactors("scaleFactors/"+scaleFactorsTag+".txt");
  config.setOutputPath(config.getOutputPath()+"/"+scaleFactorsTag);
  
  // To optimize plot readability , samples that contribute less than a fraction X are dropped from the legend
  config.setLegendThreshold(0.0);

  // To draw the histograms in no-stack mode
  //  config.setNoStack();

  // To just draw the signal histograms in no-stack mode
  //  config.setNoSignalStack();

  // set total background name
  // this is not mandatory but would save CPU and memory
  // Note: if any background scale factors are different than one, 
  //       this cannot be used and will be ignored
  // config.setTotalBackgroundName("bkg");



  ////////////
  // Variables

  // declare variables to plot
  /*
  config.addVariable(Config::BDTInput, // Type: BDTInput or BDTOutput.
  		     "dRBB",           // Name: as it appears in the histogram name  
  		     "#DeltaR(b,b)",   // Title: as it appears on x axis title
  		     0,                // XMin (default: -1)
  		     5.99,             // Xmax (default: -1) specify Xmin >= Xmax to keep original range
  		     4);               // NGroup (default: 1) to rebin x axis
  */ 
  config.addVariable(Config::BDTInput, "dPhiVBB", "#Delta#phi(V,bb)", 2.5, 3.15, 1);
  config.addVariable(Config::BDTInput, "dPhiLBmin", "#Delta#phi(lep,b)_{min}", 0, 3.15, 4);
  config.addVariable(Config::BDTInput, "pTV", "p_{T}(V) [GeV]", 0, 500, 4);
  config.addVariable(Config::BDTInput, "mBB", "m_{bb} [GeV]", 0, 300, 20);
  config.addVariable(Config::BDTInput, "pTB1", "p_{T}(b_{1}) [MeV]", 40000, 500000, 4);
  config.addVariable(Config::BDTInput, "pTB2", "p_{T}(b_{2}) [MeV]", 0, 250000, 4);
  config.addVariable(Config::BDTInput, "mTW", "m_{T}(W) [GeV]", 0, 250, 4);
  config.addVariable(Config::BDTInput, "MET", "MET [GeV]", 0, 400, 4);
  config.addVariable(Config::BDTInput, "MV1cB1", "MV1c(b_{1})", 0, 1, 1);
  config.addVariable(Config::BDTInput, "MV1cB2", "MV1c(b_{2})", 0, 1, 1);
  config.addVariable(Config::BDTInput, "pTB3", "p_{T}(b_{3}) [MeV]", 0, 250000, 4);
  config.addVariable(Config::BDTInput, "mBBB", "m(bbj) [MeV]", 0, 500000, 4);
  // Variables below of type BDTOutput will be plotted once per transformation
  
  config.addVariable(Config::BDTOutput, "mva_sherpa221_truthall_mBBcut_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_truthall_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_truthall_mBBcut", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_mBBcut_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_mBBcut", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_truthall", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_sherpa221", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_truthall_mBBcut_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_truthall_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_truthall_mBBcut", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_mBBcut_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_mBBcut", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_truthall", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221_systTTbar", "BDT signal", -1, 1, 40);
  config.addVariable(Config::BDTOutput, "mva_WZ_sherpa221", "BDT signal", -1, 1, 40);
  


  // this is an example for axis relabelling
  // config.addVariable(Config::BDTOutput, "mjj", "m(bb) [MeV]", 0, 500000, 4,
  //                    true,   // when this is true, you get uniform axes (equidistant bins). When false, bin size will varry.
  //                    true);  // when this is true, you get labels according to original unit. When false labels are from -1 to 1.

  // flags to make 1D/2D plots
  config.setMake1DPlots(true);
  config.setMake2DPlots(false);


  ///////////////
  /// Other setup

  // Other setup can be read from input file.
  // This includes:
  //  - Signal and Control regions
  //  - List of systematics
  // If you are using files with systematics all in one directory rather than separate directories use
  //  config.setSystematicDir("Systematics");
  //  config.readSetup();


  // Alternatively, Signal and Control regions can be set by hand, 
  // for example to only make plots for a subset of the regions.
  // This is also better to give proper titles for display on the plot
  config.clearRegions();
  // config.addRegion("2tag3jet_vpt120",                      // name: as it appears in histogram name
  // 		   "2 tags, 3 jets, p_{T}^{V} >= 120 GeV", // title: as it appears on the plots
  //  		   "2 tags");                              // slide title: used to decide which plots to appear on the same slide

  // config.addRegion("1tag3jet_vpt120", "1 tags, 3 jets, p_{T}^{V} >= 120 GeV", "1 tags");
  // config.addRegion("1tag2jet_vpt120", "1 tags, 2 jets, p_{T}^{V} >= 120 GeV", "1 tags");
  // config.addRegion("0tag3jet_vpt120", "0 tags, 3 jets, p_{T}^{V} >= 120 GeV", "0 tags");
  // config.addRegion("0tag2jet_vpt120", "0 tags, 2 jets, p_{T}^{V} >= 120 GeV", "0 tags");
  // config.addRegion("1tag3jet_vpt0_120", "1 tags, 3 jets, p_{T}^{V} < 120 GeV", "1 tags");
  // config.addRegion("1tag2jet_vpt0_120", "1 tags, 2 jets, p_{T}^{V} < 120 GeV", "1 tags");
  // config.addRegion("0tag3jet_vpt0_120", "0 tags, 3 jets, p_{T}^{V} < 120 GeV", "0 tags");
  // config.addRegion("0tag2jet_vpt0_120", "0 tags, 2 jets, p_{T}^{V} < 120 GeV", "0 tags");

  //  config.addRegion("2tag2jet_150ptv_SR", "2 LL tags, 3 jets, p_{T}^{V} < 120", "2 tags 3 jets low pTV");
  // config.addRegion("2mmtag3jet_vpt0_120", "2 MM tags, 3 jets, p_{T}^{V} < 120", "2 tags 3 jets low pTV");
  // config.addRegion("2tttag3jet_vpt0_120", "2 TT tags, 3 jets, p_{T}^{V} < 120", "2 tags 3 jets low pTV");
  
  // config.addRegion("2lltag3jet_vpt120", "2 LL tags, 3 jets, p_{T}^{V} >= 120", "2 tags 3 jets high pTV");
  // config.addRegion("2mmtag3jet_vpt120", "2 MM tags, 3 jets, p_{T}^{V} >= 120", "2 tags 3 jets high pTV");
  // config.addRegion("2tttag3jet_vpt120", "2 TT tags, 3 jets, p_{T}^{V} >= 120", "2 tags 3 jets high pTV");
  
  // config.addRegion("2lltag2jet_vpt0_120", "2 LL tags, 2 jets, p_{T}^{V} < 120", "2 tags 2 jets low pTV");
  // config.addRegion("2mmtag2jet_vpt0_120", "2 MM tags, 2 jets, p_{T}^{V} < 120", "2 tags 2 jets low pTV");
  // config.addRegion("2tttag2jet_vpt0_120", "2 TT tags, 2 jets, p_{T}^{V} < 120", "2 tags 2 jets low pTV");
  
  // config.addRegion("2lltag2jet_vpt120", "2 LL tags, 2 jets, p_{T}^{V} >= 120", "2 tags 2 jets high pTV");
  // config.addRegion("2mmtag2jet_vpt120", "2 MM tags, 2 jets, p_{T}^{V} >= 120", "2 tags 2 jets high pTV");
  // config.addRegion("2tttag2jet_vpt120", "2 TT tags, 2 jets, p_{T}^{V} >= 120", "2 tags 2 jets high pTV");


  // Merging regions
  // Regions can be merged, by following the example below.
  // In this example: the LL+MM+TT plot is being added to the list of regions 
  //   --> it doesn't matter if LL, MM, TT are also being plotted separately as specified above
  /*
  config.addRegion("2lltag2jet_vpt0_120", // name of the region in the input file
  		   "2tag2jet_vpt0_120",   // name of the region on the plots: regions sharing the same "plot" name will be merged 
  		   "2 tags, 2 jets, p_{T}^{V} < 120", // title: as it appears on the plots
  		   "2 tags 2 jets low pTV");          // slide title: used to decide which plots to appear on the same slide
  config.addRegion("2mmtag2jet_vpt0_120", "2tag2jet_vpt0_120", "2 tags, 2 jets, p_{T}^{V} < 120", "2 tags 2 jets low pTV");
  config.addRegion("2tttag2jet_vpt0_120", "2tag2jet_vpt0_120", "2 tags, 2 jets, p_{T}^{V} < 120", "2 tags 2 jets low pTV");
  */
  // cut based regions  
  config.addRegion(  "2tag2jet_150ptv_SR", "2tag2jet_150ptv_SR",   "2 tags, 2 jets, p_{T}^{V} >= 150 GeV", "2 tags, 2 jets");
  config.addRegion(  "2tag3jet_150ptv_SR", "2tag3jet_150ptv_SR",   "2 tags, 3 jets, p_{T}^{V} >= 150 GeV", "2 tags, 3 jets");
  // config.addRegion(  "0tag2jet_vpt200",   "0 tags, 2 jets, p_{T}^{V} >= 200 GeV", "0 tags, 2 jets");
  // config.addRegion(  "1tag3jet_vpt200",   "1 tags, 3 jets, p_{T}^{V} >= 200 GeV", "1 tags, 3 jets");
  // config.addRegion(  "1tag2jet_vpt200",   "1 tags, 2 jets, p_{T}^{V} >= 200 GeV", "1 tags, 2 jets");
  // config.addRegion("2lltag3jet_vpt200", "2 l tags, 3 jets, p_{T}^{V} >= 200 GeV", "2 l tags, 3 jets");
  // config.addRegion("2lltag2jet_vpt200", "2 l tags, 2 jets, p_{T}^{V} >= 200 GeV", "2 l tags, 2 jets");
  // config.addRegion("2mmtag3jet_vpt200", "2 m tags, 3 jets, p_{T}^{V} >= 200 GeV", "2 m tags, 3 jets");
  // config.addRegion("2mmtag2jet_vpt200", "2 m tags, 2 jets, p_{T}^{V} >= 200 GeV", "2 m tags, 2 jets");
  // config.addRegion("2tttag3jet_vpt200", "2 t tags, 3 jets, p_{T}^{V} >= 200 GeV", "2 t tags, 3 jets");
  // config.addRegion("2tttag2jet_vpt200", "2 t tags, 2 jets, p_{T}^{V} >= 200 GeV", "2 t tags, 2 jets");
  
  // config.addRegion(  "0tag3jet_vpt160_200",   "0 tags, 3 jets, 160 < p_{T}^{V} < 200 GeV", "0 tags, 3 jets");
  // config.addRegion(  "0tag2jet_vpt160_200",   "0 tags, 2 jets, 160 < p_{T}^{V} < 200 GeV", "0 tags, 2 jets");
  // config.addRegion(  "1tag3jet_vpt160_200",   "1 tags, 3 jets, 160 < p_{T}^{V} < 200 GeV", "1 tags, 3 jets");
  // config.addRegion(  "1tag2jet_vpt160_200",   "1 tags, 2 jets, 160 < p_{T}^{V} < 200 GeV", "1 tags, 2 jets");
  // config.addRegion("2lltag3jet_vpt160_200", "2 l tags, 3 jets, 160 < p_{T}^{V} < 200 GeV", "2 l tags, 3 jets");
  // config.addRegion("2lltag2jet_vpt160_200", "2 l tags, 2 jets, 160 < p_{T}^{V} < 200 GeV", "2 l tags, 2 jets");
  // config.addRegion("2mmtag3jet_vpt160_200", "2 m tags, 3 jets, 160 < p_{T}^{V} < 200 GeV", "2 m tags, 3 jets");
  // config.addRegion("2mmtag2jet_vpt160_200", "2 m tags, 2 jets, 160 < p_{T}^{V} < 200 GeV", "2 m tags, 2 jets");
  // config.addRegion("2tttag3jet_vpt160_200", "2 t tags, 3 jets, 160 < p_{T}^{V} < 200 GeV", "2 t tags, 3 jets");
  // config.addRegion("2tttag2jet_vpt160_200", "2 t tags, 2 jets, 160 < p_{T}^{V} < 200 GeV", "2 t tags, 2 jets");
  
  // config.addRegion(  "0tag3jet_vpt120_160",   "0 tags, 3 jets, 120 < p_{T}^{V} < 160 GeV", "0 tags, 3 jets");
  // config.addRegion(  "0tag2jet_vpt120_160",   "0 tags, 2 jets, 120 < p_{T}^{V} < 160 GeV", "0 tags, 2 jets");
  // config.addRegion(  "1tag3jet_vpt120_160",   "1 tags, 3 jets, 120 < p_{T}^{V} < 160 GeV", "1 tags, 3 jets");
  // config.addRegion(  "1tag2jet_vpt120_160",   "1 tags, 2 jets, 120 < p_{T}^{V} < 160 GeV", "1 tags, 2 jets");
  // config.addRegion("2lltag3jet_vpt120_160", "2 l tags, 3 jets, 120 < p_{T}^{V} < 160 GeV", "2 l tags, 3 jets");
  // config.addRegion("2lltag2jet_vpt120_160", "2 l tags, 2 jets, 120 < p_{T}^{V} < 160 GeV", "2 l tags, 2 jets");
  // config.addRegion("2mmtag3jet_vpt120_160", "2 m tags, 3 jets, 120 < p_{T}^{V} < 160 GeV", "2 m tags, 3 jets");
  // config.addRegion("2mmtag2jet_vpt120_160", "2 m tags, 2 jets, 120 < p_{T}^{V} < 160 GeV", "2 m tags, 2 jets");
  // config.addRegion("2tttag3jet_vpt120_160", "2 t tags, 3 jets, 120 < p_{T}^{V} < 160 GeV", "2 t tags, 3 jets");
  // config.addRegion("2tttag2jet_vpt120_160", "2 t tags, 2 jets, 120 < p_{T}^{V} < 160 GeV", "2 t tags, 2 jets");
  
  // config.addRegion(  "0tag3jet_vpt90_120",   "0 tags, 3 jets, 90 < p_{T}^{V} < 120 GeV", "0 tags, 3 jets");
  // config.addRegion(  "0tag2jet_vpt90_120",   "0 tags, 2 jets, 90 < p_{T}^{V} < 120 GeV", "0 tags, 2 jets");
  // config.addRegion(  "1tag3jet_vpt90_120",   "1 tags, 3 jets, 90 < p_{T}^{V} < 120 GeV", "1 tags, 3 jets");
  // config.addRegion(  "1tag2jet_vpt90_120",   "1 tags, 2 jets, 90 < p_{T}^{V} < 120 GeV", "1 tags, 2 jets");
  // config.addRegion("2lltag3jet_vpt90_120", "2 l tags, 3 jets, 90 < p_{T}^{V} < 120 GeV", "2 l tags, 3 jets");
  // config.addRegion("2lltag2jet_vpt90_120", "2 l tags, 2 jets, 90 < p_{T}^{V} < 120 GeV", "2 l tags, 2 jets");
  // config.addRegion("2mmtag3jet_vpt90_120", "2 m tags, 3 jets, 90 < p_{T}^{V} < 120 GeV", "2 m tags, 3 jets");
  // config.addRegion("2mmtag2jet_vpt90_120", "2 m tags, 2 jets, 90 < p_{T}^{V} < 120 GeV", "2 m tags, 2 jets");
  // config.addRegion("2tttag3jet_vpt90_120", "2 t tags, 3 jets, 90 < p_{T}^{V} < 120 GeV", "2 t tags, 3 jets");
  // config.addRegion("2tttag2jet_vpt90_120", "2 t tags, 2 jets, 90 < p_{T}^{V} < 120 GeV", "2 t tags, 2 jets");
  
  // config.addRegion(  "0tag3jet_vpt0_90",   "0 tags, 3 jets, 0 < p_{T}^{V} < 90 GeV", "0 tags, 3 jets");
  // config.addRegion(  "0tag2jet_vpt0_90",   "0 tags, 2 jets, 0 < p_{T}^{V} < 90 GeV", "0 tags, 2 jets");
  // config.addRegion(  "1tag3jet_vpt0_90",   "1 tags, 3 jets, 0 < p_{T}^{V} < 90 GeV", "1 tags, 3 jets");
  // config.addRegion(  "1tag2jet_vpt0_90",   "1 tags, 2 jets, 0 < p_{T}^{V} < 90 GeV", "1 tags, 2 jets");
  // config.addRegion("2lltag3jet_vpt0_90", "2 l tags, 3 jets, 0 < p_{T}^{V} < 90 GeV", "2 l tags, 3 jets");
  // config.addRegion("2lltag2jet_vpt0_90", "2 l tags, 2 jets, 0 < p_{T}^{V} < 90 GeV", "2 l tags, 2 jets");
  // config.addRegion("2mmtag3jet_vpt0_90", "2 m tags, 3 jets, 0 < p_{T}^{V} < 90 GeV", "2 m tags, 3 jets");
  // config.addRegion("2mmtag2jet_vpt0_90", "2 m tags, 2 jets, 0 < p_{T}^{V} < 90 GeV", "2 m tags, 2 jets");
  // config.addRegion("2tttag3jet_vpt0_90", "2 t tags, 3 jets, 0 < p_{T}^{V} < 90 GeV", "2 t tags, 3 jets");
  // config.addRegion("2tttag2jet_vpt0_90", "2 t tags, 2 jets, 0 < p_{T}^{V} < 90 GeV", "2 t tags, 2 jets");
  
  // One can also choose to run without systematics  
  config.clearSystematics();
  
  // or with a reduced set of systematics
  // Note: first clear the systematics as shown above
  // config.addSystematic("SysMETScaleSoftTerms", // name
  //  		       false);         // is one sided?
  // config.addSystematic("SysMETResoSoftTerms", false);
  // config.addSystematic("SysJetEResol", true);
  // config.addSystematic("SysBJetReso", true);
  // config.addSystematic("SysMJShape", false);
  // config.addSystematic("SysMETScaleSoftTerms", false);
  // config.addSystematic("SysWDPhi", false);
  // config.addSystematic("SysWPtV", false);
  // config.addSystematic("SysWbbMbbGS", false);
  // config.addSystematic("SysWMbb", false);
  // config.addSystematic("SysTopPt", false);
  // config.addSystematic("SysTtbarPtWCont", false);
  // config.addSystematic("SysTtbarMBBCont", false);
  // config.addSystematic("SysTtbarPtB1Cont", false);
  // config.addSystematic("SysTtbarMetCont", false);
  // config.addSystematic("SysWtChanAcerMC", false);
  // config.addSystematic("SysWtChanPythiaHerwig", false);
  // config.addSystematic("SysTChanPtB2", false);
  // config.addSystematic("SysVVJetScalePtST1", false);
  // config.addSystematic("SysVVJetScalePtST2", false);
  // config.addSystematic("SysVVMbb", false);
  
  ////////////////////////
  /// Detailed systematics
  
  // One can ask for detailed systematic plots for specific variables in specific region
  // This will make one plot per nuisance parameter
  // config.addDetailedSystematicsPlots("mva",               // name of the variable: as declared above
  // 				     "trafo6",            // name of the transformation: relevant for BDT outputs
  // 				     "2tag2jet_vpt0_120", // name of the region: as declared above
  // 				     "Systematics: ");    // slide title: used to decide which plots to appear on the same slide
  // config.addDetailedSystematicsPlots("mva", "trafo6", "2tag2jet_vpt120", "Systematics: ");
  // config.addDetailedSystematicsPlots("mBB", "", "2tag2jet_vpt0_120", "Systematics: ");
  // config.addDetailedSystematicsPlots("mBB", "", "2tag2jet_vpt120", "Systematics: ");
  // config.addDetailedSystematicsPlots("pTB1", "", "2tag2jet_vpt0_120", "Systematics: ");
  // config.addDetailedSystematicsPlots("pTB1", "", "2tag2jet_vpt120", "Systematics: ");
  // config.addDetailedSystematicsPlots("pTB2", "", "2tag2jet_vpt0_120", "Systematics: ");
  // config.addDetailedSystematicsPlots("pTB2", "", "2tag2jet_vpt120", "Systematics: ");
  // config.addDetailedSystematicsPlots("MET", "", "2tag2jet_vpt0_120", "Systematics: ");
  // config.addDetailedSystematicsPlots("MET", "", "2tag2jet_vpt120", "Systematics: ");
  
  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2lltag2jet_vpt120", "Systematics (high pTV): ");
  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2mmtag2jet_vpt120", "Systematics (high pTV): ");
  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2tttag2jet_vpt120", "Systematics (high pTV): ");

  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2lltag2jet_vpt0_120", "Systematics (low pTV): ");
  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2mmtag2jet_vpt0_120", "Systematics (low pTV): ");
  // config.addDetailedSystematicsPlots("mvaWlvH125", "trafo6", "2tttag2jet_vpt0_120", "Systematics (low pTV): ");
  
  // config.addDetailedSystematicsPlots("mBB", "", "2lltag2jet_vpt120", "Systematics (high pTV): ");
  // config.addDetailedSystematicsPlots("mBB", "", "2mmtag2jet_vpt120", "Systematics (high pTV): ");
  // config.addDetailedSystematicsPlots("mBB", "", "2tttag2jet_vpt120", "Systematics (high pTV): ");

  // config.addDetailedSystematicsPlots("mBB", "", "2lltag2jet_vpt0_120", "Systematics (low pTV): ");
  // config.addDetailedSystematicsPlots("mBB", "", "2mmtag2jet_vpt0_120", "Systematics (low pTV): ");
  // config.addDetailedSystematicsPlots("mBB", "", "2tttag2jet_vpt0_120", "Systematics (low pTV): ");

  // config.addDetailedSystematicsPlots("mva", "trafoF", "1tag2jet_vpt0_120",   "Systematics 2J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2lltag2jet_vpt0_120", "Systematics 2J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2mmtag2jet_vpt0_120", "Systematics 2J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2tttag2jet_vpt0_120", "Systematics 2J (low pTV): ");

  // config.addDetailedSystematicsPlots("mva", "trafoF", "1tag2jet_vpt120",   "Systematics 2J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2lltag2jet_vpt120", "Systematics 2J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2mmtag2jet_vpt120", "Systematics 2J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2tttag2jet_vpt120", "Systematics 2J (high pTV): ");

  // config.addDetailedSystematicsPlots("mva", "trafoF", "1tag3jet_vpt0_120",   "Systematics 3J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2lltag3jet_vpt0_120", "Systematics 3J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2mmtag3jet_vpt0_120", "Systematics 3J (low pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2tttag3jet_vpt0_120", "Systematics 3J (low pTV): ");

  // config.addDetailedSystematicsPlots("mva", "trafoF", "1tag3jet_vpt120",   "Systematics 3J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2lltag3jet_vpt120", "Systematics 3J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2mmtag3jet_vpt120", "Systematics 3J (high pTV): ");
  // config.addDetailedSystematicsPlots("mva", "trafoF", "2tttag3jet_vpt120", "Systematics 3J (high pTV): ");
  
  // for(int iCR=0; iCR<config.getNRegions(); iCR++) {
  //   Config::Region region = config.getRegion(iCR);
  //   for(int iVar=0; iVar<config.getNVariables(); iVar++) {
  //     Config::Variable var = config.getVariable(iVar);
  //     std::string title = "Systematics: "+var.name+" ";
  //     if(region.name.find("0tag") != std::string::npos ) {
  // 	title += "0 tag";
  //     }else if(region.name.find("1tag") != std::string::npos) {
  // 	title += "1 tag";
  //     }else{
  // 	title += "2 tag";
  // 	if(region.name.find("2jet") != std::string::npos) {
  // 	  title += " , 2 jets";
  // 	}else{
  // 	  title += " , 3 jets";
  // 	}
  // 	if(region.name.find("vpt120") != std::string::npos) {
  // 	  title += " (high pTV)";
  // 	}else {
  // 	  title += " (low pTV)";
  // 	}
  //     }
  //     config.addDetailedSystematicsPlots(var.name, 
  // 					 "", //var.type == Config::BDTOutput ? "trafo6" : "", 
  // 					 region.name,
  // 					 title);
  //   }
  // }
					 


   /////////////////////////
   /// Computing sensitivity
   
   // One can output expected sensitivity numbers based on some specific plots and regions
   // The sensitivity metric used is one of two:
   //  - Normal approximation:
   //    S/sqrt(B + dB*dB)
   //  - Log likelihood ratio:
   //    sqrt(2*((S+B)*lob(1+S/B)-S))
   // It is computed for each bin and then added in quadrature
   // Several histograms from different regions can be combined (one variable per region)
   // NOTE: the concerned variables and regions and transformations 
   //       need to be added for ploting above.
   
   // define the regions to combine
   //   config.addSensitivityPlot( "mva",             // name to uniquely identify a sensitivity calculation
   // 			     "mva",             // variable name used for this region
   // 			     "2tag2jet_vpt120", // name of the region
   // 			     "trafo6");         // name of the transformation in case of BDT
   //   config.addSensitivityPlot( "mva", "mva", "2tag3jet_vpt120", "trafo6");
   //   config.addSensitivityPlot( "mva", "mva", "2tag2jet_vpt0_120", "trafo6");
   //   config.addSensitivityPlot( "mva", "mva", "2tag3jet_vpt0_120", "trafo6");
   
   //   // you can combine the same plots in different ways
   //   config.addSensitivityPlot( "mBB", "mBB", "2tag2jet_150ptv_SR", "");
   //   config.addSensitivityPlot( "mva 2t2j", "mva", "2tag2jet_vpt0_120", "trafo6");
   //   config.addSensitivityPlot( "mva 2t3j", "mva", "2tag3jet_vpt120", "trafo6");
   //   config.addSensitivityPlot( "mva 2t3j", "mva", "2tag3jet_vpt0_120", "trafo6");
   
   //   config.addSensitivityPlot( "mva high ptv", "mva", "2tag2jet_vpt120", "trafo6");
   //   config.addSensitivityPlot( "mva high ptv", "mva", "2tag3jet_vpt120", "trafo6");
   //   config.addSensitivityPlot( "mva low ptv", "mva", "2tag2jet_vpt0_120", "trafo6");
   //   config.addSensitivityPlot( "mva low ptv", "mva", "2tag3jet_vpt0_120", "trafo6");
   
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag3jet_vpt200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag2jet_vpt200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag3jet_vpt200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag2jet_vpt200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag3jet_vpt200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag2jet_vpt200", "");
   
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag3jet_vpt160_200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag2jet_vpt160_200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag3jet_vpt160_200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag2jet_vpt160_200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag3jet_vpt160_200", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag2jet_vpt160_200", "");
   
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag3jet_vpt120_160", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag2jet_vpt120_160", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag3jet_vpt120_160", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag2jet_vpt120_160", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag3jet_vpt120_160", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag2jet_vpt120_160", "");
   
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag3jet_vpt90_120", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag2jet_vpt90_120", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag3jet_vpt90_120", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag2jet_vpt90_120", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag3jet_vpt90_120", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag2jet_vpt90_120", "");
   
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag3jet_vpt0_90", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2lltag2jet_vpt0_90", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag3jet_vpt0_90", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2mmtag2jet_vpt0_90", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag3jet_vpt0_90", "");
   //   config.addSensitivityPlot( "cuts", "mjj", "2tttag2jet_vpt0_90", "");

   config.addSensitivityPlot( "mva_sherpa221_truthall_mBBcut_systTTbar", "mva_sherpa221_truthall_mBBcut_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall_systTTbar", "mva_sherpa221_truthall_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall_mBBcut", "mva_sherpa221_truthall_mBBcut", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_mBBcut_systTTbar", "mva_sherpa221_mBBcut_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_mBBcut", "mva_sherpa221_mBBcut", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall", "mva_sherpa221_truthall", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_systTTbar", "mva_sherpa221_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221", "mva_sherpa221", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_mBBcut_systTTbar", "mva_WZ_sherpa221_truthall_mBBcut_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_systTTbar", "mva_WZ_sherpa221_truthall_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_mBBcut", "mva_WZ_sherpa221_truthall_mBBcut", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_mBBcut_systTTbar", "mva_WZ_sherpa221_mBBcut_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_mBBcut", "mva_WZ_sherpa221_mBBcut", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall", "mva_WZ_sherpa221_truthall", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_systTTbar", "mva_WZ_sherpa221_systTTbar", "2tag2jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221", "mva_WZ_sherpa221", "2tag2jet_150ptv_SR", "trafoD");

   config.addSensitivityPlot( "mva_sherpa221_truthall_mBBcut_systTTbar", "mva_sherpa221_truthall_mBBcut_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall_systTTbar", "mva_sherpa221_truthall_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall_mBBcut", "mva_sherpa221_truthall_mBBcut", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_mBBcut_systTTbar", "mva_sherpa221_mBBcut_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_mBBcut", "mva_sherpa221_mBBcut", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_truthall", "mva_sherpa221_truthall", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221_systTTbar", "mva_sherpa221_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_sherpa221", "mva_sherpa221", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_mBBcut_systTTbar", "mva_WZ_sherpa221_truthall_mBBcut_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_systTTbar", "mva_WZ_sherpa221_truthall_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall_mBBcut", "mva_WZ_sherpa221_truthall_mBBcut", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_mBBcut_systTTbar", "mva_WZ_sherpa221_mBBcut_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_mBBcut", "mva_WZ_sherpa221_mBBcut", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_truthall", "mva_WZ_sherpa221_truthall", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221_systTTbar", "mva_WZ_sherpa221_systTTbar", "2tag3jet_150ptv_SR", "trafoD");
   config.addSensitivityPlot( "mva_WZ_sherpa221", "mva_WZ_sherpa221", "2tag3jet_150ptv_SR", "trafoD");

  ///////////////////////////
  /// Now ready to make plots

  // check for errors in configuration
  if(!config.isValid()) {
    std::cout << "Error in configuration: ===> Aborting..." << std::endl;
    return;
  }

  // we have all what we need to know
  // ==> now make the plots
  PlotMaker plotter;
  plotter.makePlots(config);
}
