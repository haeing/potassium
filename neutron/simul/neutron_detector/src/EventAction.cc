#include "EventAction.hh"

#include "NeutronHit01.hh"
#include "NeutronHit02.hh"

// #include "Constants.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"

// G4int reactNum = 0;
// G4int Totnumber = 0;
// extern G4int globalhIoni; 
// extern G4int globaleIoni;
// extern G4int globalhadElastic;
// extern G4int globalnCapture;
// extern G4int globalCompt;
// extern G4int globalConv;
// extern G4int globaleBrem;
// extern G4int globalphot;
// extern G4int globalannihil;
// extern G4int globalnInelastic;
// extern G4int globalOthers;

// extern G4int globalCnt_elec;
// extern G4int globalCnt_posi;
// extern G4int globalCnt_pro;
// extern G4int globalCnt_C12;
// extern G4int globalCnt_Other;

// //
// //

// extern G4int globalOhIoni;
// extern G4int globaleOIoni;
// extern G4int globalOhadElastic;
// extern G4int globalOnCapture;
// extern G4int globalOcompt ;
// extern G4int globalOconv;
// extern G4int globalOeBrem ;
// extern G4int globalOphot;
// extern G4int globalneutronInelastic;
// extern G4int globalOther;

// extern G4int globalahIoni;
// extern G4int globalaeIoni;
// extern G4int globalahadElastic;
// extern G4int globalanCapture;
// extern G4int globalacompt ;
// extern G4int globalaconv;
// extern G4int globalaeBrem ;
// extern G4int globalaphot;
// extern G4int globalaneutronInelastic;
// extern G4int globalaOther;

// extern G4int globalphIoni;
// extern G4int globalpeIoni;
// extern G4int globalphadElastic;
// extern G4int globalpnCapture;
// extern G4int globalpcompt ;
// extern G4int globalpconv;
// extern G4int globalpeBrem ;
// extern G4int globalpphot;
// extern G4int globalpneutronInelastic;
// extern G4int globalpOther;

// extern G4int globalC13hIoni;
// extern G4int globalC13eIoni;
// extern G4int globalC13hadElastic;
// extern G4int globalC13nCapture; 
// extern G4int globalC13compt ;
// extern G4int globalC13conv;
// extern G4int globalC13eBrem ;
// extern G4int globalC13phot;
// extern G4int globalC13neutronInelastic;
// extern G4int globalC13Other;

// extern G4int globalC12hIoni;
// extern G4int globalC12eIoni;
// extern G4int globalC12hadElastic;
// extern G4int globalC12nCapture;
// extern G4int globalC12compt ;
// extern G4int globalC12conv;
// extern G4int globalC12eBrem ;
// extern G4int globalC12phot;
// extern G4int globalC12neutronInelastic;
// extern G4int globalC12Other;

// extern G4int globalghIoni;
// extern G4int globalgeIoni;
// extern G4int globalghadElastic;
// extern G4int globalgnCapture; 
// extern G4int globalgcompt ;
// extern G4int globalgconv;
// extern G4int globalgeBrem ;
// extern G4int globalgphot;
// extern G4int globalgneutronInelastic ;
// extern G4int globalgOther ;

// extern G4int globaldhIoni;
// extern G4int globaldeIoni;
// extern G4int globaldhadElastic;
// extern G4int globaldnCapture; 
// extern G4int globaldcompt ;
// extern G4int globaldconv;
// extern G4int globaldeBrem ;
// extern G4int globaldphot;
// extern G4int globaldneutronInelastic ;
// extern G4int globaldOther ;

// extern G4int globalposihIoni;
// extern G4int globalposieIoni;
// extern G4int globalposihadElastic;
// extern G4int globalposinCapture ;
// extern G4int globalposicompt ;
// extern G4int globalposiconv;
// extern G4int globalposieBrem ;
// extern G4int globalposiphot;
// extern G4int globalposineutronInelastic ;
// extern G4int globalposiOther;

// extern G4int globalehIoni;
// extern G4int globaleeIoni;
// extern G4int globalehadElastic;
// extern G4int globalenCapture; 
// extern G4int globalecompt ;
// extern G4int globaleconv;
// extern G4int globaleeBrem ;
// extern G4int globalephot;
// extern G4int globaleneutronInelastic;
// extern G4int globaleOther;

// extern G4int globalOannihil;
// extern G4int globalaannihil;
// extern G4int globalpannihil;
// extern G4int globalC13annihil;
// extern G4int globalC12annihil;
// extern G4int globalgannihil;
// extern G4int globaldannihil;
// extern G4int globalposiannihil;
// extern G4int globaleannihil;

using namespace std;

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
  if (!hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
  }
  return hc;
}
EventAction::EventAction() {}

void EventAction::TotalKineticEnergy(G4double KE)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(5, KE);
}

void EventAction::BeginOfEventAction(const G4Event*) {

  auto sdManager = G4SDManager::GetSDMpointer();
  NeutronHCID01 = sdManager->GetCollectionID("neutrondetector01/NeutronCounter01");
  NeutronHCID02 = sdManager->GetCollectionID("neutrondetector02/NeutronCounter02");
  // cout<<"Neutron HCID : "<<NeutronHCID<<endl;
  
}

