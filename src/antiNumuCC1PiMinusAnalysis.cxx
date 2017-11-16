#include "antiNumuCC1PiMinusAnalysis.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "antiNumuCC1PiMinusSelection.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "baseToyMaker.hxx"
#include "NuDirUtils.hxx"
#include "KinematicsUtils.hxx"
#include "TreeConverterUtils.hxx"

#include "antiNumuCCAnalysis.hxx"

/*  CHANGES IN NEW VERSION

    Updated documentation
*/



/*
  A highland2 Analysis inherits in ultimate instance from AnalysisAlgorithm. 
  In this particular case an intermediate class baseTrackerAnalysis is used, 
  which does all the work that is common for tracker analysis 
  Then antiNumuCC1PiMinusAnalysis inherits from baseTrackerAnalysis, which inherits from baseAnalysis, which inherits from AnalysisAlgorithm.
  
  The class that does the loops (over events, over toys, etc) is AnalysisLoop (under highlandCore). 
  There you can see the analysis flow, which is as follows

  LOOP over Spills{
    InitializeSpill
    LOOP over Bunches in the Spill{
      InitializeBunch
      LOOP over Configurations{
        InitializeConfiguration
        LOOP over Toy experiments for each configuration{
          InitializeToy
          LOOP over Enabled Selections{
            InitializeSelection
            Apply The Selection
            FinalizeSelection
          }
          FinalizeToy
          FillMicroTrees (Fill toy-dependent variables in micro-trees)
        }
        FillToyVarsInMicroTrees (Fill toy-independent variables)
        FillCategories (Fill categories for color code drawing)
        FinalizeConfiguration
      }
      FinalizeBunch
    }
    FillTruthTree (Fill the truth tree)
    FinalizeSpill
  }

  The Initialize.. and Finalize... methods can be implemented by the user to do more complicated analyses, but are not mandatory


  These is the list of mandatory methods to configure the analysis (call at initialization level):
  
  - Initialize:           This is all initializations that cannot be done in the constructor should be put 
                          (the ones that require access to a parameter in the parameters file)
  - DefineSelections:     Add to the SelectionManager the selections  we have defined in other files
  - DefineSystematics:    Add to the SystematicManager the systematics to be run (defined in other files)
  - DefineCorrections:    Add to the CorrectionManager the corrections to be run (defined in other files)
  - DefineConfigurations: Add to the ConfigurationManager the configurations to be considered
  - DefineMicroTrees:     Define the micro-tree variables
  - DefineTruthTree:      Define the variables of the truth tree

  These are the mandatory methods that are run for each Toy Experiment 

  - FillToyVarsInMicroTrees: Fill the micro-trees toy-dependent   variables (run for each toy experiment)

  These are the methods that are run for Event (Bunch)

  - FillMicroTrees:          Fill the micro-trees toy-independent variables. (MANDATORY) 
  - FillCategories:          Fill the micro-tree variables used for color code drawing (OPTIONAL)

  These are the mandatory methods that are run for each Spill

  - FillTruthTree:           Fill the truth tree variables
  - CheckFillTruthTree:      Check whether to include or not  a given true vertex in the truth tree

  These are the methods that are run at initialization level

  - FillConfigTree:           Fill the user defined variables in the single-entry config tree
  
*/


//********************************************************************
antiNumuCC1PiMinusAnalysis::antiNumuCC1PiMinusAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************
  // Add the package version (to be stored in the "config" tree)
  ND::versioning().AddPackage("antiNumuCC1PiMinusAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ANTINUMUCC1PIMINUSANALYSISROOT")));

  _antiNumuCCAnalysis = new antiNumuCCAnalysis(this);
  UseAnalysis(_antiNumuCCAnalysis);  

}

//********************************************************************
bool antiNumuCC1PiMinusAnalysis::Initialize(){
//********************************************************************
  
  /* In this method we Initialize everything that requires access to parameters in the parameters file. 
     This is because in order to the overwride parameters file 
     option (-p param.dat) to work, parameters cannot be accessed in the constructors. 
   */

  // Initialize the base class
  //  if (!baseTrackerAnalysis::Initialize()) return false;

  if (!_antiNumuCCAnalysis->Initialize()) return false;


  // We could add a second set of categories with a given prefix ("fgd2" in this case)
  //  anaUtils::AddStandardCategories("fgd2");
  
  // Minimum accum level to save event into the output Micro-trees
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("antiNumuCC1PiMinusAnalysis.MinAccumLevelToSave"));
  std::cout<<"ACCUM LEVEL "<<GetMinAccumCutLevelToSave()<<"\n";
  // Define categories
  //  anaUtils::AddStandardCategories();
  anaUtils::AddStandardAntiNumuCategories();

  // ----- Now add my custom category ---------
  
  std::string topology2_types[] = {"CC-0#pi",      "CC-1#pi^{+}",      "CC-other",     "NC-0#pi", "NC-1#pi^{+}", "NC-Other",  
				   "Anti CC-0#pi", "Anti CC-1#pi^{-}", "Anti CC-other","Anti NC-0#pi", "Anti NC-1#pi^{-}", "Anti NC-Other", 
				   "OOFV", "Other"};    // Names of the category types
  int topology2_codes[]         = { 1      , 2      , 3       , 4     ,    5,    6,
				    7     ,8      ,9       ,10    ,   11,   12, 
				    13  , 14    };    // code associated to each type
  int topology2_colors[]        = { kCyan      , kBlue      , kGreen       , kRed      , kPink, kMagenta,
				    kCyan - 6  , kBlue - 6  ,kGreen - 6    , kRed - 6  , kPink-6,kMagenta-6,
				    28 , 1};    // color associated to each type 
  const int NTYPES = sizeof(topology2_types)/sizeof(topology2_types[0]);       // get the number of types added
  // Add the category to the CategoryManager (currently a singleton, to be changed)
  anaUtils::_categ->AddCategory("topology2", NTYPES, topology2_types, topology2_codes, topology2_colors);

  return true;
}

//! [antiNumuCC1PiMinusAnalysis_DefineSelections]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineSelections(){
//********************************************************************
  
  /* In this method the user will add to the SelectionManager (accessed by  sel() ) the selections to be run, 
     defined in other files. In general an analysis has a single selection, which could have multiple branches. 
     Each of the branches is in fact a different selection (different cuts and actions), but defining them as branches 
     we ussualy gain in speed and simplicity since the steps that are common are applied only once. 
     Sometimes the user does not want to expend time merging two selections in a single one (as branches), but preffers to run 
     both independently. This is in general done for quick checks (are the selections mutually exclusive ?, etc). 
     This is possible in highland2. An example on how to do that is shown below. 

     In this case we add two selections to the SelectionManager provided:
     - Name of the selection (kTrackerTutorial, kTrackerTutorialBranches)
     - Title, the one dumped by the DrawingTools::DumpSelections() method. It is an explaination of the selection
     - Pointer to the selection. The argument in the constructor (false) indicates that the 
       step sequence is not broken when a cut is not passed. In this way we can save intermediate information for events 
       not passing the entire selection
   */


  // Add a simple selection with no branches
  sel().AddSelection("kFGD1AntiNumuCC1Pi","anti numu CC 1Pi- analysis",
		     new antiNumuCC1PiMinusSelection(false));
}
//! [antiNumuCC1PiMinusAnalysis_DefineSelections]

