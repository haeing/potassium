#include "EventAction.hh"
#include "NeutronHit.hh"

//#include "Constants.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

G4int reactNum = 0;
G4int Totnumber = 0;
extern G4int TotalReact;
extern G4int TotnumberEvt;

using std::array;
using std::vector;
using std::cout;
using std::endl;

G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
      G4ExceptionDescription msg;
      msg << "No hits collection of this event found." << G4endl;
      G4Exception("EventAction::EndOfEventAction()",
                  "Code001", JustWarning, msg);
      return nullptr;
  }

  auto hc = hce->GetHC(collId);
  if ( ! hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
  }
  return hc;
}
EventAction::EventAction()
{}


void EventAction::BeginOfEventAction(const G4Event*)
{
    //cout<<"one event start"<<endl;
    auto sdManager = G4SDManager::GetSDMpointer();
    NeutronHCID = sdManager->GetCollectionID("neutrondetector/NeutronCounter");
    //cout<<"Neutron HCID : "<<NeutronHCID<<endl;
}


void EventAction::EndOfEventAction(const G4Event* event)
{
   cout<<"end of event process start : "<<event -> GetEventID()<<endl;

auto analysisManager = G4AnalysisManager::Instance();
    
auto hc = GetHC(event, NeutronHCID);
if(!hc) return;

auto test = hc -> GetSize();
//  cout<< "Step Size : "<< test << endl;
if (event->GetEventID()%1000==0)
  cout<<" Event ID : "<<event -> GetEventID()<<endl;

G4double initKen = 0.;
G4double finKen = 0.;
G4double Kdep = 0.;
G4double TotKdep =0.;
G4int number = 0;

G4int ID = -1;
G4int id = -1;
G4double globalEdep = 0;

//

G4double globalElectron = 0;
G4double globalPositron = 0;
G4double globalProton = 0;
G4double globalC12 = 0;
G4double globalNucleus = 0;

for(int i =0; i<test; i++)
{

  G4double edep = 0;

  G4double ElectronEdep = 0;
  G4double PositronEdep = 0;
  G4double ProtonEdep = 0;
  G4double C12Edep = 0;
  G4double NucleusEdep = 0;

  auto hit = static_cast<NeutronHit*>(hc->GetHit(i));

  initKen = hit -> GetInKEn();
  finKen = hit -> GetFnKEn();
  number = hit -> GetNum();
   edep = hit->GetEdep();
// cout<<"energy deposit : "<<edep<<endl;
  id = hit -> GetID();

   ElectronEdep = hit -> GetElectronEdep();
   PositronEdep = hit -> GetPositronEdep();
   ProtonEdep = hit -> GetProtonEdep();
   C12Edep = hit -> GetCarbon12Edep();
   NucleusEdep = hit -> GetNucleusEdep();

  // cout<<endl;
  // cout<<"Total Edep : "<<edep<<endl;
  // cout<<"Electron Edep : "<<ElectronEdep<<endl;
  // cout<<"Positron Edep : "<<PositronEdep<<endl;
  // cout<<"Proton Edep : "<<ProtonEdep<<endl;
  // cout<<"C12 Edep : "<<C12Edep<<endl;
  // cout<<"Other Nucleus Edep : "<<NucleusEdep<<endl;
  // cout<<endl;

  globalEdep+= edep;
  Totnumber += number;

  globalElectron += ElectronEdep;
  globalPositron += PositronEdep;
  globalProton += ProtonEdep;
  globalC12 += C12Edep;
  globalNucleus += NucleusEdep;

}
  cout<<endl;
  cout<<"Sum of Total Edep : "<<globalEdep<<endl;
  cout<<"Sum of Electron Edep : "<<globalElectron<<endl;
  cout<<"Sum of Positron Edep : "<<globalPositron<<endl;
  cout<<"Sum of Proton Edep : "<<globalProton<<endl;
  cout<<"Sum of C12 Edep : "<<globalC12<<endl;
  cout<<"Sum of Other Nucleus Edep : "<<globalNucleus<<endl;
  cout<<"Sum of Other Edep : "<<globalElectron + globalPositron + globalProton + globalC12 + globalNucleus<<endl;
  cout<<endl;

analysisManager -> FillNtupleDColumn(0, globalEdep);

analysisManager -> FillNtupleDColumn(2, globalElectron);
analysisManager -> FillNtupleDColumn(3, globalPositron);
analysisManager -> FillNtupleDColumn(4, globalProton);
analysisManager -> FillNtupleDColumn(5, globalC12);
analysisManager -> FillNtupleDColumn(6, globalNucleus);

// cout<<"total energy deposit : "<<globalEdep<<endl;
//cout<<"Total number : "<<Totnumber<<endl;

if(Totnumber != 0)
{
  reactNum = 1;
}
else
{
  reactNum = 0;
}

TotalReact += reactNum ;
TotnumberEvt += Totnumber;

analysisManager->AddNtupleRow();

//cout<<"one event is done"<<endl;
}