void EventAction::EndOfEventAction(const G4Event* event) {

  auto analysisManager = G4AnalysisManager::Instance();

  //
  // This is for Neutron Detector 01
  auto hc01 = GetHC(event, NeutronHCID01);
  if (!hc01)
    return;

  auto test = hc01->GetSize();
  if (event->GetEventID() % 100000 == 0)
    cout << " Event ID : " << event->GetEventID() << endl;

  G4double initKen = 0.;
  G4double finKen = 0.;
  G4double Kdep = 0.;
  G4double TotKdep = 0.;
  G4int number = 0;

  vector<int> ID(1);
  vector<int>::iterator it;
  G4int id = -1;
  G4int pid = -1;
  G4String Pname = "non";
  vector<double> globalEdep(1);
  vector<G4int> parentID(1);
  vector<G4String> ParticleName(1);
  vector<double> protonEdep(1);
  vector<double> CarbonEdep(1);
  G4double NeutronKineticEnergy = 0. *MeV;
  G4double nKE = 0.;

  for (int i = 0; i < test; i++) {
    G4double edep = 0;
    // G4double proton_edep = 0.;
    // G4double carbon_edep = 0.;

    auto hit = static_cast<NeutronHit01*>(hc01->GetHit(i));

    initKen = hit->GetInKEn();
    finKen = hit->GetFnKEn();
    number = hit->GetNum();
    edep = hit->GetEdep();
    nKE = hit -> GetNeutronKineticEnergy();
    NeutronKineticEnergy += nKE;
    
    id = hit->GetID();
    pid = hit->GetParentID();
    Pname = hit->GetPName();

    // proton_edep = hit -> GetProtonEdep();
    // carbon_edep = hit -> GetCarbon12Edep();

  it = find(ID.begin(), ID.end(), id);

  if(it == ID.end())
  {
    ID.push_back(id);
    globalEdep.push_back(edep);
    parentID.push_back(pid);
    ParticleName.push_back(Pname);

    // protonEdep.push_back(proton_edep);
    // CarbonEdep.push_back(carbon_edep);
  }
  else
  {
    int n = it - ID.begin();
    globalEdep[n] += edep;

    // protonEdep[n] += proton_edep;
    // CarbonEdep[n] += carbon_edep;

  }
  }
  G4double TotalEdep = 0 *MeV;
  G4double NeutronEdep = 0 *MeV;
  G4double TotalProtonEdep = 0. *MeV;
  G4double TotalCarbonEdep = 0. *MeV;

  for(int i=0; i<globalEdep.size()-1; i++)
  {
    if(globalEdep[i+1] > 133 *eV)
    {
      TotalEdep += globalEdep[i+1];
    }
  }
  

//
// Extraction Energy Depostion from Neutron
// cout<<"size of ID : "<<ID.size()<<endl;
// cout<<"size of Edep : "<<globalEdep.size()<<endl;
// cout<<"size of parent ID : "<<parentID.size()<<endl;
// cout<<"size of particle name : "<<ParticleName.size()<<endl;
// for(int i=0; i<ID.size(); i++)
// {
//   cout<<"ID["<<i<<"] : "<<ID[i]<<endl;
//   cout<<"parent ID["<<i<<"] : "<<parentID[i]<<endl;
//   cout<<"particle name["<<i<<"] : "<<ParticleName[i]<<endl;
//   cout<<"energy deposition["<<i<<"] : "<<globalEdep[i]<<endl;
// }
vector<G4String>::iterator neutron_it;
vector<G4String>::iterator proton_it;
vector<G4String>::iterator c12_it;
vector<G4String>::iterator c13_it;

vector<G4String>::iterator iterator = ParticleName.begin();
vector<vector<G4String>::iterator> proton_it_vector(1);
vector<vector<G4String>::iterator> c12_it_vector(1);
vector<vector<G4String>::iterator> c13_it_vector(1);

for(int k = 0; k<ID.size()-1; k++)
{

neutron_it = find(iterator, ParticleName.end(),"neutron");
if(neutron_it == ParticleName.end())
{
  break;
}

//
//For Neutron Total Edep
if(neutron_it != ParticleName.end())
{
  
G4int n = neutron_it - ParticleName.begin();
// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
for(int i=0; i<ID.size()-1; i++)
{
  
  if((ID[n] == parentID[i]) && (globalEdep[i] > 133 *eV))
  {

    NeutronEdep += globalEdep[i];
    
  }
}
 
}

//
//For Proton Edep

if((proton_it != ParticleName.end()) && (neutron_it != ParticleName.end()))
{
  
  
G4int n = neutron_it - ParticleName.begin();

// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
for(int i=0; i<ID.size() -1; i++)
{
  proton_it = find(iterator, ParticleName.end(), "proton");
  
  // cout<<"1st : "<<*proton_it <<endl;
  // cout<<"2nd : "<<*ParticleName.end() <<endl;
  if(proton_it == ParticleName.end())
  {
    break;
  }
  
  if(proton_it != ParticleName.end())
  {
    G4int p = proton_it - ParticleName.begin();
    
  }
  G4int p = proton_it - ParticleName.begin();

  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  if((ID[n] == parentID[p]) && (globalEdep[p] > 133 *eV))
  {
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalProtonEdep += globalEdep[p];
  }
  iterator = proton_it +1;
}
 
}

iterator = ParticleName.begin();

//
//For Carbon Edep
if(((c12_it != ParticleName.end())||(c13_it != ParticleName.end()))&&(neutron_it != ParticleName.end()))
{
  
G4int n = neutron_it - ParticleName.begin();

// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;

//
//C12
for(int i=0; i<ID.size() -1; i++)
{
  c12_it = find(iterator, ParticleName.end(), "C12");
  
  if(c12_it == ParticleName.end())
  {
    break;
  }
  G4int c12;
  if(c12_it != ParticleName.end())
  {
    // cout<<"c12 index : "<<c12_it - ParticleName.begin()<<endl;
    // cout<<"Is this really c12? : "<<ParticleName[c12_it - ParticleName.begin()]<<endl;
    
    c12 = c12_it - ParticleName.begin();
    

  }
  
  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  if((ID[n] == parentID[c12]) && (globalEdep[c12] > 133 *eV))
  {
  
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalCarbonEdep += globalEdep[c12];
  }

  iterator = c12_it +1;
}

iterator = ParticleName.begin();

//
//C13
for(int i=0; i<ID.size() -1; i++)
{
  c13_it = find(iterator, ParticleName.end(), "C13");
  
  if(c13_it == ParticleName.end())
  {
    break;
  }
  
  G4int c13;
  if(c13_it != ParticleName.end())
  {
    
    c13 = c13_it - ParticleName.begin();
    
  }

  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  // cout<<"global edep : "<<globalEdep[c13]<<endl;
  if((ID[n] == parentID[c13]) && (globalEdep[c13] > 133 *eV))
  {
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalCarbonEdep += globalEdep[c13];
  }

  iterator = c13_it +1;
}
 
}

iterator = neutron_it +1;
}

  //
  // This is for Neutron Detector 02
  auto hc02 = GetHC(event, NeutronHCID02);
  if (!hc02)
    return;

  auto test02 = hc02->GetSize();

  G4double initKen02 = 0.;
  G4double finKen02 = 0.;
  G4double Kdep02 = 0.;
  G4double TotKdep02 = 0.;
  G4int number02 = 0;

  vector<int> ID02(1);
  vector<int>::iterator it02;
  G4int id02 = -1;
  G4int pid02 = -1;
  G4String Pname02 = "non";
  vector<double> globalEdep02(1);
  vector<G4int> parentID02(1);
  vector<G4String> ParticleName02(1);
  vector<double> protonEdep02(1);
  vector<double> CarbonEdep02(1);
  G4double NeutronKineticEnergy02 = 0. *MeV;
  G4double nKE02 = 0.;

  for (int i = 0; i < test02; i++) {
    G4double edep02 = 0;
    // G4double proton_edep = 0.;
    // G4double carbon_edep = 0.;

    auto hit02 = static_cast<NeutronHit02*>(hc02->GetHit(i));

    initKen02 = hit02->GetInKEn();
    finKen02 = hit02->GetFnKEn();
    number02 = hit02->GetNum();
    edep02 = hit02->GetEdep();
    nKE02 = hit02 -> GetNeutronKineticEnergy();
    NeutronKineticEnergy02 += nKE02;
    
    id02 = hit02->GetID();
    pid02 = hit02->GetParentID();
    Pname02 = hit02->GetPName();

    // proton_edep = hit -> GetProtonEdep();
    // carbon_edep = hit -> GetCarbon12Edep();

  it02 = find(ID02.begin(), ID02.end(), id02);

  if(it02 == ID02.end())
  {
    ID02.push_back(id02);
    globalEdep02.push_back(edep02);
    parentID02.push_back(pid02);
    ParticleName02.push_back(Pname02);

    // protonEdep.push_back(proton_edep);
    // CarbonEdep.push_back(carbon_edep);
  }
  else
  {
    int n02 = it02 - ID02.begin();
    globalEdep02[n02] += edep02;

    // protonEdep[n] += proton_edep;
    // CarbonEdep[n] += carbon_edep;

  }
  }
  G4double TotalEdep02 = 0 *MeV;
  G4double NeutronEdep02 = 0 *MeV;
  G4double TotalProtonEdep02 = 0. *MeV;
  G4double TotalCarbonEdep02 = 0. *MeV;

  for(int i=0; i<globalEdep02.size()-1; i++)
  {
    if(globalEdep02[i+1] > 133 *eV)
    {
      TotalEdep02 += globalEdep02[i+1];
    }
  }
  

//
// Extraction Energy Depostion from Neutron
// cout<<"size of ID : "<<ID.size()<<endl;
// cout<<"size of Edep : "<<globalEdep.size()<<endl;
// cout<<"size of parent ID : "<<parentID.size()<<endl;
// cout<<"size of particle name : "<<ParticleName.size()<<endl;
// for(int i=0; i<ID.size(); i++)
// {
//   cout<<"ID["<<i<<"] : "<<ID[i]<<endl;
//   cout<<"parent ID["<<i<<"] : "<<parentID[i]<<endl;
//   cout<<"particle name["<<i<<"] : "<<ParticleName[i]<<endl;
//   cout<<"energy deposition["<<i<<"] : "<<globalEdep[i]<<endl;
// }
vector<G4String>::iterator neutron_it02;
vector<G4String>::iterator proton_it02;
vector<G4String>::iterator c12_it02;
vector<G4String>::iterator c13_it02;

vector<G4String>::iterator iterator02 = ParticleName02.begin();
vector<vector<G4String>::iterator> proton_it_vector02(1);
vector<vector<G4String>::iterator> c12_it_vector02(1);
vector<vector<G4String>::iterator> c13_it_vector02(1);

for(int k = 0; k<ID02.size() -1; k++)
{

neutron_it02 = find(iterator02, ParticleName02.end(),"neutron");
if(neutron_it02 == ParticleName02.end())
{
  break;
}

//
//For Neutron Total Edep
if(neutron_it02 != ParticleName02.end())
{
  
G4int n02 = neutron_it02 - ParticleName02.begin();
// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
for(int i=0; i<ID02.size() -1; i++)
{
  
  if((ID02[n02] == parentID02[i]) && (globalEdep02[i] > 133 *eV))
  {

    NeutronEdep02 += globalEdep02[i];
    
  }
}
 
}

//
//For Proton Edep

if((proton_it02 != ParticleName02.end()) && (neutron_it02 != ParticleName02.end()))
{
  
  
G4int n02 = neutron_it02 - ParticleName02.begin();

// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
for(int i=0; i<ID02.size() -1; i++)
{
  proton_it02 = find(iterator02, ParticleName02.end(), "proton");
  
  // cout<<"1st : "<<*proton_it <<endl;
  // cout<<"2nd : "<<*ParticleName.end() <<endl;
  if(proton_it02 == ParticleName02.end())
  {
    break;
  }
  
  if(proton_it02 != ParticleName02.end())
  {
    G4int p02 = proton_it02 - ParticleName02.begin();
    
  }
  G4int p02 = proton_it02 - ParticleName02.begin();

  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  if((ID02[n02] == parentID[p02]) && (globalEdep02[p02] > 133 *eV))
  {
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalProtonEdep02 += globalEdep02[p02];
  }
  iterator02 = proton_it02 +1;
}
 
}

iterator02 = ParticleName02.begin();

//
//For Carbon Edep
if(((c12_it02 != ParticleName02.end())||(c13_it02 != ParticleName02.end()))&&(neutron_it02 != ParticleName02.end()))
{
  
G4int n02 = neutron_it02 - ParticleName02.begin();

// for(int i=0; i<ParticleName.size(); i++)
// {
//   cout<<"particle name : "<<ParticleName[i]<<endl;
// }

// cout<<"where is neutron : "<<n<<endl;

// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;

//
//C12
for(int i=0; i<ID02.size() -1; i++)
{
  c12_it02 = find(iterator02, ParticleName02.end(), "C12");
  
  if(c12_it02 == ParticleName02.end())
  {
    break;
  }
  G4int c1202;
  if(c12_it02 != ParticleName02.end())
  {
    // cout<<"c12 index : "<<c12_it - ParticleName.begin()<<endl;
    // cout<<"Is this really c12? : "<<ParticleName[c12_it - ParticleName.begin()]<<endl;
    
    c1202 = c12_it02 - ParticleName02.begin();
    

  }
  
  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  if((ID02[n02] == parentID02[c1202]) && (globalEdep02[c1202] > 133 *eV))
  {
  
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalCarbonEdep02 += globalEdep02[c1202];
  }

  iterator02 = c12_it02 +1;
}

iterator02 = ParticleName02.begin();

//
//C13
for(int i=0; i<ID02.size() -1; i++)
{
  c13_it02 = find(iterator02, ParticleName02.end(), "C13");
  
  if(c13_it02 == ParticleName02.end())
  {
    break;
  }
  
  G4int c1302;
  if(c13_it02 != ParticleName02.end())
  {
    
    c1302 = c13_it02 - ParticleName02.begin();
    
  }

  // cout<<"id check : "<<ID[n]<<endl;
  // cout<<"parent check : "<<parentID[i]<<endl;
  // cout<<"number of loop : "<<i<<endl;
  // cout<<"global edep : "<<globalEdep[c13]<<endl;
  if((ID02[n02] == parentID02[c1302]) && (globalEdep02[c1302] > 133 *eV))
  {
    // cout<<"daughter particle is "<<ParticleName[i]<<endl;
    // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

    TotalCarbonEdep02 += globalEdep02[c1302];
  }

  iterator02 = c13_it02 +1;
}
 
}

iterator02 = neutron_it02 +1;
}

//
//Neutron Detector 01
if(TotalEdep/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(0, TotalEdep/MeV);
}

  // analysisManager->FillNtupleDColumn(2, globalElectron);
  // analysisManager->FillNtupleDColumn(3, globalPositron);
if(TotalProtonEdep/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(1, TotalProtonEdep/MeV);
}

if(TotalCarbonEdep/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(2, TotalCarbonEdep/MeV);
}

if(NeutronEdep/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(3, NeutronEdep/MeV);
}
  // analysisManager->FillNtupleDColumn(6, globalNucleus);

if(NeutronKineticEnergy/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(4, NeutronKineticEnergy/MeV);
}
  


//
//Neutron Detector 02

if(TotalEdep02/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(6, TotalEdep02/MeV);
}

if(TotalProtonEdep02/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(7, TotalProtonEdep02/MeV);
}

if(TotalCarbonEdep02/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(8, TotalCarbonEdep02/MeV);
}
  // analysisManager->FillNtupleDColumn(2, globalElectron);
  // analysisManager->FillNtupleDColumn(3, globalPositron);
  // analysisManager->FillNtupleDColumn(6, globalNucleus);

if(NeutronEdep02/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(9, NeutronEdep02/MeV);
}

if(NeutronKineticEnergy02/MeV > 133 *eV)
{
  analysisManager->FillNtupleDColumn(10, NeutronKineticEnergy02/MeV);
}

  analysisManager->AddNtupleRow();
  
}

