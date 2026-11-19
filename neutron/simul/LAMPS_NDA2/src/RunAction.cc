
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "G4RunManager.hh"

#include "TFile.h"
#include "TTree.h"

#include "EventData.hh"

using namespace std;

RunAction::RunAction(EventAction* eventAction)
:fEventAction(eventAction)
{
  // auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetDefaultFileType("root");
  // analysisManager->SetVerboseLevel(0);
  // analysisManager->SetNtupleMerging(false);
  // analysisManager->SetFileName("Neutron");


  // Creating ntuple
  // if ( fEventAction ) {
  //   analysisManager->CreateNtuple("tree", "Hits");

  //   //
  //   // Whole NDA
  //   analysisManager->CreateNtupleDColumn("Edep", fEventAction -> GetNDAEdep());  // column Id = 0

  //   analysisManager->FinishNtuple();

  // }

  // Set ntuple output file
  // analysisManager->SetNtupleFileName(0, "./LAMPS_NDA2.root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  // auto analysisManager = G4AnalysisManager::Instance();

  // analysisManager->Reset();
  // analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  // auto analysisManager = G4AnalysisManager::Instance();
  // analysisManager->Write();

  // analysisManager->CloseFile(false);
}