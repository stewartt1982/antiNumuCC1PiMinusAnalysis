#ifndef antiNumuCC1PiMinusSelection_h
#define antiNumuCC1PiMinusSelection_h

#include "antiNumuCCSelection.hxx"
#include "SelectionBase.hxx"
#include "Parameters.hxx"
#include "ToyBoxTracker.hxx"
#include "baseTrackerAnalysis.hxx"

/* In this file there is an example of selection with a single branch. This is identical to the numuCCSelection (under psycheSelections) 
   used by the numuCCAnalysis in highland2.
   Two define a selection you need in this file:
   - If a specific box is needed (the ToyBoxB is not sufficient) define it first such that it is known when the Selection is defined
   - Declare the selection inheriting from SelectionBase or another existing selection
   - Declare all steps needed by the selection
*/

/*  CHANGES IN NEW VERSION

*/


//---- Define the class for the new selection, which should inherit from SelectionBase or from another existing selection -------
class antiNumuCC1PiMinusSelection: public SelectionBase {
 public:
  antiNumuCC1PiMinusSelection(bool forceBreak=true);
  virtual ~antiNumuCC1PiMinusSelection(){}


  ///========= These are mandatory functions ==================

  /// In this method all steps are added to the selection
  void DefineSteps();
  
  /// Set detector FV
  void DefineDetectorFV();

  /// Create a proper instance of the box (ToyBoxB) to store all relevant 
  /// information to be passed from one step to the next
  //ToyBoxB* MakeToyBox() {return new ToyBoxTracker();}
  inline ToyBoxB* MakeToyBox();

  /// Fill the EventBox with the objects needed by this selection
  void InitializeEvent(AnaEventC& event);

  //==========================================================


  //---- These are optional functions, needed by FITTERS but not by highland2 analyses --------------

  bool FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]);
  SampleId::SampleEnum GetSampleEnum(){return SampleId::kFGD1NuMuCC;}


protected:
  antiNumuCCSelection _antiNumuCCSelection;
  /*  
  //---- These are optional functions, but recommended to customize systematics and increase speed --------------

  bool IsRelevantRecObjectForSystematic(const AnaEventC&, AnaTrackB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantTrueObjectForSystematic(const AnaEventC&, AnaTrueParticleB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantRecObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrackB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantTrueObjectForSystematicInToy(const AnaEventC&, const ToyBoxB&, AnaTrueParticleB*, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool IsRelevantSystematic(const AnaEventC&, const ToyBoxB&, Int_t syst_index, Int_t branch=0) const {(void)syst_index;(void)branch;return true;}
  bool CheckRedoSelection(const AnaEventB&, const ToyBoxB& PreviousToyBox, Int_t& redoFromStep){(void)PreviousToyBox;redoFromStep=0;return true;}

  */
};

class ToyBoxAntiNumuCC1pi: public ToyBoxTracker {
public:
  ToyBoxAntiNumuCC1pi() {
    nNegativePionTPCtracks = 0;
    nOtherTPCtracks = 0;
    anaUtils::ClearArray(NegativePionTPCtracks, 100);
    anaUtils::ClearArray(OtherTPCtracks, 100);
  }

  virtual ~ToyBoxAntiNumuCC1pi() {}

  void Reset() {
    anaUtils::ClearArray(NegativePionTPCtracks, nNegativePionTPCtracks);
    anaUtils::ClearArray(OtherTPCtracks, nOtherTPCtracks);
    nNegativePionTPCtracks = 0;
    nOtherTPCtracks = 0;
  }
  // negative TPC pion candidates 
  AnaTrackB* NegativePionTPCtracks[100];
  int nNegativePionTPCtracks;

  //other tracks not associated with the pion (other meason, baryons, leptons)
  //will be positive due to the cut on number of negative tracks
  AnaTrackB* OtherTPCtracks[100];
  int nOtherTPCtracks;
};

inline ToyBoxB* antiNumuCC1PiMinusSelection::MakeToyBox(){return new ToyBoxAntiNumuCC1pi();}

class ProtonPIDCut:       public StepBase {
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new ProtonPIDCut();}
};


class RejectECalUncontainedMuon: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new RejectECalUncontainedMuon();}
};


class AntiMuonPIDCutNew: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new AntiMuonPIDCutNew();}
};

class OneNegativePionCut: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new OneNegativePionCut();}
};

 
class TotalNegativeMultiplicityCut: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new TotalNegativeMultiplicityCut();}
};

class FindPositiveOtherTrackAction: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindPositiveOtherTrackAction();}
};

class FindNegativePionTrackAction: public StepBase {
public:
  using StepBase::Apply;
  bool Apply(AnaEventC& event, ToyBoxB& box) const;
  StepBase* MakeClone(){return new FindNegativePionTrackAction();}
};


// ///---- Define all steps -------
// class FillSummaryAction_tutorial: public StepBase{
// public:
//   using StepBase::Apply;
//   bool Apply(AnaEventC& event, ToyBoxB& box) const;  
//   StepBase* MakeClone(){return new FillSummaryAction_tutorial();}
// };




#endif