//
//This one is setting for global variable
//And Energy for all particle from each reaction just in case
//



// void EventAction::EndOfEventAction(const G4Event* event) {
//   cout << "event end 01" << endl;

//   auto analysisManager = G4AnalysisManager::Instance();

//   auto hc = GetHC(event, NeutronHCID);
//   if (!hc)
//     return;

//   auto test = hc->GetSize();
//   cout<<"test value : "<<test<<endl;
//   //  cout<< "Step Size : "<< test << endl;
//   if (event->GetEventID() % 1000 == 0)
//     cout << " Event ID : " << event->GetEventID() << endl;

//   G4double initKen = 0.;
//   G4double finKen = 0.;
//   G4double Kdep = 0.;
//   G4double TotKdep = 0.;
//   G4int number = 0;

// cout<<"hello event01"<<endl;
//   vector<int> ID(1);
//   vector<int>::iterator it;
//   G4int id = -1;
//   G4int pid = -1;
//   G4String Pname = "non";
//   vector<double> globalEdep(1);
//   vector<G4int> parentID(1);
//   vector<G4String> ParticleName(1);

//   cout<<"hello event02"<<endl;
//   cout<<"hello event09"<<endl;

//   //

//   G4double globalElectron = 0;
//   G4double globalPositron = 0;
//   G4double globalProton = 0;
//   G4double globalC12 = 0;
//   G4double globalNucleus = 0;

