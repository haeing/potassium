
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "G4RunManager.hh"

extern G4int reactNum;
extern G4int Totnumber;
G4int TotalReact = 0;
G4int TotnumberEvt = 0;

using std::cout;
using std::endl;


RunAction::RunAction(EventAction* eventAction)
:fEventAction(eventAction)
{
   // cout<<"Start runaction initialization"<<endl;
  // Create the generic analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
     // If the filename extension is not provided, the default file type (root)
     // will be used for all files specified without extension.
  analysisManager->SetVerboseLevel(1);

  // Default settings
  analysisManager->SetNtupleMerging(false);
     // Note: merging ntuples is available only with Root output
  analysisManager->SetFileName("Neutron");

  // Book histograms, ntuple


  // Creating ntuple
  if ( fEventAction ) {
    analysisManager->CreateNtuple("tree", "Hits");
    analysisManager->CreateNtupleDColumn("TotalEdep");  // column Id = 0
    analysisManager->CreateNtupleIColumn("secNum"); // column Id = 1

    analysisManager->CreateNtupleDColumn("ElectronEdep"); //column Id = 2
    analysisManager->CreateNtupleDColumn("PositronEdep"); //column Id = 3
    analysisManager->CreateNtupleDColumn("ProtonEdep"); //column Id = 4
    analysisManager->CreateNtupleDColumn("C12Edep"); //column Id = 5
    analysisManager->CreateNtupleDColumn("NucleusEdep"); //column Id = 6
    //analysisManager->CreateNtupleDColumn("test");

    analysisManager->FinishNtuple();
  }

  // Set ntuple output file
  analysisManager->SetNtupleFileName(0, "./Test250211.root");

  cout<<"Set the root file "<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  cout<<"run start"<<endl;
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  cout<<"instanciate analysismanager"<<endl;

  // Reset histograms from previous run
  analysisManager->Reset();
  cout<<"reset the histogram"<<endl;

  // Open an output file
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();

  cout<<"open the foot file"<<endl;
  cout<<"Begin of run process is done"<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  // save histograms & ntuple
  //
  cout<<"test reaction number : "<<reactNum<<" test total number : "<<Totnumber<<endl;
  cout<<"test total reaction number : "<<TotalReact<<" test total event number : "<<TotnumberEvt<<endl;

  cout<<"end of run process start"<<endl;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();

  cout<< "write the root file"<<endl;

  analysisManager->CloseFile(false);
  cout<<"close the root file"<<endl;
  const auto number = run -> GetNumberOfEvent();
  cout<<"number of Event : "<<number<<endl;

  cout<<"run is done"<<endl;
    // Keep content of histos so that they are plotted.
    // The content will be reset at start of the next run.
}
