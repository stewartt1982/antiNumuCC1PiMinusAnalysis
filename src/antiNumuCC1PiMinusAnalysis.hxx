#ifndef antiNumuCC1PiMinusAnalysis_h
#define antiNumuCC1PiMinusAnalysis_h

#include "baseTrackerAnalysis.hxx"
#include "ND280AnalysisUtils.hxx"
#include "antiNumuCCAnalysis.hxx"
#include "antiNumuCC1PiMinusSelection.hxx"

/// Inherit from baseTrackerAnalysis, which inherit from basAnslysis, which inherits from AnalysisAlgorithm (in highlandCore)
class antiNumuCC1PiMinusAnalysis: public baseTrackerAnalysis {
 public:
  antiNumuCC1PiMinusAnalysis(AnalysisAlgorithm* ana=NULL);
  virtual ~antiNumuCC1PiMinusAnalysis(){}

  //---- These are mandatory functions (pure virtual, =0, in AnalysisAlgorithm) ---

  // However most of them were already implemented in baseAnalysis and baseTracker analysis
  // Some of them are extend it here and some others are not   

  bool Initialize();

  //  void DefineProductions();     // No need to extend it
  //  void DefineInputConverters(); // no need to extend it

  /// Add to the selection manager the selections for this analysis
  void DefineSelections();
  void DefineCorrections();
  void DefineSystematics();
  void DefineConfigurations();
  void DefineMicroTrees(bool addBase=true);
  void DefineTruthTree();

  void FillMicroTrees(bool addBase=true);
  void FillToyVarsInMicroTrees(bool addBase=true);

  bool CheckFillTruthTree(const AnaTrueVertex& vtx);

  //  using baseTrackerAnalysis::FillTruthTree;
  void FillTruthTree(const AnaTrueVertex& vtx);

  const ToyBoxAntiNumuCC1pi& mybox(){return *static_cast<const ToyBoxAntiNumuCC1pi*>(&box());}
  //--------------------
  

  void FillConfigTree();
  void FillCategories();

  Int_t GetTopology2(const AnaTrueVertex& trueVertex, const SubDetId::SubDetEnum det, bool IsAntinu);

protected:
  antiNumuCCAnalysis* _antiNumuCCAnalysis; 

public:

  /* Here we create an enum with all variables we want to add to the trees, starting at the last variable it was added by used analyses or base analyses, 
     in this case the last of the nueCCAnalysis
  */

  enum enumStandardMicroTrees_antiNumuCC1PiMinusAnalysis{
    selmu_pdg = antiNumuCCAnalysis::enumStandardMicroTreesLast_antiNumuCCAnalysis+1,
    selmu_likemip,
    selmu_likepi,
    selmu_likep,
    selmu_likeele,

    selpi_pdg,
    //pion variables
    //copies of selmu variables from numuCCAnalysis
    selpi_truemom,
    selpi_nuErecQE,
    selpi_closest_tpc,
    selpi_detectors,
    selpi_truepos,
    selpi_trueendpos,
    selpi_truedir,
    selpi_dir,
    selpi_enddir,
    selpi_pos,
    selpi_endpos,
    selpi_det,
    selpi_ntpcs,
    selpi_tpc_det,
    selpi_tpc_nhits,
    selpi_tpc_nnodes,
    selpi_tpc_charge,
    selpi_tpc_mom,
    selpi_tpc_bfield_mom,
    selpi_tpc_efield_mom,
    selpi_tpc_emom,
    selpi_tpc_truemom,
    selpi_tpc_dedx_raw,
    selpi_tpc_dedx_expmu,
    selpi_tpc_dedx_exppi,
    selpi_tpc_dedx_expele,
    selpi_tpc_dedx_expp,
    selpi_nfgds,
    selpi_fgd_det,
    selpi_fgd_x,
    selpi_fgd_V11,
    selpi_fgd_V33,
    selpi_fgd_V55,
    selpi_fgd_V77,
    selpi_fgd_VLayer,
    selpi_fgd_pullmu,

    selpi_necals,
    selpi_ecal_det,

    selpi_ecal_nhits,
    selpi_ecal_nnodes,
    selpi_ecal_length,
    selpi_ecal_showerstartpos,
    selpi_ecal_showerendpos,
    selpi_ecal_showerstartdir,
    selpi_ecal_showerenddir,

    selpi_ecal_EMenergy,
    selpi_ecal_edeposit,
    selpi_ecal_IsShower,

    selpi_ecal_mipem,
    selpi_ecal_mippion,
    selpi_ecal_emhip,
    selpi_ecal_containment,
    selpi_ecal_trshval,
    selpi_ecal_mostupstreamlayerhit,