//   G4int TothIoni = 0;
//   G4int ToteIoni = 0;
//   G4int TothadElastic = 0;
//   G4int TotnCapture = 0;
//   G4int TotCompt = 0;
//   G4int TotConv = 0;
//   G4int ToteBrem = 0;
//   G4int Totphot = 0;
//   G4int Totannihil = 0;
//   G4int TotnInelastic = 0;
//   G4int TotOthers = 0;

//   G4int TotCnt_elec = 0;
//   G4int TotCnt_posi = 0;
//   G4int TotCnt_pro = 0;
//   G4int TotCnt_C12 = 0;
//   G4int TotCnt_Other = 0;
// cout<<"hello event10"<<endl;
//   //
//   //

//   G4int SumhIoni = 0.;
//   G4int SumeIoni = 0.;
//   G4int SumhadElastic = 0.;
//   G4int SumnCapture = 0.;
//   G4int Sumcompt = 0.;
//   G4int Sumconv = 0.;
//   G4int SumeBrem = 0.;
//   G4int Sumphot = 0.;
//   G4int SumneutronInelastic = 0.;
//   G4int SumOther = 0.;

//   G4int TotahIoni = 0.;
//   G4int TotaeIoni = 0.;
//   G4int TotahadElastic = 0.;
//   G4int TotanCapture = 0.;
//   G4int Totacompt = 0.;
//   G4int Totaconv = 0.;
//   G4int TotaeBrem = 0.;
//   G4int Totaphot = 0.;
//   G4int TotaneutronInelastic = 0.;
//   G4int TotaOther = 0.;

//   G4int TotphIoni = 0.;
//   G4int TotpeIoni = 0.;
//   G4int TotphadElastic = 0.;
//   G4int TotpnCapture = 0.;
//   G4int Totpcompt = 0.;
//   G4int Totpconv = 0.;
//   G4int TotpeBrem = 0.;
//   G4int Totpphot = 0.;
//   G4int TotpneutronInelastic = 0.;
//   G4int TotpOther = 0.;

//   G4int TotC13hIoni = 0.;
//   G4int TotC13eIoni = 0.;
//   G4int TotC13hadElastic = 0.;
//   G4int TotC13nCapture = 0.;
//   G4int TotC13compt = 0.;
//   G4int TotC13conv = 0.;
//   G4int TotC13eBrem = 0.;
//   G4int TotC13phot = 0.;
//   G4int TotC13neutronInelastic = 0.;
//   G4int TotC13Other = 0.;

//   G4int TotC12hIoni = 0.;
//   G4int TotC12eIoni = 0.;
//   G4int TotC12hadElastic = 0.;
//   G4int TotC12nCapture = 0.;
//   G4int TotC12compt = 0.;
//   G4int TotC12conv = 0.;
//   G4int TotC12eBrem = 0.;
//   G4int TotC12phot = 0.;
//   G4int TotC12neutronInelastic = 0.;
//   G4int TotC12Other = 0.;

//   G4int TotghIoni = 0.;
//   G4int TotgeIoni = 0.;
//   G4int TotghadElastic = 0.;
//   G4int TotgnCapture = 0.;
//   G4int Totgcompt = 0.;
//   G4int Totgconv = 0.;
//   G4int TotgeBrem = 0.;
//   G4int Totgphot = 0.;
//   G4int TotgneutronInelastic = 0.;
//   G4int TotgOther = 0.;

//   G4int TotdhIoni = 0.;
//   G4int TotdeIoni = 0.;
//   G4int TotdhadElastic = 0.;
//   G4int TotdnCapture = 0.;
//   G4int Totdcompt = 0.;
//   G4int Totdconv = 0.;
//   G4int TotdeBrem = 0.;
//   G4int Totdphot = 0.;
//   G4int TotdneutronInelastic = 0.;
//   G4int TotdOther = 0.;

//   G4int TotposihIoni = 0.;
//   G4int TotposieIoni = 0.;
//   G4int TotposihadElastic = 0.;
//   G4int TotposinCapture = 0.;
//   G4int Totposicompt = 0.;
//   G4int Totposiconv = 0.;
//   G4int TotposieBrem = 0.;
//   G4int Totposiphot = 0.;
//   G4int TotposineutronInelastic = 0.;
//   G4int TotposiOther = 0.;