//! [antiNumuCC1PiMinusAnalysis_DefineCorrections]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineCorrections(){
//********************************************************************

  /* Corrections change the input data to rectify a problem in the MC or in the real data. Imagine for example that the energy deposited by a particle
     in a given sub-detector is overestimated in the MC by about 5%, and that this effect depends on the particle type (6% for muons and 4% for electrons).
     We could introduce a correction for the MC, which would scale the deposited energy by 0.94 for true muons and by 0.96 for true electrons. In this way
     we make sure that any cut using the deposited energy will have the same effect in data and MC, avoiding the corresponding systematic.

     The entire analysis will proceed on the modified data
     Corrections are only applied once per spill.
  */

  // Some corrections are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  //  baseTrackerAnalysis::DefineCorrections();

  _antiNumuCCAnalysis->DefineCorrections();
  // ----- We can add here extra corrections ----------

  // A dummy correction which moves forward the position of all global tracks by some mm, as specified in 
  // an external data file (data/tutorialCorrection.dat)
  //  corr().AddCorrection("tutorialCorrection", new tutorialCorrection());
}
//! [antiNumuCC1PiMinusAnalysis_DefineCorrections]

//! [antiNumuCC1PiMinusAnalysis_DefineSystematics]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineSystematics(){
//********************************************************************
  
  /*  Systematics will modify the effective number of events passing the selection and the distribution of the observables. 

      After corrections have been applied it is possible to 
      "manipulate" again the information using Systematics. In this case the purpose is different: we want to test several values of 
      a given detector property and check the impact on the number of selected events. This allows propagating systematic errors numerically. 
      
      There are two kind of systematics:
      - Variations: modify the input data (momentum, PID variables, etc). The selection is run on the modified data such that 
        the result of the selection can be different 
      - Weights: do not modify the input data. The selection is not affected. Simply a weight is added to the event. 
        Since events with different values of a given observable (i.e. momentum ) can have different weights, 
        distributions of that observable may change.
      
      In the above example about the deposited energy (in DefineCorrections(), the correction introduced cannot be perfectly known. 
      The 4% and 6% mentioned have an error (i.e. 0.5%). 
      This error should be propagated as a systematic. A given number of toy experiments will be run with different values of the scaling parameter 
      for the deposited energy (i.e. for muons 0.93, 0.935, 0.95, ..., following
      a gaussian distribution with mean 0.94 and sigma 0.005). If a cut on the deposited energy (or a variable using it)
      is applied the number of selected events could differ depending on the scaling applied.
      The RMS of the number of selected events for all toy experiments represents the systematic error induced by the deposited energy scaling.
   */


  // Some systematics are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  //  baseTrackerAnalysis::DefineSystematics();

  _antiNumuCCAnalysis->DefineSystematics();
  // ----- We can add here extra systematics ----------

  // An example of variation systematic, which is added to the EventVariationManager
  //evar().AddEventVariation(kTutorialVariation, "TutorialVarSyst",   new tutorialVariationSystematics());

  // A example of weight systematic, which is added to the EventWeightManager
  //eweight().AddEventWeight(   kTutorialWeight,    "TutorialWeightSyst", new tutorialWeightSystematics());
}
//! [antiNumuCC1PiMinusAnalysis_DefineSystematics]

//! [antiNumuCC1PiMinusAnalysis_DefineConfigurations]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineConfigurations(){
//********************************************************************

  /*  
      The user can run several analyses in parallel minimising the acces to disk. 
      Those parallel analyses are call configurations. Although this might be extended in the future, currenly 
      configurations only allow you to specify which systematics errors will be propagated, the number of toy experiments 
      and the random seed. 
      
      A tree for each configuration is produced in the output file, with the same name as the configuration. 
      By default a single configuration (called "default") is run, producing
      a single tree (with name default). This tree does not have any systematics enabled and hence it represents the nominal selection. 
  */

  // Some configurations are defined in baseTrackerAnalysis (have a look at baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx)
  //  baseTrackerAnalysis::DefineConfigurations();
  _antiNumuCCAnalysis->DefineConfigurations();
  // Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  // Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");

  // // Add a new configuration called "tutorial_syst" with the following properties:
  // // - the number of toys defined in the baseAnalysis parameters file
  // // - the random seed for toy experiments defined in the baseAnalysis parameters file
  // // - The ToyMaker used to CreateTheToyExperiments (random throws for each systematic parameter).
  // //   Have a look at baseToyMaker in baseAnalysis/vXrY/src
  // // - This configuration has two systematics kTutorialWeight and kTutorialVariation, defined above
  
  // AddConfiguration(conf(), tutorial_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
  // conf().EnableSystematic(kTutorialWeight,    tutorial_syst);  // Enable the kTutorialWeight in the tutorial_syst configuration
  // conf().EnableSystematic(kTutorialVariation, tutorial_syst);  // Enable the kTutorialVariation in the tutorial_syst configuration

  // // Disable the configuration when requested
  // if (!ND::params().GetParameterI("antiNumuCC1PiMinusAnalysis.Systematics.EnableTutorialSystematics"))
  //   conf().DisableConfiguration(tutorial_syst);
}
//! [antiNumuCC1PiMinusAnalysis_DefineConfigurations]