    // SMRD vars
    selpi_nsmrds,
    selpi_smrd_det,
    selpi_smrd_edeposit,
    selpi_smrd_nhits,
    selpi_smrd_nnodes,
    selpi_smrd_dir,
    selpi_smrd_enddir,

    selpi_mom,
    selpi_costheta,
    selpi_amom,
    selpi_likemip,
    selpi_likemu,
    selpi_tpc_pullmu,
    selpi_tpc_pullpi,
    selpi_tpc_pullele,
    selpi_tpc_pullp,
    selpi_tpc_dedx,
    selpi_likepi,
    selpi_likep,
    selpi_likeele,

    selrealmu_pdg,
    //pion variables
    //copies of selmu variables from numuCCAnalysis
    selrealmu_truemom,
    selrealmu_nuErecQE,
    selrealmu_closest_tpc,
    selrealmu_detectors,
    selrealmu_truepos,
    selrealmu_trueendpos,
    selrealmu_truedir,
    selrealmu_dir,
    selrealmu_enddir,
    selrealmu_pos,
    selrealmu_endpos,
    selrealmu_det,
    selrealmu_ntpcs,
    selrealmu_tpc_det,
    selrealmu_tpc_nhits,
    selrealmu_tpc_nnodes,
    selrealmu_tpc_charge,
    selrealmu_tpc_mom,
    selrealmu_tpc_bfield_mom,
    selrealmu_tpc_efield_mom,
    selrealmu_tpc_emom,
    selrealmu_tpc_truemom,
    selrealmu_tpc_dedx_raw,
    selrealmu_tpc_dedx_expmu,
    selrealmu_tpc_dedx_exppi,
    selrealmu_tpc_dedx_expele,
    selrealmu_tpc_dedx_expp,
    selrealmu_nfgds,
    selrealmu_fgd_det,
    selrealmu_fgd_x,
    selrealmu_fgd_V11,
    selrealmu_fgd_V33,
    selrealmu_fgd_V55,
    selrealmu_fgd_V77,
    selrealmu_fgd_VLayer,
    selrealmu_fgd_pullmu,

    selrealmu_necals,
    selrealmu_ecal_det,

    selrealmu_ecal_nhits,
    selrealmu_ecal_nnodes,
    selrealmu_ecal_length,
    selrealmu_ecal_showerstartpos,
    selrealmu_ecal_showerendpos,
    selrealmu_ecal_showerstartdir,
    selrealmu_ecal_showerenddir,

    selrealmu_ecal_EMenergy,
    selrealmu_ecal_edeposit,
    selrealmu_ecal_IsShower,

    selrealmu_ecal_mipem,
    selrealmu_ecal_mippion,
    selrealmu_ecal_emhip,
    selrealmu_ecal_containment,
    selrealmu_ecal_trshval,
    selrealmu_ecal_mostupstreamlayerhit,

    // SMRD vars
    selrealmu_nsmrds,
    selrealmu_smrd_det,
    selrealmu_smrd_edeposit,
    selrealmu_smrd_nhits,
    selrealmu_smrd_nnodes,
    selrealmu_smrd_dir,
    selrealmu_smrd_enddir,

    selrealmu_mom,
    selrealmu_costheta,
    selrealmu_amom,
    selrealmu_likemip,
    selrealmu_likemu,
    selrealmu_tpc_pullmu,
    selrealmu_tpc_pullpi,
    selrealmu_tpc_pullele,
    selrealmu_tpc_pullp,
    selrealmu_tpc_dedx,
    selrealmu_likepi,
    truepion_mom,
    trueproton_mom,
    enumStandardMicroTreesLast_antiNumuCC1PiMinusAnalysis
  };

  // Here we create an enum with all systematics we want to add starting at the last systematic it was added by used analyses or base analyses, 
  enum enumSyst_antiNumuCC1PiMinusAnalysis{
    kTutorialWeight = SystId::SystEnumLast_SystId+1,
    kTutorialVariation,
    enumSystLast_antiNumuCC1PiMinusAnalysis
  };

  // Here we create an enum with all configurations we want to add starting at the last configuration it was added by used analyses or base analyses, 
  enum enumConf_antiNumuCC1PiMinusAnalysis{
    tutorial_syst=enumConfLast_baseTrackerAnalysis+1,
    enumConfLast_bantiNumuCC1PiMinusAnalysis
  };

  // Here we create an enum with all variables we want to add to the config tree starting with the last it wad added by used analyses or base analyses, 
  enum enumConfigTree_antiNumuCC1PiMinusAnalysis{
    nNucleonsFGD1 = enumConfigTreeLast_AnalysisAlgorithm+1,
    nNucleonsFGD2scint,
    nNucleonsFGD2water,
    enumConfigTreeLast_antiNumuCC1PiMinusAnalysis
  };
  
};

#endif