//   G4int TotehIoni = 0.;
//   G4int ToteeIoni = 0.;
//   G4int TotehadElastic = 0.;
//   G4int TotenCapture = 0.;
//   G4int Totecompt = 0.;
//   G4int Toteconv = 0.;
//   G4int ToteeBrem = 0.;
//   G4int Totephot = 0.;
//   G4int ToteneutronInelastic = 0.;
//   G4int ToteOther = 0.;

//   G4int TotOannihil = 0.;
//   G4int Totaannihil = 0.;
//   G4int Totpannihil = 0.;
//   G4int TotC13annihil = 0.;
//   G4int TotC12annihil = 0.;
//   G4int Totgannihil = 0.;
//   G4int Totdannihil = 0.;
//   G4int Totposiannihil = 0.;
//   G4int Toteannihil = 0.;
// cout<<"hello event 11"<<endl;
//   for (int i = 0; i < test; i++) {
//     cout<<"hello event 12"<<endl;
//     G4double edep = 0;

//     G4double ElectronEdep = 0;
//     G4double PositronEdep = 0;
//     G4double ProtonEdep = 0;
//     G4double C12Edep = 0;
//     G4double NucleusEdep = 0;

//     G4int hIoni = 0;
//     G4int eIoni = 0;
//     G4int hadElastic = 0;
//     G4int nCapture = 0;
//     G4int compt = 0;
//     G4int conv = 0;
//     G4int eBrem = 0;
//     G4int phot = 0;
//     G4int annihil = 0;
//     G4int nInelastic = 0;
//     G4int Others = 0;

//     G4int Cnt_elec = 0;
//     G4int Cnt_posi = 0;
//     G4int Cnt_pro = 0;
//     G4int Cnt_C12 = 0;
//     G4int Cnt_Other = 0;

//     //
//     //

//   G4int RhIoni = 0.;
//   G4int ReIoni = 0.;
//   G4int RhadElastic = 0.;
//   G4int RnCapture = 0.;
//   G4int Rcompt = 0.;
//   G4int Rconv = 0.;
//   G4int ReBrem = 0.;
//   G4int Rphot = 0.;
//   G4int RneutronInelastic = 0.;
//   G4int RROther = 0.;

//   G4int ahIoni = 0.;
//   G4int aeIoni = 0.;
//   G4int ahadElastic = 0.;
//   G4int anCapture = 0.;
//   G4int acompt = 0.;
//   G4int aconv = 0.;
//   G4int aeBrem = 0.;
//   G4int aphot = 0.;
//   G4int aneutronInelastic = 0.;
//   G4int aOther = 0.;

//   G4int phIoni = 0.;
//   G4int peIoni = 0.;
//   G4int phadElastic = 0.;
//   G4int pnCapture = 0.;
//   G4int pcompt = 0.;
//   G4int pconv = 0.;
//   G4int peBrem = 0.;
//   G4int pphot = 0.;
//   G4int pneutronInelastic = 0.;
//   G4int pOther = 0.;

//   G4int C13hIoni = 0.;
//   G4int C13eIoni = 0.;
//   G4int C13hadElastic = 0.;
//   G4int C13nCapture = 0.;
//   G4int C13compt = 0.;
//   G4int C13conv = 0.;
//   G4int C13eBrem = 0.;
//   G4int C13phot = 0.;
//   G4int C13neutronInelastic = 0.;
//   G4int C13Other = 0.;

//   G4int C12hIoni = 0.;
//   G4int C12eIoni = 0.;
//   G4int C12hadElastic = 0.;
//   G4int C12nCapture = 0.;
//   G4int C12compt = 0.;
//   G4int C12conv = 0.;
//   G4int C12eBrem = 0.;
//   G4int C12phot = 0.;
//   G4int C12neutronInelastic = 0.;
//   G4int C12Other = 0.;

//   G4int ghIoni = 0.;
//   G4int geIoni = 0.;
//   G4int ghadElastic = 0.;
//   G4int gnCapture = 0.;
//   G4int gcompt = 0.;
//   G4int gconv = 0.;
//   G4int geBrem = 0.;
//   G4int gphot = 0.;
//   G4int gneutronInelastic = 0.;
//   G4int gOther = 0.;

//   G4int dhIoni = 0.;
//   G4int deIoni = 0.;
//   G4int dhadElastic = 0.;
//   G4int dnCapture = 0.;
//   G4int dcompt = 0.;
//   G4int dconv = 0.;
//   G4int deBrem = 0.;
//   G4int dphot = 0.;
//   G4int dneutronInelastic = 0.;
//   G4int dOther = 0.;

//   G4int posihIoni = 0.;
//   G4int posieIoni = 0.;
//   G4int posihadElastic = 0.;
//   G4int posinCapture = 0.;
//   G4int posicompt = 0.;
//   G4int posiconv = 0.;
//   G4int posieBrem = 0.;
//   G4int posiphot = 0.;
//   G4int posineutronInelastic = 0.;
//   G4int posiOther = 0.;

//   G4int ehIoni = 0.;
//   G4int eeIoni = 0.;
//   G4int ehadElastic = 0.;
//   G4int enCapture = 0.;
//   G4int ecompt = 0.;
//   G4int econv = 0.;
//   G4int eeBrem = 0.;
//   G4int ephot = 0.;
//   G4int eneutronInelastic = 0.;
//   G4int eOther = 0.;

//   G4int Oannihil = 0.;
//   G4int aannihil = 0.;
//   G4int pannihil = 0.;
//   G4int C13annihil = 0.;
//   G4int C12annihil = 0.;
//   G4int gannihil = 0.;
//   G4int dannihil = 0.;
//   G4int posiannihil = 0.;
//   G4int eannihil = 0.;
// cout<<"hello event03"<<endl;
//     auto hit = static_cast<NeutronHit*>(hc->GetHit(i));
// cout<<"hello event04"<<endl;
//     initKen = hit->GetInKEn();
//     finKen = hit->GetFnKEn();
//     number = hit->GetNum();
//     edep = hit->GetEdep();
//     cout<<"hello event05"<<endl;
//     id = hit->GetID();
//     cout<<"hello event06"<<endl;
//     pid = hit->GetParentID();
//     cout<<"hello event07"<<endl;
//     Pname = hit->GetPName();

//     cout<<"hello event08"<<endl;


//     ElectronEdep = hit->GetElectronEdep();
//     PositronEdep = hit->GetPositronEdep();
//     ProtonEdep = hit->GetProtonEdep();
//     C12Edep = hit->GetCarbon12Edep();
//     NucleusEdep = hit->GetNucleusEdep();

