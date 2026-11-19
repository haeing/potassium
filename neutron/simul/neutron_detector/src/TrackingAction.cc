#include "TrackingAction.hh"

#include "EventAction.hh"
#include "TrackingMessenger.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4HadronicProcess.hh"
#include "G4Nucleus.hh"

#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(EventAction* event)
:fEvent(event)
 
{
  fTrackMessenger = new TrackingMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
  delete fTrackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::SetTimeWindow(G4double t1, G4double dt)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  auto particle = track -> GetDefinition() ->GetParticleName();
  auto pretrackKE = track -> GetKineticEnergy();

  if(particle == "neutron")
  {
    auto CreatorProcess = track -> GetCreatorProcess();
    if(!(CreatorProcess))
    {
      cout<<"there are no creator process !"<<endl;
      return;
    }
    auto ProcessName = CreatorProcess -> GetProcessName();    
  
    if(ProcessName == "alphaInelastic")
    {
      fEvent -> TotalKineticEnergy(pretrackKE);
    }
    


    //
    // Checking Weird Energy Neutron
    // if(pretrackKE > 6 || pretrackKE <9)
    // {
    //   auto CreatorProcess = track -> GetCreatorProcess();
    //   if(!(CreatorProcess))
    //   {
    //     cout<<"there are no creator process !"<<endl;
    //     return;
    //   }

    //   auto ProcessName = CreatorProcess -> GetProcessName();
    //   cout<<endl;
    //   cout<<"This weird energy come from : "<<ProcessName<<endl;

    //   //
    //   // Checking Target with Weird Neutron 

    //   G4int Z = 0;
    //   G4int A = 0;
    //   const auto hadronicProcess = dynamic_cast<const G4HadronicProcess*>(CreatorProcess);

    //   if (!hadronicProcess) 
    //   {
    //     cout << "Error: hadronicProcess is nullptr" << endl;
    //   }

    //   const G4Nucleus* targetNucleus = hadronicProcess -> GetTargetNucleus();

    //   if (!targetNucleus) 
    //   {
    //       cout << "Warning: targetNucleus is nullptr" << endl;
    //   }
    //   if(targetNucleus)
    //   {

    //    Z = targetNucleus -> GetZ_asInt();
    //    A = targetNucleus -> GetA_asInt();

    //   cout << "Neutron reacted with nucleus (Z= " << Z << ", A=" << A << ")" << endl;
    //   cout<<endl;

    //   //
    //   // Saving into text file as log
    //   ofstream out("test.txt",ios::app);
    //   out<<"This weird energy come from : "<<ProcessName<<endl;

    //   out<<"Target for neutron Inelastic : A = "<<A<<", Z = "<<Z<<endl;
    //   out<<endl;

    //   out.close();
    // }
    // }

  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{


}