//! [antiNumuCC1PiMinusAnalysis_DefineMicroTrees]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************

  /*  We call Micro-trees to the standard analysis trees appearing in the output file. 
      There is always a Micro-Tree call "default" which should contain the basic info to understand our selection. 
      The user can add extra Micro-Trees by adding configurations to the analysis (see DefineConfigurations method above).

      Here we give an example of different variables that can be added. Have a look at OutputManager.hxx under highlandCore
      to see all available methods.
  */

  // -------- Add variables to the analysis tree ----------------------

  // Variables from baseTrackerAnalysis (run, event, ..., tracker related stuff, ...)
  //if (addBase) baseTrackerAnalysis::DefineMicroTrees(addBase);
  if (addBase) _antiNumuCCAnalysis->DefineMicroTrees(addBase);


  //--- Additional Muon Info ------------------
  AddVarI(     output(),  selmu_pdg,         "");
  AddVarF(output(), selmu_likepi,   "muon candidate pion TPC PID likelihood");
  AddVarF(output(), selmu_likep,   "muon candidate proton TPC PID likelihood");
  AddVarF(output(), selmu_likeele,   "muon candidate electron TPC PID likelihood");
  //--- PION INFORMATION TO SAVE -------
  AddVarI(        output(), selpi_pdg,             "");
  AddVarF(  output(), selpi_truemom,    "pion candidate true momentum");
  AddVar4VF(output(), selpi_truepos,    "pion candidate true position");
  AddVar4VF(output(), selpi_trueendpos, "pion candidate true end position");
  AddVar3VF(output(), selpi_truedir,    "pion candidate true direction");

  // --- pion candidate recon variables ---
  AddVar3VF(output(), selpi_dir,        "pion candidate reconstructed direction");
  AddVar3VF(output(), selpi_enddir,     "pion candidate reconstructed end direction");
  AddVar4VF(output(), selpi_pos,        "pion candidate reconstructed position");
  AddVar4VF(output(), selpi_endpos,     "pion candidate reconstructed end position");

  AddVarI(  output(), selpi_closest_tpc,"pion candidate closest TPC");
  AddVarI(  output(), selpi_detectors,  "pion candidate detectors");
  AddVarI(  output(), selpi_det,        "detector enum of the pion candidate start position");

  AddVarF(output(), selpi_mom,      "pion candidate reconstructed momentum");
  AddVarF(output(), selpi_costheta, "pion candidate reconstructed cos(theta). w.r.t. to neutrino direction");

  // --- reconstructed neutrino energy ----
  // (true neutrino energy is already saved in basAnalysis as nu_trueE)
  AddVarF(output(), selpi_nuErecQE,   "neutrino reconstructed energy with pion candidate's reconstructed kinematics in ccqe formula");

  AddVarF(output(), selpi_amom,     "pion candidate alternate reconstructed momentum ");
  AddVarF(output(), selpi_likemu,   "pion candidate muon TPC PID likelihood");
  AddVarF(output(), selpi_likemip,  "pion candidate MIP TPC PID likelihood");
  AddVarF(output(), selpi_likepi,   "pion candidate pion TPC PID likelihood");
  AddVarF(output(), selpi_likep,   "pion candidate proton TPC PID likelihood");
  AddVarF(output(), selpi_likeele,   "pion candidate electron TPC PID likelihood");
  
  // --- info by tpc
  AddVarVI(output(), selpi_tpc_det,         "pion candidate TPC number",                               selpi_ntpcs);
  AddVarVI(output(), selpi_tpc_nhits,       "pion candidate #hits in each TPC",                        selpi_ntpcs);
  AddVarVI(output(), selpi_tpc_nnodes,      "pion candidate #nodes in each TPC",                       selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_charge,      "pion candidate reconstructed charge in each TPC",         selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_mom,         "pion candidate reconstructed momentum in each TPC",       selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_bfield_mom,  "pion candidate reconstructed momentum with BField refit in each TPC",       selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_efield_mom,  "pion candidate reconstructed momentum with EFiled refit in each TPC",       selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_emom,        "pion candidate reconstructed momentum error in each TPC", selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_truemom,     "pion candidate true momentum in each TPC",                selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx_raw,    "pion candidate raw dEdx (CT) in each TPC",                selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx_expmu,  "pion candidate expected dEdx for pions in each TPC",      selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx_exppi,  "pion candidate expected dEdx for pions in each TPC",      selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx_expele, "pion candidate expected dEdx for electrons in each TPC",  selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx_expp,   "pion candidate expected dEdx for protons in each TPC",    selpi_ntpcs);

  
  AddVarVF(output(), selpi_tpc_pullmu,  "pion candidate muon pull in each TPC",     selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_pullpi,  "pion candidate pion pull in each TPC",     selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_pullele, "pion candidate electron pull in each TPC", selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_pullp,   "pion candidate proton pull in each TPC",   selpi_ntpcs);
  AddVarVF(output(), selpi_tpc_dedx,    "pion candidate dEdx (CT) in each TPC",     selpi_ntpcs);
  

  // --- info by FGD
  AddVarVI(output(), selpi_fgd_det,   "fgd index of pion candidate's fgd segments",      selpi_nfgds);
  AddVarVF(output(), selpi_fgd_x,     "pion candidate track length in each FGD",         selpi_nfgds);
  AddVarVF(output(), selpi_fgd_V11,   "pion candidate V11 vertex activity in each FGD",  selpi_nfgds);
  AddVarVF(output(), selpi_fgd_V33,   "pion candidate V33 vertex activity in each FGD",  selpi_nfgds);
  AddVarVF(output(), selpi_fgd_V55,   "pion candidate V55 vertex activity in each FGD",  selpi_nfgds);
  AddVarVF(output(), selpi_fgd_V77,   "pion candidate V77 vertex activity in each FGD",  selpi_nfgds);
  AddVarVF(output(), selpi_fgd_VLayer,"pion candidate VLayer vertex activity in each FGD",  selpi_nfgds);
  AddVarVF(output(), selpi_fgd_pullmu,"pion candidate muon pull in each FGD ", selpi_nfgds);
  

  // --- info by ECAL
  AddVarVI(output(), selpi_ecal_det,   "ECal sub-module",                                         selpi_necals);
  AddVarVI(output(), selpi_ecal_nhits, "The number of hits in the selected track's ECal segment", selpi_necals);
  AddVarVI(output(), selpi_ecal_nnodes,         "", selpi_necals);
  AddVarVF(output(), selpi_ecal_length,         "", selpi_necals);
  AddVarMF(output(), selpi_ecal_showerstartpos, "", selpi_necals,-30,4);
  AddVarMF(output(), selpi_ecal_showerendpos,   "", selpi_necals,-30,4);
  AddVarMF(output(), selpi_ecal_showerstartdir, "", selpi_necals,-30,3);
  AddVarMF(output(), selpi_ecal_showerenddir,   "", selpi_necals,-30,3);

  AddVarVF(output(), selpi_ecal_EMenergy,     "", selpi_necals);
  AddVarVF(output(), selpi_ecal_edeposit,     "", selpi_necals);
  AddVarVI(output(), selpi_ecal_IsShower,     "", selpi_necals);

  AddVarVF(output(), selpi_ecal_mipem,                "MipEm value of the selected track's ECal segment. Negative means more MIP-like, positive means more EM shower-like",     selpi_necals);
  AddVarVF(output(), selpi_ecal_mippion,              "MipPion value the selected track's ECal segment. Negative means more MIP-like, positive means more showering pion-like", selpi_necals);
  AddVarVF(output(), selpi_ecal_emhip,                "EmHip value of the selected track's ECal segment. Negative means more EM shower-like, positive means more HIP-like.",    selpi_necals);
  AddVarVF(output(), selpi_ecal_containment,          "",                                                                                                                       selpi_necals);
  AddVarVF(output(), selpi_ecal_trshval,              "",                                                                                                                       selpi_necals);
  AddVarVI(output(), selpi_ecal_mostupstreamlayerhit, "",                                                                                                                       selpi_necals);

  // --- info by SMRD
  AddVarVI(output(), selpi_smrd_det,    "", selpi_nsmrds);
  AddVarVI(output(), selpi_smrd_nhits,  "", selpi_nsmrds);
  AddVarVI(output(), selpi_smrd_nnodes, "", selpi_nsmrds);
  AddVarMF(output(), selpi_smrd_dir,    "", selpi_nsmrds, -30, 3);
  AddVarMF(output(), selpi_smrd_enddir, "", selpi_nsmrds, -30, 3);
  AddVarVF(output(), selpi_smrd_edeposit,     "", selpi_nsmrds);

  
  //--- OTHER INFORMATION TO SAVE -------
  //--- PION INFORMATION TO SAVE -------
  AddVarI(        output(), selrealmu_pdg,             "");
  AddVarF(  output(), selrealmu_truemom,    "pion candidate true momentum");
  AddVar4VF(output(), selrealmu_truepos,    "pion candidate true position");
  AddVar4VF(output(), selrealmu_trueendpos, "pion candidate true end position");
  AddVar3VF(output(), selrealmu_truedir,    "pion candidate true direction");

  // --- pion candidate recon variables ---
  AddVar3VF(output(), selrealmu_dir,        "pion candidate reconstructed direction");
  AddVar3VF(output(), selrealmu_enddir,     "pion candidate reconstructed end direction");
  AddVar4VF(output(), selrealmu_pos,        "pion candidate reconstructed position");
  AddVar4VF(output(), selrealmu_endpos,     "pion candidate reconstructed end position");

  AddVarI(  output(), selrealmu_closest_tpc,"pion candidate closest TPC");
  AddVarI(  output(), selrealmu_detectors,  "pion candidate detectors");
  AddVarI(  output(), selrealmu_det,        "detector enum of the pion candidate start position");

  AddVarF(output(), selrealmu_mom,      "pion candidate reconstructed momentum");
  AddVarF(output(), selrealmu_costheta, "pion candidate reconstructed cos(theta). w.r.t. to neutrino direction");

  // --- reconstructed neutrino energy ----
  // (true neutrino energy is already saved in basAnalysis as nu_trueE)
  AddVarF(output(), selrealmu_nuErecQE,   "neutrino reconstructed energy with pion candidate's reconstructed kinematics in ccqe formula");

  AddVarF(output(), selrealmu_amom,     "pion candidate alternate reconstructed momentum ");
  AddVarF(output(), selrealmu_likemu,   "pion candidate muon TPC PID likelihood");
  AddVarF(output(), selrealmu_likemip,  "pion candidate MIP TPC PID likelihood");
  AddVarF(output(), selrealmu_likepi,   "pion candidate pion TPC PID likelihood");
  
  // --- info by tpc
  AddVarVI(output(), selrealmu_tpc_det,         "pion candidate TPC number",                               selrealmu_ntpcs);
  AddVarVI(output(), selrealmu_tpc_nhits,       "pion candidate #hits in each TPC",                        selrealmu_ntpcs);
  AddVarVI(output(), selrealmu_tpc_nnodes,      "pion candidate #nodes in each TPC",                       selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_charge,      "pion candidate reconstructed charge in each TPC",         selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_mom,         "pion candidate reconstructed momentum in each TPC",       selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_bfield_mom,  "pion candidate reconstructed momentum with BField refit in each TPC",       selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_efield_mom,  "pion candidate reconstructed momentum with EFiled refit in each TPC",       selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_emom,        "pion candidate reconstructed momentum error in each TPC", selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_truemom,     "pion candidate true momentum in each TPC",                selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx_raw,    "pion candidate raw dEdx (CT) in each TPC",                selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx_expmu,  "pion candidate expected dEdx for pions in each TPC",      selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx_exppi,  "pion candidate expected dEdx for pions in each TPC",      selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx_expele, "pion candidate expected dEdx for electrons in each TPC",  selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx_expp,   "pion candidate expected dEdx for protons in each TPC",    selrealmu_ntpcs);

  
  AddVarVF(output(), selrealmu_tpc_pullmu,  "pion candidate muon pull in each TPC",     selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_pullpi,  "pion candidate pion pull in each TPC",     selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_pullele, "pion candidate electron pull in each TPC", selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_pullp,   "pion candidate proton pull in each TPC",   selrealmu_ntpcs);
  AddVarVF(output(), selrealmu_tpc_dedx,    "pion candidate dEdx (CT) in each TPC",     selrealmu_ntpcs);
  

  // --- info by FGD
  AddVarVI(output(), selrealmu_fgd_det,   "fgd index of pion candidate's fgd segments",      selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_x,     "pion candidate track length in each FGD",         selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_V11,   "pion candidate V11 vertex activity in each FGD",  selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_V33,   "pion candidate V33 vertex activity in each FGD",  selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_V55,   "pion candidate V55 vertex activity in each FGD",  selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_V77,   "pion candidate V77 vertex activity in each FGD",  selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_VLayer,"pion candidate VLayer vertex activity in each FGD",  selrealmu_nfgds);
  AddVarVF(output(), selrealmu_fgd_pullmu,"pion candidate muon pull in each FGD ", selrealmu_nfgds);
  

  // --- info by ECAL
  AddVarVI(output(), selrealmu_ecal_det,   "ECal sub-module",                                         selrealmu_necals);
  AddVarVI(output(), selrealmu_ecal_nhits, "The number of hits in the selected track's ECal segment", selrealmu_necals);
  AddVarVI(output(), selrealmu_ecal_nnodes,         "", selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_length,         "", selrealmu_necals);
  AddVarMF(output(), selrealmu_ecal_showerstartpos, "", selrealmu_necals,-30,4);
  AddVarMF(output(), selrealmu_ecal_showerendpos,   "", selrealmu_necals,-30,4);
  AddVarMF(output(), selrealmu_ecal_showerstartdir, "", selrealmu_necals,-30,3);
  AddVarMF(output(), selrealmu_ecal_showerenddir,   "", selrealmu_necals,-30,3);

  AddVarVF(output(), selrealmu_ecal_EMenergy,     "", selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_edeposit,     "", selrealmu_necals);
  AddVarVI(output(), selrealmu_ecal_IsShower,     "", selrealmu_necals);

  AddVarVF(output(), selrealmu_ecal_mipem,                "MipEm value of the selected track's ECal segment. Negative means more MIP-like, positive means more EM shower-like",     selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_mippion,              "MipPion value the selected track's ECal segment. Negative means more MIP-like, positive means more showering pion-like", selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_emhip,                "EmHip value of the selected track's ECal segment. Negative means more EM shower-like, positive means more HIP-like.",    selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_containment,          "",                                                                                                                       selrealmu_necals);
  AddVarVF(output(), selrealmu_ecal_trshval,              "",                                                                                                                       selrealmu_necals);
  AddVarVI(output(), selrealmu_ecal_mostupstreamlayerhit, "",                                                                                                                       selrealmu_necals);

  // --- info by SMRD
  AddVarVI(output(), selrealmu_smrd_det,    "", selrealmu_nsmrds);
  AddVarVI(output(), selrealmu_smrd_nhits,  "", selrealmu_nsmrds);
  AddVarVI(output(), selrealmu_smrd_nnodes, "", selrealmu_nsmrds);
  AddVarMF(output(), selrealmu_smrd_dir,    "", selrealmu_nsmrds, -30, 3);
  AddVarMF(output(), selrealmu_smrd_enddir, "", selrealmu_nsmrds, -30, 3);
  AddVarVF(output(), selrealmu_smrd_edeposit,     "", selrealmu_nsmrds);
  

  
}
//! [antiNumuCC1PiMinusAnalysis_DefineMicroTrees]