//     hIoni = hit -> GetNhIoni();
//     eIoni = hit -> GetNeIoni();
//     hadElastic = hit -> GetNhadElastic();
//     nCapture = hit -> GetNnCapture();
//     compt = hit -> GetNCompt();
//     conv = hit -> GetNConv();
//     eBrem = hit -> GetNeBrem();
//     phot = hit -> GetNphot();
//     annihil = hit -> GetNannihil();
//     nInelastic = hit -> GetNnInelastic();
//     Others = hit -> GetNReaction();

//     Cnt_elec = hit -> GetNnInelaCount01();
//     Cnt_posi = hit -> GetNnInelaCount02();
//     Cnt_pro = hit -> GetNnInelaCount03();
//     Cnt_C12 = hit -> GetNnInelaCount04();
//     Cnt_Other = hit -> GetNnInelaCount05();

//     //
//     //
//   RhIoni = hit -> GethIoni();
//   ReIoni = hit -> GeteIoni();
//   RhadElastic = hit -> GethadElastic();
//   RnCapture = hit -> GetnCapture();
//   Rcompt = hit -> Getcompt();
//   Rconv = hit -> Getconv();
//   ReBrem = hit -> GeteBrem();
//   Rphot = hit -> Getphot();
//   RneutronInelastic = hit -> GetneutronInelastic();
//   RROther = hit -> GetOther();

//   ahIoni = hit -> GetahIoni();
//   aeIoni = hit -> GetaeIoni();
//   ahadElastic = hit ->GetahadElastic();
//   anCapture = hit -> GetanCapture();
//   acompt = hit -> Getacompt();
//   aconv = hit -> Getaconv();
//   aeBrem = hit -> GetaeBrem();
//   aphot = hit -> Getaphot();
//   aneutronInelastic = hit -> GetaneutronInelastic();
//   aOther = hit -> GetaOther();

//   phIoni = hit -> GetphIoni();
//   peIoni = hit -> GetpeIoni();
//   phadElastic = hit -> GetphadElastic();
//   pnCapture = hit -> GetpnCapture();
//   pcompt = hit -> Getpcompt();
//   pconv = hit -> Getpconv();
//   peBrem = hit ->GetpeBrem();
//   pphot = hit ->Getpphot();
//   pneutronInelastic = hit -> GetpneutronInelastic();
//   pOther = hit -> GetpOther();

//   C13hIoni = hit -> GetC13hIoni();
//   C13eIoni = hit -> GetC13eIoni();
//   C13hadElastic = hit -> GetC13hadElastic();
//   C13nCapture = hit -> GetC13nCapture();
//   C13compt = hit -> GetC13compt();
//   C13conv = hit -> GetC13conv();
//   C13eBrem = hit -> GetC13eBrem();
//   C13phot = hit -> GetC13phot();
//   C13neutronInelastic = hit -> GetC13neutronInelastic();
//   C13Other = hit -> GetC13Other();

//   C12hIoni = hit -> GetC12hIoni();
//   C12eIoni = hit -> GetC12eIoni();
//   C12hadElastic = hit -> GetC12hadElastic();
//   C12nCapture = hit -> GetC12nCapture();
//   C12compt = hit -> GetC12compt();
//   C12conv = hit -> GetC12conv();
//   C12eBrem = hit -> GetC12eBrem();
//   C12phot = hit -> GetC12phot();
//   C12neutronInelastic = hit -> GetC12neutronInelastic();
//   C12Other = hit -> GetC12Other();

//   ghIoni = hit -> GetghIoni();
//   geIoni = hit -> GetgeIoni();
//   ghadElastic = hit -> GetghadElastic();
//   gnCapture = hit -> GetgnCapture();
//   gcompt = hit -> Getgcompt();
//   gconv = hit -> Getgconv();
//   geBrem = hit -> GetgeBrem();
//   gphot = hit -> Getgphot();
//   gneutronInelastic = hit -> GetgneutronInelastic();
//   gOther = hit -> GetgOther();

//   dhIoni = hit -> GetdhIoni();
//   deIoni = hit -> GetdeIoni();
//   dhadElastic = hit -> GetdhadElastic();
//   dnCapture = hit -> GetdnCapture();
//   dcompt = hit -> Getdcompt();
//   dconv = hit -> Getdconv();
//   deBrem = hit -> GetdeBrem();
//   dphot = hit -> Getdphot();
//   dneutronInelastic = hit -> GetdneutronInelastic();
//   dOther = hit -> GetdOther();

//   posihIoni = hit -> GetposihIoni();
//   posieIoni = hit -> GetposieIoni();
//   posihadElastic = hit -> GetposihadElastic();
//   posinCapture = hit -> GetposinCapture();
//   posicompt = hit -> Getposicompt();
//   posiconv = hit -> Getposiconv();
//   posieBrem = hit -> GetposieBrem();
//   posiphot = hit -> Getposiphot();
//   posineutronInelastic = hit -> GetposineutronInelastic();
//   posiOther = hit -> GetposiOther();

//   ehIoni = hit -> GetehIoni();
//   eeIoni = hit -> GeteneutronInelastic();
//   ehadElastic = hit -> GetehadElastic();
//   enCapture = hit -> GetenCapture();
//   ecompt = hit -> Getecompt();
//   econv = hit -> Geteconv();
//   eeBrem = hit -> GeteeBrem();
//   ephot = hit -> Getephot();
//   eneutronInelastic = hit -> GeteneutronInelastic();
//   eOther = hit -> GeteOther();

//   Oannihil = hit -> Getannihil();
//   aannihil =hit -> Getaannihil();
//   pannihil = hit -> Getpannihil();
//   C13annihil = hit -> GetC13annihil();
//   C12annihil = hit -> GetC12annihil();
//   gannihil = hit -> Getgannihil();
//   dannihil = hit -> Getdannihil();
//   posiannihil = hit -> Getposiannihil();
//   eannihil = hit -> Geteannihil();

//     // cout<<endl;
//     // cout<<"Total Edep : "<<edep<<endl;
//     // cout<<"Electron Edep : "<<ElectronEdep<<endl;
//     // cout<<"Positron Edep : "<<PositronEdep<<endl;
//     // cout<<"Proton Edep : "<<ProtonEdep<<endl;
//     // cout<<"C12 Edep : "<<C12Edep<<endl;
//     // cout<<"Other Nucleus Edep : "<<NucleusEdep<<endl;
//     // cout<<endl;

//   it = find(ID.begin(), ID.end(), id);
//   if(it == ID.end())
//   {
//     ID.push_back(id);
//     globalEdep.push_back(edep);
//     parentID.push_back(pid);
//     ParticleName.push_back(Pname);
//   }
//   else
//   {
//     int n = it - ID.begin();
//     globalEdep[n] += edep;

//   }

// cout<<"hello event04"<<endl;
//     Totnumber += number;

//     globalElectron += ElectronEdep;
//     globalPositron += PositronEdep;
//     globalProton += ProtonEdep;
//     globalC12 += C12Edep;
//     globalNucleus += NucleusEdep;

