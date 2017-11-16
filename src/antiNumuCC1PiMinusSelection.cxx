#include "antiNumuCC1PiMinusSelection.hxx"
#include "antiNumuCCSelection.hxx"
#include "baseSelection.hxx"
#include "CutUtils.hxx"
#include "SubDetId.hxx"
#include "EventBoxUtils.hxx"
#include "trackerSelectionUtils.hxx"

//********************************************************************
antiNumuCC1PiMinusSelection::antiNumuCC1PiMinusSelection(bool forceBreak): SelectionBase(forceBreak,EventBoxId::kEventBoxTracker) {
//********************************************************************

  _antiNumuCCSelection.Initialize();

  
  
}

//********************************************************************
void antiNumuCC1PiMinusSelection::DefineDetectorFV(){
//********************************************************************

  // Set the detector Fiducial Volume in which the selection is applied. This is now a mandatory method
  SetDetectorFV(SubDetId::kFGD1);
}

//********************************************************************
void antiNumuCC1PiMinusSelection::DefineSteps(){
//********************************************************************

  // Cuts must be added in the right order
  // last "true" means the step sequence is broken if cut is not passed (default is "false")
  //Cuts
  //Event quality cuts
  //0 - Event quality
  //Track cuts
  //1 - greater than 0 TPC tracks
  //2 - quality and fiducial
  //Multiplicity - 
  
  // This is mandatory. An alias should be given to each of the branches
  AddStep(StepBase::kCut,    "event quality",      new EventQualityCut(), true);
  AddStep(StepBase::kCut,    "> 0 tracks ", new TotalMultiplicityCut(),true);
  AddStep(StepBase::kAction, "find leading tracks",
	  new FindLeadingTracksAction_antinu());
  AddStep(StepBase::kAction, "find vertex",        new FindVertexAction());
  AddStep(StepBase::kAction, "fill summary",       new FillSummaryAction_antinu());
  AddStep(StepBase::kCut,    "quality+fiducial",   new TrackQualityFiducialCut(), true);
  //Vetos
  AddStep(StepBase::kAction, "find veto track",    new FindVetoTrackAction());
  AddStep(StepBase::kCut,    "veto",               new ExternalVetoCut());
  AddStep(StepBase::kAction, "find oofv track",    new FindOOFVTrackAction());
  AddStep(StepBase::kCut,    "External FGD1",      new ExternalFGD1lastlayersCut());
  //now let us add some cuts not from other analysis
  //is the  Highest mom track a pos track?
  AddStep(StepBase::kCut,    "pos_mult",           new PositiveMultiplicityCut());
  AddStep(StepBase::kCut,"Total neg. multiplicity", 
	  new TotalNegativeMultiplicityCut());
  //Pre-sel end
  AddStep(StepBase::kCut,    "Anti muon PID",      new AntiMuonPIDCutNew());
  //  AddStep(StepBase::kCut,    "Reject proton PID",      new ProtonPIDCut());
  AddStep(StepBase::kAction,"Find Pion(s)", new FindNegativePionTrackAction());
  AddStep(StepBase::kCut,"One Neg. Pion cut",new OneNegativePionCut());
  AddStep(StepBase::kCut,"Reject ECal uncontained mip like tracks",new RejectECalUncontainedMuon());
  
  AddStep(StepBase::kAction,"Find non-leading positive tracks", new FindPositiveOtherTrackAction());

  //  AddStep(StepBase::kCut, "No other positive meson/muon", new PositiveMuonMesonCut());
  SetBranchAlias(0,"trunk");
  
  // // By default the preselection correspond to cuts 0-2. 
  // // It means that if any of the three first cuts (0,1,2) is not passed 
  // // the loop over toys will be broken ===> A single toy will be run
  SetPreSelectionAccumLevel(0);
}


//********************************************************************
bool antiNumuCC1PiMinusSelection::FillEventSummary(AnaEventC& event, Int_t allCutsPassed[]){
//********************************************************************

  if(allCutsPassed[0]){
    static_cast<AnaEventSummaryB*>(event.Summary)->EventSample = SampleId::kFGD1NuMuCC;
  }
  return (static_cast<AnaEventSummaryB*>(event.Summary)->EventSample != SampleId::kUnassigned);
}