//! [antiNumuCC1PiMinusAnalysis_DefineTruthTree]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::DefineTruthTree(){
//********************************************************************

  /*
//! [antiNumuCC1PiMinusAnalysis_DefineTruthTree_info]
    The main trees in the output file are the "default" and "truth" trees. The "default" tree contains all selected events (the ones that passed a given cut, ussualy 
    specified in the parameters file). This is the tree used to optimise the selection cuts, to make selection plots and to compute or plot the purity of the selection. 
    The main reason for having a separate tree, call "truth", is to allow the efficiency computation. The "default" tree does not contain all true signal events, 
    because it will be too big. The preselection for that tree is only based in reconstructed quantities (selection cuts). Instead, to compute true signal efficiencies 
    we need all true signal events to be present in the output tree. The "truth" tree fullfils this condition. Since, in general, this tree will contain many events, only a 
    reduced set of variables (the ones needed to compute the efficiency) is stored in that tree. 
    
//! [antiNumuCC1PiMinusAnalysis_DefineTruthTree_info]

    SUMMARY: The "truth" tree also appears in the output file. It contains all interactions in which we are interested in regardless on whether 
    the selection was passed or not. This is the tree that should be used to compute signal efficiencies
*/


  // Variables from baseTrackerAnalysis (run, event, ...)
  //baseTrackerAnalysis::DefineTruthTree();
  _antiNumuCCAnalysis->DefineTruthTree();
  AddVarF(  output(), truepion_mom,      "true pion momentum");
  AddVarF(  output(), trueproton_mom,      "true proton momentum");
  // //--- muon variables -------
  // AddVarI(  output(), truelepton_pdg,      "true lepton PDG");
  // AddVar4VF(output(), truelepton_pos,      "true lepton position");
  // // Moved to baseAnalysis !!!!!!!
  // //  AddVarF(  output(), truelepton_mom,      "true lepton momentum");
  // //  AddVarF(  output(), truelepton_costheta, "true lepton cos(theta) w.r.t. neutrino direction");
  // //  AddVar3VF(output(), truelepton_dir,      "true lepton direction"); 
}
//! [antiNumuCC1PiMinusAnalysis_DefineTruthTree]