//     TothIoni += hIoni;
//     ToteIoni += eIoni;
//     TothadElastic += hadElastic;
//     TotnCapture += nCapture;
//     TotCompt += compt;
//     TotConv += conv;
//     ToteBrem += eBrem;
//     Totphot += phot;
//     Totannihil += annihil;
//     TotnInelastic += nInelastic;
//     TotOthers += Others;

//     TotCnt_elec += Cnt_elec;
//     TotCnt_posi += Cnt_posi;
//     TotCnt_pro += Cnt_pro;
//     TotCnt_C12 += Cnt_C12;
//     TotCnt_Other += Cnt_Other;

//   //
//   //

// SumhIoni += RhIoni;
// SumeIoni += ReIoni;
// SumhadElastic += RhadElastic;
// SumnCapture += RnCapture;
// Sumcompt += Rcompt;
// Sumconv += Rconv;
// SumeBrem += ReBrem;
// Sumphot += Rphot;
// SumneutronInelastic += RneutronInelastic;
// SumOther += RROther; 

// TotahIoni += ahIoni;
// TotaeIoni += aeIoni;
// TotahadElastic += ahadElastic;
// TotanCapture += anCapture;
// Totacompt += acompt;
// Totaconv += aconv;
// TotaeBrem += aeBrem;
// Totaphot += aphot;
// TotaneutronInelastic += aneutronInelastic;
// TotaOther += aOther; 

// TotphIoni += phIoni;
// TotpeIoni += peIoni;
// TotphadElastic += phadElastic;
// TotpnCapture += pnCapture;
// Totpcompt += pcompt;
// Totpconv += pconv;
// TotpeBrem += peBrem;
// Totpphot += pphot;
// TotpneutronInelastic += pneutronInelastic;
// TotpOther += pOther; 

// TotC13hIoni += C13hIoni;
// TotC13eIoni += C13eIoni;
// TotC13hadElastic += C13hadElastic;
// TotC13nCapture += C13nCapture;
// TotC13compt += C13compt;
// TotC13conv += C13conv;
// TotC13eBrem += C13eBrem;
// TotC13phot += C13phot;
// TotC13neutronInelastic += C13neutronInelastic;
// TotC13Other += C13Other;

// TotC12hIoni += C12hIoni;
// TotC12eIoni += C12eIoni;
// TotC12hadElastic += C12hadElastic;
// TotC12nCapture += C12nCapture;
// TotC12compt += C12compt;
// TotC12conv += C12conv;
// TotC12eBrem += C12eBrem;
// TotC12phot += C12phot;
// TotC12neutronInelastic += C12neutronInelastic;
// TotC12Other += C12Other ;

// TotghIoni += ghIoni;
// TotgeIoni += geIoni;
// TotghadElastic += ghadElastic;
// TotgnCapture += gnCapture;
// Totgcompt += gcompt;
// Totgconv += gconv;
// TotgeBrem += geBrem;
// Totgphot += gphot;
// TotgneutronInelastic += gneutronInelastic;
// TotgOther += gOther ;

// TotdhIoni += dhIoni;
// TotdeIoni += deIoni;
// TotdhadElastic += dhadElastic;
// TotdnCapture += dnCapture;
// Totdcompt += dcompt;
// Totdconv += dconv;
// TotdeBrem += deBrem;
// Totdphot += dphot;
// TotdneutronInelastic += dneutronInelastic;
// TotdOther += dOther ;

// TotposihIoni += posihIoni;
// TotposieIoni += posieIoni;
// TotposihadElastic += posihadElastic;
// TotposinCapture += posinCapture;
// Totposicompt += posicompt;
// Totposiconv += posiconv;
// TotposieBrem += posieBrem;
// Totposiphot += posiphot;
// TotposineutronInelastic += posineutronInelastic;
// TotposiOther += posiOther ;

// TotehIoni += ehIoni;
// ToteeIoni += eeIoni;
// TotehadElastic += ehadElastic;
// TotenCapture += enCapture;
// Totecompt += ecompt;
// Toteconv += econv;
// ToteeBrem += eeBrem;
// Totephot += ephot;
// ToteneutronInelastic += eneutronInelastic;
// ToteOther += eOther;

// TotOannihil += Oannihil;
// Totaannihil += aannihil;
// Totpannihil += pannihil;
// TotC13annihil += C13annihil;
// TotC12annihil += C12annihil;
// Totgannihil += gannihil;
// Totdannihil += dannihil;
// Totposiannihil += posiannihil;
// Toteannihil += eannihil;

//   }
//   // cout << endl;
//   // cout << "Sum of Total Edep : " << globalEdep << endl;
//   // cout << "Sum of Electron Edep : " << globalElectron << endl;
//   // cout << "Sum of Positron Edep : " << globalPositron << endl;
//   // cout << "Sum of Proton Edep : " << globalProton << endl;
//   // cout << "Sum of C12 Edep : " << globalC12 << endl;
//   // cout << "Sum of Other Nucleus Edep : " << globalNucleus << endl;
//   // cout << "Sum of Other Edep : " << globalElectron + globalPositron + globalProton + globalC12 + globalNucleus << endl;
//   // cout << endl;
// cout<<"hello even 12"<<endl;
//   G4double TotalEdep = 0;
//   G4double NeutronEdep = 0;
//   // cout<<"number of track id : "<<globalEdep.size()<<endl;
//   // cout<<endl;

//   for(int i=0; i<globalEdep.size()-1; i++)
//   {
//     if(globalEdep[i+1] > 133 *eV)
//     {
//       // cout<<"energy : "<<globalEdep[i+1]<<endl;
//       TotalEdep += globalEdep[i+1];
//     }
//   }
//   // cout<<endl;
//   cout<<"hello event13"<<endl;

// //
// // Extraction Energy Depostion from Neutron
// cout<<"size of ID : "<<ID.size()<<endl;
// cout<<"size of Edep : "<<globalEdep.size()<<endl;
// cout<<"size of parent ID : "<<parentID.size()<<endl;
// cout<<"size of particle name : "<<ParticleName.size()<<endl;
// for(int i=0; i<ID.size(); i++)
// {
//   cout<<"ID["<<i<<"] : "<<ID[i]<<endl;
//   cout<<"parent ID["<<i<<"] : "<<parentID[i]<<endl;
//   cout<<"particle name["<<i<<"] : "<<ParticleName[i]<<endl;
//   cout<<"energy deposition["<<i<<"] : "<<globalEdep[i]<<endl;
// }

// vector<G4String>::iterator neutron_it;
// neutron_it = find(ParticleName.begin(), ParticleName.end(),"neutron");
// G4int n = neutron_it - ParticleName.begin();
// cout<<"where is neutron : "<<n<<endl;
// cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;