//**************************************************
void antiNumuCC1PiMinusSelection::InitializeEvent(AnaEventC& eventC){
//**************************************************

  // Cast the event
  AnaEventB& event = *static_cast<AnaEventB*>(&eventC); 

  // Create the appropriate EventBox if it does not exist yet
  if (!event.EventBoxes[EventBoxId::kEventBoxTracker])
    event.EventBoxes[EventBoxId::kEventBoxTracker] = new EventBoxTracker();

  // Fill all objects needed by the selection in the event box
  boxUtils::FillTracksWithTPC(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  // boxUtils::FillTracksWithFGD(event,             static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  // boxUtils::FillTrajsChargedInTPC(event);
  // boxUtils::FillTrajsChargedInFGDAndNoTPC(event, static_cast<SubDetId::SubDetEnum>(GetDetectorFV()));
  
}


bool RejectECalUncontainedMuon::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);

  //Float_t PIDLikelihood[4];
  //anaUtils::GetPIDLikelihood(*box.NegativePionTPCtracks[0], PIDLikelihood);
  if(box.NegativePionTPCtracks[0]) {
    if(box.NegativePionTPCtracks[0]->nECALSegments == 1) {
      //    box.NegativePionTPCtracks[0]->ECALSegments[0]->Containment == 0; //not contained
      AnaECALParticleB* ECALSegmentB = box.NegativePionTPCtracks[0]->ECALSegments[0];
      AnaECALParticle* ECALSegment = static_cast<AnaECALParticle*>(ECALSegmentB);
      if(ECALSegment->Containment == 0 && ECALSegment->PIDMipPion < 0.)
	return kFALSE;
    }
  }
  return kTRUE;
}

bool ProtonPIDCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  //**************************************************
  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);
  if(box.MainTrack) {
    if (anaUtils::GetPIDLikelihood(*box.MainTrack,2)>0.9)
      return false;
    else    
      return true;
  }
  else return false;
}

bool OneNegativePionCut::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);

  return (box.nNegativePionTPCtracks==1);
}

bool FindNegativePionTrackAction::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);

  if(box.HMNtrack)
    {
      //lets make sure the HMNTrack is pion like
      //and not an elecron
      //Taken from anti-numu CC MultiPi analysis
      Float_t PIDLikelihood[4];
      anaUtils::GetPIDLikelihood(*box.HMNtrack, PIDLikelihood);

      // For Positive tracks we distinguish pions, electrons and protons.
      double ElLklh = PIDLikelihood[1];  
      double PionLklh = PIDLikelihood[3];  
      double norm = ElLklh+PionLklh;
      ElLklh /= norm; 
      PionLklh /= norm;
      
      if( PionLklh > 0.3 ){ //was 0.8 but optimised cut value was found to be 0.3 // Id associated to the largest of the two probabilities.
	
	box.NegativePionTPCtracks[0]=box.HMNtrack;
	box.nNegativePionTPCtracks=1;
      }
    }
  return true;
}


bool AntiMuonPIDCutNew::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  //**************************************************

  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);

  Float_t cut = 0.18;

  Float_t PIDLikelihood[4];
  if(box.HMPtrack)
    anaUtils::GetPIDLikelihood(*box.HMPtrack, PIDLikelihood);
  else 
    return false;

  if (PIDLikelihood[0]>cut) {
    return true; 
  }

  return false;
}





//**************************************************
bool TotalNegativeMultiplicityCut::Apply(AnaEventC& event, ToyBoxB& boxB) const{
//**************************************************
//GetTrueParticle()->GParentPDG
  (void)event;
  ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);

    
  //There should be one, and only one negative track
  return (box.nNegativeTPCtracks == 1);
}

bool FindPositiveOtherTrackAction::Apply(AnaEventC& event, ToyBoxB& boxB) const {
  (void)event;
  ToyBoxAntiNumuCC1pi& box = *static_cast<ToyBoxAntiNumuCC1pi*>(&boxB);

  for(int ii=0;ii<box.nPositiveTPCtracks;ii++) {
    box.OtherTPCtracks[box.nOtherTPCtracks]=box.PositiveTPCtracks[ii];
    box.nOtherTPCtracks++;
  }

  return true;
}


// bool PositiveMuonMesonCut::Apply(AnaEventC& event, ToyBoxB& boxB) {

//   (void)event;  
//   ToyBoxTracker& box = *static_cast<ToyBoxTracker*>(&boxB);
//   // Pulls in order: Muon, Electron, Proton, Pion
//   Float_t PIDLikelihood[4];
//   //loop over positive tracks exce-pt for the HMPtrack
  
//   anaUtils::GetPIDLikelihood(*ptrack, PIDLikelihood);

//   // For Positive tracks we distinguish pions, electrons and protons.
//   double ElLklh = PIDLikelihood[1];  
//   double ProtonLklh = PIDLikelihood[2];  
//   double PionLklh = PIDLikelihood[3];  
//   double norm = ElLklh+ProtonLklh+PionLklh;
//   ProtonLklh /= norm; 
//   ElLklh /= norm; 
//   PionLklh /= norm; 

//   if( ProtonLklh > ElLklh && ProtonLklh > PionLklh ) continue; // If the highest probability is a proton continue. 

//   // Id associated to the largest of the two probabilities.
//   if( PionLklh > ElLklh ){
//     cc4piMultiPibox->PositivePionTPCtracks[cc4piMultiPibox->nPositivePionTPCtracks] = ptrack;
//     cc4piMultiPibox->nPositivePionTPCtracks++;
//   }
//   else {
//     if( ptrack->Momentum > 900. ) continue; // This is mainly protons.
//     cc4piMultiPibox->PosPi0TPCtracks[cc4piMultiPibox->nPosPi0TPCtracks] = ptrack; 
//     cc4piMultiPibox->nPosPi0TPCtracks++;
//   }
// }