//! [antiNumuCC1PiMinusAnalysis_FillMicroTrees]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::FillMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-independent variables (all except the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called once all toys has been run, what means that the value of all variables for the last toy will be saved. This is not a problem 
      for variables that are not expected to change from a toy to another.
  */

  // Variables from baseTrackerAnalysis (run, event, ...)
  if (addBase) _antiNumuCCAnalysis->FillMicroTrees(addBase);

  //flag for filling possible filling of other track
  bool searchother=false;
  if(mybox().MainTrack) {
    output().FillVar(selmu_likepi, anaUtils::GetPIDLikelihood( *(mybox().MainTrack),3));
    output().FillVar(selmu_likep, anaUtils::GetPIDLikelihood( *(mybox().MainTrack),2));
    output().FillVar(selmu_likeele, anaUtils::GetPIDLikelihood( *(mybox().MainTrack),1));
    if(mybox().MainTrack->TrueObject) {
      output().FillVar(selmu_pdg,     mybox().MainTrack->GetTrueParticle()->PDG);
      if(mybox().MainTrack->GetTrueParticle()->PDG != -13)
	searchother=true;

      AnaTrueVertex *vtx = static_cast<AnaTrueVertex*>(mybox().MainTrack->GetTrueParticle()->TrueVertex);
      //if(anaUtils::GetTopology(*vtx, SubDetId::kFGD1,true) == 1)
      //std::cout<<"nNeg TPC tracks "<<mybox().nNegativeTPCtracks<<"\n";
      // if(mybox().nNegativeTPCtracks>1) {//  && anaUtils::GetTopology(*vtx, SubDetId::kFGD1,true) == 1) {
      // 	for(int i=0;i<mybox().nNegativeTPCtracks;i++) {
      // 	  if(mybox().NegativeTPCtracks[i]->TrueObject) {
      // 	    std::cout<<"Negative Track "<<i<<" "<<mybox().NegativeTPCtracks[i]->GetTrueParticle()->PDG<<" topology "<<anaUtils::GetTopology(*vtx, SubDetId::kFGD1,true)<<"\n";
      // 	  }
	  
      // 	}
      // }
      
      
    }
  }

  if(mybox().NegativePionTPCtracks[0]) {
    if(mybox().NegativePionTPCtracks[0]->TrueObject) {
      output().FillVar(selpi_truemom, mybox().NegativePionTPCtracks[0]->GetTrueParticle()->Momentum);
      output().FillVar(selpi_pdg,     mybox().NegativePionTPCtracks[0]->GetTrueParticle()->PDG);
      output().FillVectorVarFromArray(selpi_trueendpos, mybox().NegativePionTPCtracks[0]->GetTrueParticle()->PositionEnd, 4);
      output().FillVar(selpi_truemom,          mybox().NegativePionTPCtracks[0]->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selpi_truepos,   mybox().NegativePionTPCtracks[0]->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selpi_trueendpos,mybox().NegativePionTPCtracks[0]->GetTrueParticle()->PositionEnd, 4);
      output().FillVectorVarFromArray(selpi_truedir,   mybox().NegativePionTPCtracks[0]->GetTrueParticle()->Direction, 3);
 
    }
    // Momentum
    output().FillVar(selpi_mom, mybox().NegativePionTPCtracks[0]->Momentum);

    // Costheta
    // vertex defined as start of muon track, neutrino direction calculated from this
    TVector3 nuDirVec = anaUtils::GetNuDirRec(mybox().NegativePionTPCtracks[0]->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(mybox().NegativePionTPCtracks[0]->DirectionStart);
    // The cosine of the angle between the muon and the neutrino
    double costheta_pi_nu = nuDirVec.Dot(muDirVec);
    output().FillVar(selpi_costheta, (Float_t)costheta_pi_nu);

    output().FillVectorVarFromArray(selpi_pos,    mybox().NegativePionTPCtracks[0]->PositionStart, 4);
    output().FillVectorVarFromArray(selpi_endpos, mybox().NegativePionTPCtracks[0]->PositionEnd, 4);
    output().FillVectorVarFromArray(selpi_dir,    mybox().NegativePionTPCtracks[0]->DirectionStart, 3);
    output().FillVectorVarFromArray(selpi_enddir, mybox().NegativePionTPCtracks[0]->DirectionEnd, 3);
    output().FillVar(selpi_det,                   anaUtils::GetDetector(mybox().NegativePionTPCtracks[0]->PositionStart));
    output().FillVar(selpi_amom, (static_cast<AnaTrack*>(mybox().NegativePionTPCtracks[0]))->MomentumMuon);
    // PID likelihoods
    output().FillVar(selpi_likemu, anaUtils::GetPIDLikelihood( *(mybox().NegativePionTPCtracks[0]),0));
    output().FillVar(selpi_likemip,anaUtils::GetPIDLikelihoodMIP( *(mybox().NegativePionTPCtracks[0])));
    output().FillVar(selpi_likepi, anaUtils::GetPIDLikelihood( *(mybox().NegativePionTPCtracks[0]),3));
    output().FillVar(selpi_likep, anaUtils::GetPIDLikelihood( *(mybox().NegativePionTPCtracks[0]),2));
    output().FillVar(selpi_likeele, anaUtils::GetPIDLikelihood( *(mybox().NegativePionTPCtracks[0]),1));
        // The closest TPC
    SubDetId::SubDetEnum tpc = anaUtils::GetClosestTPC(*(mybox().NegativePionTPCtracks[0]));
    output().FillVar(selpi_closest_tpc, tpc+1);

    // Track composition
    output().FillVar(selpi_detectors, static_cast<AnaTrack*>(mybox().NegativePionTPCtracks[0])->Detectors);

    // Info in all TPCs

    for (Int_t subdet = 0; subdet<3; subdet++) {
      if (!SubDetId::GetDetectorUsed(mybox().NegativePionTPCtracks[0]->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
      AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *mybox().NegativePionTPCtracks[0],static_cast<SubDetId::SubDetEnum >(subdet+2)));
      if (!TPCSegment) continue;
      output().FillVectorVar(selpi_tpc_det,     subdet);
      output().FillVectorVar(selpi_tpc_mom,     TPCSegment->Momentum);

      output().FillVectorVar(selpi_tpc_bfield_mom, TPCSegment->RefitMomentum);
      output().FillVectorVar(selpi_tpc_efield_mom, TPCSegment->EFieldRefitMomentum);
      output().FillVectorVar(selpi_tpc_charge,  TPCSegment->Charge);
      output().FillVectorVar(selpi_tpc_nhits,   TPCSegment->NHits);
      output().FillVectorVar(selpi_tpc_nnodes,  TPCSegment->NNodes);
      output().FillVectorVar(selpi_tpc_emom,    TPCSegment->MomentumError);

      if (TPCSegment->TrueObject)
        output().FillVectorVar(selpi_tpc_truemom, TPCSegment->GetTrueParticle()->Momentum);

      if (TPCSegment->Original->Original)
        output().FillVectorVar(selpi_tpc_dedx_raw,    (static_cast< const AnaTPCParticle* >(TPCSegment->Original->Original))->dEdxMeas);

      output().FillVectorVar(selpi_tpc_dedx_expmu,  TPCSegment->dEdxexpMuon);
      output().FillVectorVar(selpi_tpc_dedx_exppi,  TPCSegment->dEdxexpPion);
      output().FillVectorVar(selpi_tpc_dedx_expele, TPCSegment->dEdxexpEle);
      output().FillVectorVar(selpi_tpc_dedx_expp,   TPCSegment->dEdxexpProton);

  
      output().FillVectorVar(selpi_tpc_dedx,   TPCSegment->dEdxMeas);
      output().FillVectorVar(selpi_tpc_pullmu, TPCSegment->Pullmu);
      output().FillVectorVar(selpi_tpc_pullpi, TPCSegment->Pullpi);
      output().FillVectorVar(selpi_tpc_pullele,TPCSegment->Pullele);
      output().FillVectorVar(selpi_tpc_pullp,  TPCSegment->Pullp);
      

      output().IncrementCounterForVar(selpi_tpc_det);
    }

        // Info in all FGDs
    for (Int_t subdet = 0; subdet<2; subdet++) {
      if (!SubDetId::GetDetectorUsed(mybox().NegativePionTPCtracks[0]->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
      AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet( *mybox().NegativePionTPCtracks[0],static_cast<SubDetId::SubDetEnum >(subdet)));
      if (!FGDSegment) continue;

      // output().FillVectorVar("selpi_fgd_det",     (Int_t)FGDSegment->Detector);
      output().FillVectorVar(selpi_fgd_det,      subdet);
      output().FillVectorVar(selpi_fgd_x,        FGDSegment->X);
      output().FillVectorVar(selpi_fgd_V11,      FGDSegment->Vertex1x1);
      output().FillVectorVar(selpi_fgd_V33,      FGDSegment->Vertex3x3);
      output().FillVectorVar(selpi_fgd_V55,      FGDSegment->Vertex5x5);
      output().FillVectorVar(selpi_fgd_V77,      FGDSegment->Vertex7x7);
      output().FillVectorVar(selpi_fgd_VLayer,   FGDSegment->VertexLayer);

      if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars"))
        output().FillVectorVar(selpi_fgd_pullmu,   FGDSegment->Pullmu);

      output().IncrementCounterForVar(selpi_fgd_det);
    }

    // Info in all ECALs
    for (Int_t subdet = 0; subdet<9; subdet++) {
      if (!SubDetId::GetDetectorUsed(mybox().NegativePionTPCtracks[0]->Detector, static_cast<SubDetId::SubDetEnum >(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet( *mybox().NegativePionTPCtracks[0],static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) {
	// Int_t tempcont[]={-999,-999};
	// output().FillVectorVar(selpi_ecal_containment,tempcont);
	
	continue;
      }

      output().FillVectorVar(selpi_ecal_det,                     subdet);
      output().FillVectorVar(selpi_ecal_nhits,                   ECALSegment->NHits);
      output().FillVectorVar(selpi_ecal_nnodes,                  ECALSegment->NNodes);
      output().FillVectorVar(selpi_ecal_length,                  ECALSegment->Length);
      output().FillMatrixVarFromArray(selpi_ecal_showerstartpos, ECALSegment->PositionStart,  4 );
      output().FillMatrixVarFromArray(selpi_ecal_showerendpos,   ECALSegment->PositionEnd,    4 );
      output().FillMatrixVarFromArray(selpi_ecal_showerstartdir, ECALSegment->DirectionStart, 3 );
      output().FillMatrixVarFromArray(selpi_ecal_showerenddir,   ECALSegment->DirectionEnd,   3 );

      output().FillVectorVar(selpi_ecal_EMenergy,      ECALSegment->EMEnergy);
      output().FillVectorVar(selpi_ecal_edeposit,      ECALSegment->EDeposit);
      output().FillVectorVar(selpi_ecal_IsShower,      ECALSegment->IsShowerLike);

      output().FillVectorVar(selpi_ecal_trshval,              ECALSegment->TrShVal);
      output().FillVectorVar(selpi_ecal_emhip,                ECALSegment->PIDEmHip);
      output().FillVectorVar(selpi_ecal_mippion,              ECALSegment->PIDMipPion);
      output().FillVectorVar(selpi_ecal_mipem,                ECALSegment->PIDMipEm);
      output().FillVectorVar(selpi_ecal_containment,          ECALSegment->Containment);
      output().FillVectorVar(selpi_ecal_mostupstreamlayerhit, ECALSegment->MostUpStreamLayerHit);

      output().IncrementCounterForVar(selpi_ecal_det);

    }

    // Info in SMRDs
    for (Int_t i = 0; i < mybox().NegativePionTPCtracks[0]->nSMRDSegments; i++){
      AnaSMRDParticle* smrdTrack = static_cast<AnaSMRDParticle*>(mybox().NegativePionTPCtracks[0]->SMRDSegments[i]);
      if (!smrdTrack) continue;

      output().FillVectorVar(selpi_smrd_det,
			     convUtils::GetLocalDetEnum(SubDetId::kSMRD, SubDetId::GetSubdetectorEnum(smrdTrack->Detector)));
      output().FillVectorVar(selpi_smrd_nhits,    smrdTrack->NHits);
      output().FillVectorVar(selpi_smrd_nnodes,   smrdTrack->NNodes);
      output().FillVectorVar(selpi_smrd_edeposit, smrdTrack->EDeposit);
      output().FillMatrixVarFromArray(selpi_smrd_dir,    smrdTrack->DirectionStart,    3 );
      output().FillMatrixVarFromArray(selpi_smrd_enddir, smrdTrack->DirectionEnd, 3 );
      output().IncrementCounterForVar(selpi_smrd_det);
    }

  }

  //check if we need to search the other tracks for the muon, only positive tracks as this is relevant
  //to this selction
  bool muonfound=false;
  int muindex;
  if(searchother) {
    for(int ii=0;ii<mybox().nOtherTPCtracks;ii++) {
      if(mybox().OtherTPCtracks[ii]->TrueObject) {
	if(mybox().OtherTPCtracks[ii]->GetTrueParticle()->PDG == -13) {
	  muonfound=true;
	  muindex=ii;
	}
      }
    }
  }

  AnaTrackB* realmuon = mybox().OtherTPCtracks[muindex];
  if(muonfound) {
    if(realmuon->TrueObject) {
      output().FillVar(selrealmu_pdg,     realmuon->GetTrueParticle()->PDG);
      output().FillVectorVarFromArray(selrealmu_trueendpos, realmuon->GetTrueParticle()->PositionEnd, 4);
      output().FillVar(selrealmu_truemom,          realmuon->GetTrueParticle()->Momentum);
      output().FillVectorVarFromArray(selrealmu_truepos,   realmuon->GetTrueParticle()->Position, 4);
      output().FillVectorVarFromArray(selrealmu_trueendpos,realmuon->GetTrueParticle()->PositionEnd, 4);
      output().FillVectorVarFromArray(selrealmu_truedir,   realmuon->GetTrueParticle()->Direction, 3);
 
    }
    // Momentum
    output().FillVar(selrealmu_mom, realmuon->Momentum);

    // Costheta
    // vertex defined as start of muon track, neutrino direction calculated from this
    TVector3 nuDirVec = anaUtils::GetNuDirRec(realmuon->PositionStart);
    TVector3 muDirVec = anaUtils::ArrayToTVector3(realmuon->DirectionStart);
    // The cosine of the angle between the muon and the neutrino
    double costheta_pi_nu = nuDirVec.Dot(muDirVec);
    output().FillVar(selrealmu_costheta, (Float_t)costheta_pi_nu);

    output().FillVectorVarFromArray(selrealmu_pos,    realmuon->PositionStart, 4);
    output().FillVectorVarFromArray(selrealmu_endpos, realmuon->PositionEnd, 4);
    output().FillVectorVarFromArray(selrealmu_dir,    realmuon->DirectionStart, 3);
    output().FillVectorVarFromArray(selrealmu_enddir, realmuon->DirectionEnd, 3);
    output().FillVar(selrealmu_det,                   anaUtils::GetDetector(realmuon->PositionStart));
    output().FillVar(selrealmu_amom, (static_cast<AnaTrack*>(realmuon))->MomentumMuon);
    // PID likelihoods
    output().FillVar(selrealmu_likemu, anaUtils::GetPIDLikelihood( *(realmuon),0));
    output().FillVar(selrealmu_likemip,anaUtils::GetPIDLikelihoodMIP( *(realmuon)));
    output().FillVar(selrealmu_likepi, anaUtils::GetPIDLikelihood( *(realmuon),3));

        // The closest TPC
    SubDetId::SubDetEnum tpc = anaUtils::GetClosestTPC(*(realmuon));
    output().FillVar(selrealmu_closest_tpc, tpc+1);

    // Track composition
    output().FillVar(selrealmu_detectors, static_cast<AnaTrack*>(realmuon)->Detectors);

    // Info in all TPCs

    for (Int_t subdet = 0; subdet<3; subdet++) {
      if (!SubDetId::GetDetectorUsed(realmuon->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
      AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *realmuon,static_cast<SubDetId::SubDetEnum >(subdet+2)));
      if (!TPCSegment) continue;
      output().FillVectorVar(selrealmu_tpc_det,     subdet);
      output().FillVectorVar(selrealmu_tpc_mom,     TPCSegment->Momentum);

      output().FillVectorVar(selrealmu_tpc_bfield_mom, TPCSegment->RefitMomentum);
      output().FillVectorVar(selrealmu_tpc_efield_mom, TPCSegment->EFieldRefitMomentum);
      output().FillVectorVar(selrealmu_tpc_charge,  TPCSegment->Charge);
      output().FillVectorVar(selrealmu_tpc_nhits,   TPCSegment->NHits);
      output().FillVectorVar(selrealmu_tpc_nnodes,  TPCSegment->NNodes);
      output().FillVectorVar(selrealmu_tpc_emom,    TPCSegment->MomentumError);

      if (TPCSegment->TrueObject)
        output().FillVectorVar(selrealmu_tpc_truemom, TPCSegment->GetTrueParticle()->Momentum);

      if (TPCSegment->Original->Original)
        output().FillVectorVar(selrealmu_tpc_dedx_raw,    (static_cast< const AnaTPCParticle* >(TPCSegment->Original->Original))->dEdxMeas);

      output().FillVectorVar(selrealmu_tpc_dedx_expmu,  TPCSegment->dEdxexpMuon);
      output().FillVectorVar(selrealmu_tpc_dedx_exppi,  TPCSegment->dEdxexpPion);
      output().FillVectorVar(selrealmu_tpc_dedx_expele, TPCSegment->dEdxexpEle);
      output().FillVectorVar(selrealmu_tpc_dedx_expp,   TPCSegment->dEdxexpProton);

  
      output().FillVectorVar(selrealmu_tpc_dedx,   TPCSegment->dEdxMeas);
      output().FillVectorVar(selrealmu_tpc_pullmu, TPCSegment->Pullmu);
      output().FillVectorVar(selrealmu_tpc_pullpi, TPCSegment->Pullpi);
      output().FillVectorVar(selrealmu_tpc_pullele,TPCSegment->Pullele);
      output().FillVectorVar(selrealmu_tpc_pullp,  TPCSegment->Pullp);
      

      output().IncrementCounterForVar(selrealmu_tpc_det);
    }

        // Info in all FGDs
    for (Int_t subdet = 0; subdet<2; subdet++) {
      if (!SubDetId::GetDetectorUsed(realmuon->Detector, static_cast<SubDetId::SubDetEnum >(subdet))) continue;
      AnaFGDParticle* FGDSegment = static_cast<AnaFGDParticle*>(anaUtils::GetSegmentInDet( *realmuon,static_cast<SubDetId::SubDetEnum >(subdet)));
      if (!FGDSegment) continue;

      // output().FillVectorVar("selrealmu_fgd_det",     (Int_t)FGDSegment->Detector);
      output().FillVectorVar(selrealmu_fgd_det,      subdet);
      output().FillVectorVar(selrealmu_fgd_x,        FGDSegment->X);
      output().FillVectorVar(selrealmu_fgd_V11,      FGDSegment->Vertex1x1);
      output().FillVectorVar(selrealmu_fgd_V33,      FGDSegment->Vertex3x3);
      output().FillVectorVar(selrealmu_fgd_V55,      FGDSegment->Vertex5x5);
      output().FillVectorVar(selrealmu_fgd_V77,      FGDSegment->Vertex7x7);
      output().FillVectorVar(selrealmu_fgd_VLayer,   FGDSegment->VertexLayer);

      if (!ND::params().GetParameterI("numuCCAnalysis.MicroTrees.AdditionalToyVars"))
        output().FillVectorVar(selrealmu_fgd_pullmu,   FGDSegment->Pullmu);

      output().IncrementCounterForVar(selrealmu_fgd_det);
    }

    // Info in all ECALs
    for (Int_t subdet = 0; subdet<9; subdet++) {
      if (!SubDetId::GetDetectorUsed(realmuon->Detector, static_cast<SubDetId::SubDetEnum >(subdet+6))) continue;

      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(anaUtils::GetSegmentInDet( *realmuon,static_cast<SubDetId::SubDetEnum >(subdet+6)));

      if (!ECALSegment) continue;

      output().FillVectorVar(selrealmu_ecal_det,                     subdet);
      output().FillVectorVar(selrealmu_ecal_nhits,                   ECALSegment->NHits);
      output().FillVectorVar(selrealmu_ecal_nnodes,                  ECALSegment->NNodes);
      output().FillVectorVar(selrealmu_ecal_length,                  ECALSegment->Length);
      output().FillMatrixVarFromArray(selrealmu_ecal_showerstartpos, ECALSegment->PositionStart,  4 );
      output().FillMatrixVarFromArray(selrealmu_ecal_showerendpos,   ECALSegment->PositionEnd,    4 );
      output().FillMatrixVarFromArray(selrealmu_ecal_showerstartdir, ECALSegment->DirectionStart, 3 );
      output().FillMatrixVarFromArray(selrealmu_ecal_showerenddir,   ECALSegment->DirectionEnd,   3 );

      output().FillVectorVar(selrealmu_ecal_EMenergy,      ECALSegment->EMEnergy);
      output().FillVectorVar(selrealmu_ecal_edeposit,      ECALSegment->EDeposit);
      output().FillVectorVar(selrealmu_ecal_IsShower,      ECALSegment->IsShowerLike);

      output().FillVectorVar(selrealmu_ecal_trshval,              ECALSegment->TrShVal);
      output().FillVectorVar(selrealmu_ecal_emhip,                ECALSegment->PIDEmHip);
      output().FillVectorVar(selrealmu_ecal_mippion,              ECALSegment->PIDMipPion);
      output().FillVectorVar(selrealmu_ecal_mipem,                ECALSegment->PIDMipEm);
      output().FillVectorVar(selrealmu_ecal_containment,          ECALSegment->Containment);
      output().FillVectorVar(selrealmu_ecal_mostupstreamlayerhit, ECALSegment->MostUpStreamLayerHit);

      output().IncrementCounterForVar(selrealmu_ecal_det);

    }

    // Info in SMRDs
    for (Int_t i = 0; i < realmuon->nSMRDSegments; i++){
      AnaSMRDParticle* smrdTrack = static_cast<AnaSMRDParticle*>(realmuon->SMRDSegments[i]);
      if (!smrdTrack) continue;

      output().FillVectorVar(selrealmu_smrd_det,
			     convUtils::GetLocalDetEnum(SubDetId::kSMRD, SubDetId::GetSubdetectorEnum(smrdTrack->Detector)));
      output().FillVectorVar(selrealmu_smrd_nhits,    smrdTrack->NHits);
      output().FillVectorVar(selrealmu_smrd_nnodes,   smrdTrack->NNodes);
      output().FillVectorVar(selrealmu_smrd_edeposit, smrdTrack->EDeposit);
      output().FillMatrixVarFromArray(selrealmu_smrd_dir,    smrdTrack->DirectionStart,    3 );
      output().FillMatrixVarFromArray(selrealmu_smrd_enddir, smrdTrack->DirectionEnd, 3 );
      output().IncrementCounterForVar(selrealmu_smrd_det);
    }

  }
  //Fill -muon variables
  
  
}
//! [antiNumuCC1PiMinusAnalysis_FillMicroTrees]

//! [antiNumuCC1PiMinusAnalysis_FillToyVarsInMicroTrees]
//********************************************************************
void antiNumuCC1PiMinusAnalysis::FillToyVarsInMicroTrees(bool addBase){
//********************************************************************

  /*  In this method we fill all toy-dependent variables (the ones added with AddToy...) defined in the method DefineMicroTrees. 
      This method is called at the end of each toy.

      There could be many variables that are toy-dependent. We don't need to save all of them as toy variables, but only the ones we are interested in plotting 
      for different toys. 

      TOY VARIABLES ARE VERY SPACE CONSUMING SO WE SHOULD MINIMISE ITS NUMBER !!!!
  */

  // Fill the common variables
  if (addBase) _antiNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
  // //---- variables specific for this analysis -----
  // if (box().MainTrack){
  //   // Since we are applying a systematic that varies the momentum we need to save the momentum for each toy
  //   output().FillToyVar(selmu_mom, box().MainTrack->Momentum);    

  //   // Info in all TPCs 
  //   for (Int_t subdet = 0; subdet<3; subdet++) {
  //     if (!SubDetId::GetDetectorUsed(box().MainTrack->Detector, static_cast<SubDetId::SubDetEnum >(subdet+2))) continue;
  //     AnaTPCParticle* TPCSegment = static_cast<AnaTPCParticle*>(anaUtils::GetSegmentInDet( *box().MainTrack, static_cast<SubDetId::SubDetEnum >(subdet+2)));
  //     if (!TPCSegment) continue;
  //     // In principle we need this variable here when PID systematics are run
  //     output().FillToyVectorVar(selmu_tpc_dedx,   TPCSegment->dEdxMeas, subdet);
  //   }
  // }
}
//! [antiNumuCC1PiMinusAnalysis_FillToyVarsInMicroTrees]

//********************************************************************
bool antiNumuCC1PiMinusAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  /* To avoid unecessary events in the "truth" tree in this method we define the condition to include or not a given 
     true vertex in the tree. In principle we whould only save the true vertices that match our signal definition
  */

  // In this case we only save numu (NuPDG=14) charged current  (0<ReacCode<30) interactions in the FGD1 FV
  // bool numuCC=vtx.ReacCode>0 && vtx.ReacCode<30 && vtx.NuPDG==14;// && vtx.LeptonPDG==13;  
  // return (anaUtils::InFiducialVolume(SubDetId::kFGD1, vtx.Position, FVDef::FVdefminFGD1,FVDef::FVdefmaxFGD1) && numuCC);
  
  

  return true;
}

//********************************************************************
void antiNumuCC1PiMinusAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************

  bool IsAntinu = true;
  // Fill the common variables defined in baseTrackerAnalysis/vXrY/src/baseTrackerAnalysis.cxx
  _antiNumuCCAnalysis->FillTruthTree(vtx);
  
  // ---- Fill the extra variables ------
  //output().FillVar(truelepton_pdg,vtx.LeptonPDG);
  //output().FillVectorVarFromArray(truelepton_pos, vtx.Position, 4);
  
  // Moved to baseAnalysis !!!!!!!
  //  output().FillVar(truelepton_costheta,(Float_t)cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
  

  
  output().FillVar(truepion_mom,vtx.PionMom);
  //std::cout<<vtx.PionMom<<" "<<vtx.LeptonMom<<" "<<vtx.ProtonMom<<" "<<"\n";
  output().FillVar(trueproton_mom,vtx.ProtonMom);
  //  output().FillVectorVarFromArray(truelepton_dir, vtx.LeptonDir,3);

  
 
  Int_t code = -999;
  code=GetTopology2(vtx, SubDetId::kFGD1, IsAntinu);
  cat().SetCode("topology2",code,-999);
}

//********************************************************************
void antiNumuCC1PiMinusAnalysis::FillCategories(){
//********************************************************************

  /* This method fills the micro-tree variables related with event and particle categories for color drawing. 
     Those variables are added automatically (no need to explicitely add them in DefineMicroTrees) to the 
     micro-trees, but must be filled by the analyser, provided the event and the relevant track (the lepton candidate, 
     that is the HMN track, in this case). 

     This method is not in baseTrackerAnalysis anymore, meaning that if it is not implemented in your analysis, the cattegories will not be filled
  */

  bool IsAntinu = true;
  // For the muon candidate
  //  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"", SubDetId::kFGD1);
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kFGD1,IsAntinu);
  // We could add a second set of categories with a given prefix ("fgd2" in this case). See antiNumuCC1PiMinusAnalysis::Initialize() above
  //  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"fgd2", SubDetId::kFGD2);

  Int_t code = -999;
  //now let us fill the topology2 categories
  if(mybox().MainTrack) {
    if(mybox().MainTrack->TrueObject){ 
      if(mybox().MainTrack->GetTrueParticle()->PDG!= 0 ){
	AnaTrueParticle* trueTrack = static_cast<AnaTrueParticle*>(mybox().MainTrack->TrueObject);
	if(trueTrack->TrueVertex) {
	  const AnaTrueVertex* trueVertex = static_cast<const AnaTrueVertex*>(trueTrack->TrueVertex);
	  code = GetTopology2(*trueVertex, SubDetId::kFGD1, IsAntinu);
	}
      }
    }
  }

  cat().SetCode("topology2",code,-999);
}

Int_t antiNumuCC1PiMinusAnalysis::GetTopology2(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu) {
  //**************************************************
  
  /* Classify reaction types
     -1 = no true vertex
     0 = CC 0pi      = mu + X nucleons (+ 0 mesons)
     1 = CC 1pi+(-)  = mu + X nucleons + 1 pion +(-)
     2 = CC other
     3 = BKG: not (anti-)numu, NC
     7 = out of FV
   */
  
  // out of FGD1 FV
  if ( ! anaUtils::InFiducialVolume(det,trueVertex.Position)) return 7;
  
  /// primary particles from the true vertex
  Int_t Nmuon =     trueVertex.NPrimaryParticles[ParticleId::kMuon];
  Int_t Nantimuon = trueVertex.NPrimaryParticles[ParticleId::kAntiMuon];
  Int_t Nmeson =    trueVertex.NPrimaryParticles[ParticleId::kMesons];
  Int_t Npipos =    trueVertex.NPrimaryParticles[ParticleId::kPiPos];
  Int_t Npineg =    trueVertex.NPrimaryParticles[ParticleId::kPiNeg];

  // non numu CC, i.e. BKG
  // if ( ! IsAntinu && Nmuon <= 0) return BKG; // BKG
  //if (IsAntinu && Nantimuon <= 0) return BKG; // BKG
  if(!IsAntinu) {
    if(Nmuon == 0) 
      if(Nmeson == 1) {
	if(Npipos == 1) {
	  return 5;
	}
	else {
	  return 6;
	}
      }
      else if(Nmeson == 0) {
	return 4;
      }
      else {
	return 6;
      }      
  }
  else if(IsAntinu) {
    if(Nantimuon == 0) 
      if(Nmeson == 1) {
	if(Npineg == 1) {
	  return -5;
	}
	else {
	  return -6;
	}
      }
      else if(Nmeson == 0) {
	return -4;
      }
      else {
	return -6;
      }      
  }

  if(Nmuon == 1 && Nantimuon == 0) {
      if(Nmeson == 1) {
	if(Npipos == 1) {
	  return 2;
	}
	else {
	  return 3;
	}
      }
      else if(Nmeson == 0) {
	return 1;
      }
      else {
	return 3; 
      }  
  }
  else if(Nantimuon == 1 && Nmuon == 0) {
    if(Nmeson == 1) {
      if(Npineg == 1) {
	return -2;
      }
      else {
	return -3;
      }
      }
    else if(Nmeson == 0) {
      return -1;
    }
    else {
	return -3; 
    }
  }
  else {
    return 8;
  }

  return 14;
  std::cout<<"at the end but no return value\n";
} 

//********************************************************************
void antiNumuCC1PiMinusAnalysis::FillConfigTree(){
//********************************************************************

  /* 
     In this method we fill variables in the config tree. This is a single entry tree (event independent) where information about the 
     analysis being run is stored (enabled systematics, corrections and selections, description of the selection, info about the input file, etc). 
     All this is done automatically. However the user can add its own variables. Since this is a single entry tree, the same method can be used 
     to add and fill the variables
  */
  
  
  // Add and fill number of nucleons in each of the targets
  AddVarD(output(), nNucleonsFGD1,      "number of targets in FGD1");
  AddVarD(output(), nNucleonsFGD2scint, "number of targets in FGD2 scintillator");
  AddVarD(output(), nNucleonsFGD2water, "number of targets in FGD2 water");

  output().FillVar(nNucleonsFGD1,      anaUtils::GetNTargets(anaUtils::kFGD1));
  output().FillVar(nNucleonsFGD2scint, anaUtils::GetNTargets(anaUtils::kFGD2xymodules));
  output().FillVar(nNucleonsFGD2water, anaUtils::GetNTargets(anaUtils::kFGD2watermodules));
}