// for(int i=0; i<ID.size(); i++)
// {
//   cout<<"number of loop : "<<i<<endl;
//   if(ID[n] == parentID[i])
//   {
//     cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;
//   }
// }
  
// cout<<"hello event05"<<endl;

//   analysisManager->FillNtupleDColumn(0, TotalEdep);

//   analysisManager->FillNtupleDColumn(2, globalElectron);
//   analysisManager->FillNtupleDColumn(3, globalPositron);
//   analysisManager->FillNtupleDColumn(4, globalProton);
//   analysisManager->FillNtupleDColumn(5, globalC12);
//   analysisManager->FillNtupleDColumn(6, globalNucleus);
//   analysisManager->FillNtupleDColumn(7, NeutronEdep);

//   globalhIoni += TothIoni;
//   globaleIoni += ToteIoni;
//   globalhadElastic += TothadElastic;
//   globalnCapture += TotnCapture;
//   globalCompt += TotCompt;  
//   globalConv += TotConv;
//   globaleBrem += ToteBrem;
//   globalphot += Totphot;
//   globalannihil += Totannihil;
//   globalnInelastic += TotnInelastic;
//   globalOthers += TotOthers;

//   globalCnt_elec += TotCnt_elec;
//   globalCnt_posi += TotCnt_posi;
//   globalCnt_pro += TotCnt_pro;
//   globalCnt_C12 += TotCnt_C12;
//   globalCnt_Other += TotCnt_Other;

//   //
//   //

// globalOhIoni += SumhIoni;
// globaleOIoni += SumeIoni;
// globalOhadElastic += SumhadElastic;
// globalOnCapture += SumnCapture;
// globalOcompt += Sumcompt;
// globalOconv += Sumconv;
// globalOeBrem += SumeBrem;
// globalOphot += Sumphot;
// globalneutronInelastic += SumneutronInelastic;
// globalOther += SumOther; 

// globalahIoni += TotahIoni;
// globalaeIoni += TotaeIoni;
// globalahadElastic += TotahadElastic;
// globalanCapture += TotanCapture;
// globalacompt += Totacompt;
// globalaconv += Totaconv;
// globalaeBrem += TotaeBrem;
// globalaphot += Totaphot;
// globalaneutronInelastic += TotaneutronInelastic;
// globalaOther += TotaOther; 

// globalphIoni += TotphIoni;
// globalpeIoni += TotpeIoni;
// // number of hadron elastic channel from proton
// if(TotphadElastic == 0)
// {
//   globalphadElastic += 0;
// }
// else
// {
//   globalphadElastic += 1;
// }

// //number of nCapture channel from proton
// if(TotpnCapture == 0)
// {
//   globalpnCapture += 0;
// }
// else
// {
//   globalpnCapture += 1;
// }

// globalpcompt += Totpcompt;
// globalpconv += Totpconv;
// globalpeBrem += TotpeBrem;
// globalpphot += Totpphot;
// globalpneutronInelastic += TotpneutronInelastic;
// globalpOther += TotpOther; 

// globalC13hIoni += TotC13hIoni;
// globalC13eIoni += TotC13eIoni;
// globalC13hadElastic += TotC13hadElastic;

// //number of nCapture channel from C13
// if(TotC13nCapture == 0)
// {
//   globalC13nCapture += 0;
// }
// else
// {
//   globalC13nCapture += 1;
// }

// globalC13compt += TotC13compt;
// globalC13conv += TotC13conv;
// globalC13eBrem += TotC13eBrem;
// globalC13phot += TotC13phot;
// globalC13neutronInelastic += TotC13neutronInelastic;
// globalC13Other += TotC13Other;

// globalC12hIoni += TotC12hIoni;
// globalC12eIoni += TotC12eIoni;

// //number of hadron elastic channel from C12
// if(TotC12hadElastic == 0)
// {
//   globalC12hadElastic += 0;
// }
// else
// {
//   globalC12hadElastic += 1;
// }

// //number of nCapture channel from C12
// if(TotC12nCapture == 0)
// {
//   globalC12nCapture += 0;
// }
// else
// {
//   globalC12nCapture += 1;
// }

// globalC12compt += TotC12compt;
// globalC12conv += TotC12conv;
// globalC12eBrem += TotC12eBrem;
// globalC12phot += TotC12phot;
// globalC12neutronInelastic += TotC12neutronInelastic;
// globalC12Other += TotC12Other ;

// globalghIoni += TotghIoni;
// globalgeIoni += TotgeIoni;
// globalghadElastic += TotghadElastic;

// //number of nCapture channel from gamma
// if(TotgnCapture == 0)
// {
//   globalgnCapture += 0;
// }
// else
// {
//   globalgnCapture += 1;
// }

// globalgcompt += Totgcompt;
// globalgconv += Totgconv;
// globalgeBrem += TotgeBrem;
// globalgphot += Totgphot;
// globalgneutronInelastic += TotgneutronInelastic;
// globalgOther += TotgOther ;

// globaldhIoni += TotdhIoni;
// globaldeIoni += TotdeIoni;
// globaldhadElastic += TotdhadElastic;

// //number of nCapture channel from deuteron
// if(TotdnCapture == 0)
// {
//   globaldnCapture += 0;
// }
// else
// {
//   globaldnCapture += 1;
// }

// globaldcompt += Totdcompt;
// globaldconv += Totdconv;
// globaldeBrem += TotdeBrem;
// globaldphot += Totdphot;
// globaldneutronInelastic += TotdneutronInelastic;
// globaldOther += TotdOther ;

// globalposihIoni += TotposihIoni;
// globalposieIoni += TotposieIoni;
// globalposihadElastic += TotposihadElastic;
// globalposinCapture += TotposinCapture;
// globalposicompt += Totposicompt;
// globalposiconv += Totposiconv;
// globalposieBrem += TotposieBrem;
// globalposiphot += Totposiphot;
// globalposineutronInelastic += TotposineutronInelastic;
// globalposiOther += TotposiOther ;

// globalehIoni += TotehIoni;
// globaleeIoni += ToteeIoni;
// globalehadElastic += TotehadElastic;
// globalenCapture += TotenCapture;
// globalecompt += Totecompt;
// globaleconv += Toteconv;
// globaleeBrem += ToteeBrem;
// globalephot += Totephot;
// globaleneutronInelastic += ToteneutronInelastic;
// globaleOther += ToteOther;

// globalOannihil += TotOannihil;
// globalaannihil += Totaannihil;
// globalpannihil += Totpannihil;
// globalC13annihil += TotC13annihil;
// globalC12annihil += TotC12annihil;
// globalgannihil += Totgannihil;
// globaldannihil += Totdannihil;
// globalposiannihil += Totposiannihil;
// globaleannihil += Toteannihil;


//   analysisManager->AddNtupleRow();

//   // cout<<"one event is done"<<endl;
// }
