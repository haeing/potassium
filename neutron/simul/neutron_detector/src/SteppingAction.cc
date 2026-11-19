#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

using namespace std;;
                           

SteppingAction::SteppingAction()
{ }

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  const auto event = G4RunManager::GetRunManager()->GetCurrentEvent();
  const auto runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  if (runID != fRunID || event != fEvent || event->GetEventID() != fEventID) {
    fEvent = event;
    fEventID = event->GetEventID();
    fRunID = runID;
    fEscapedNeutrons.clear();
  }
  const auto track = aStep->GetTrack();
  if (track->GetDefinition()->GetParticleName() != "neutron") return;
  const auto birthVolume = track->GetLogicalVolumeAtVertex();
  if (!birthVolume || birthVolume->GetName() != "Am-Be Source") return;
  const auto pre = aStep->GetPreStepPoint()->GetPhysicalVolume();
  const auto post = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if (!pre || pre->GetName() != "Am-Be source" ||
      (post && post->GetName() == "Am-Be source")) return;
  if (!fEscapedNeutrons.insert(track->GetTrackID()).second) return;
  auto analysis = G4AnalysisManager::Instance();
  analysis->FillNtupleDColumn(1, 0,
      aStep->GetPostStepPoint()->GetKineticEnergy()/MeV);
  analysis->AddNtupleRow(1);
}
