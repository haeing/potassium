
#include "RunAction.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "G4RunManager.hh"

//   G4int globalhIoni = 0; 
//   G4int globaleIoni = 0;
//   G4int globalhadElastic = 0;
//   G4int globalnCapture = 0;
//   G4int globalCompt = 0;
//   G4int globalConv = 0;
//   G4int globaleBrem = 0;
//   G4int globalphot = 0;
//   G4int globalannihil = 0;
//   G4int globalnInelastic = 0;
//   G4int globalOthers = 0;

//   G4int globalCnt_elec = 0;
//   G4int globalCnt_posi = 0;
//   G4int globalCnt_pro = 0;
//   G4int globalCnt_C12 = 0;
//   G4int globalCnt_Other = 0;

//   G4String ProcessName;
//   G4String nParticle;

// //
// //

// G4int globalOhIoni = 0;
// G4int globaleOIoni = 0;
// G4int globalOhadElastic = 0;
// G4int globalOnCapture = 0;
// G4int globalOcompt = 0 ;
// G4int globalOconv = 0;
// G4int globalOeBrem = 0 ;
// G4int globalOphot = 0;
// G4int globalneutronInelastic = 0;
// G4int globalOther = 0;

// G4int globalahIoni = 0;
// G4int globalaeIoni = 0;
// G4int globalahadElastic = 0;
// G4int globalanCapture = 0;
// G4int globalacompt  = 0;
// G4int globalaconv = 0;
// G4int globalaeBrem  = 0;
// G4int globalaphot = 0;
// G4int globalaneutronInelastic = 0;
// G4int globalaOther = 0;

// G4int globalphIoni = 0;
// G4int globalpeIoni = 0;
// G4int globalphadElastic = 0;
// G4int globalpnCapture = 0;
// G4int globalpcompt  = 0;
// G4int globalpconv = 0;
// G4int globalpeBrem  = 0;
// G4int globalpphot = 0;
// G4int globalpneutronInelastic = 0;
// G4int globalpOther = 0;

// G4int globalC13hIoni = 0;
// G4int globalC13eIoni = 0;
// G4int globalC13hadElastic = 0;
// G4int globalC13nCapture = 0; 
// G4int globalC13compt  = 0;
// G4int globalC13conv = 0;
// G4int globalC13eBrem  = 0;
// G4int globalC13phot = 0;
// G4int globalC13neutronInelastic = 0;
// G4int globalC13Other = 0;

// G4int globalC12hIoni;
// G4int globalC12eIoni;
// G4int globalC12hadElastic;
// G4int globalC12nCapture;
// G4int globalC12compt ;
// G4int globalC12conv;
// G4int globalC12eBrem ;
// G4int globalC12phot;
// G4int globalC12neutronInelastic;
// G4int globalC12Other;

// G4int globalghIoni = 0;
// G4int globalgeIoni = 0;
// G4int globalghadElastic = 0;
// G4int globalgnCapture = 0;
// G4int globalgcompt  = 0;
// G4int globalgconv = 0;
// G4int globalgeBrem  = 0;
// G4int globalgphot = 0;
// G4int globalgneutronInelastic  = 0;
// G4int globalgOther  = 0;

// G4int globaldhIoni = 0;
// G4int globaldeIoni = 0;
// G4int globaldhadElastic = 0;
// G4int globaldnCapture = 0; 
// G4int globaldcompt = 0 ;
// G4int globaldconv = 0;
// G4int globaldeBrem = 0 ;
// G4int globaldphot = 0;
// G4int globaldneutronInelastic = 0 ;
// G4int globaldOther = 0 ;

// G4int globalposihIoni = 0;
// G4int globalposieIoni = 0;
// G4int globalposihadElastic = 0;
// G4int globalposinCapture = 0 ;
// G4int globalposicompt = 0 ;
// G4int globalposiconv = 0;
// G4int globalposieBrem = 0 ;
// G4int globalposiphot = 0;
// G4int globalposineutronInelastic = 0 ;
// G4int globalposiOther = 0;

// G4int globalehIoni = 0;
// G4int globaleeIoni = 0;
// G4int globalehadElastic = 0;
// G4int globalenCapture = 0;
// G4int globalecompt  = 0;
// G4int globaleconv = 0;
// G4int globaleeBrem  = 0;
// G4int globalephot = 0;
// G4int globaleneutronInelastic = 0;
// G4int globaleOther = 0;

// G4int globalOannihil = 0;
// G4int globalaannihil = 0;
// G4int globalpannihil = 0;
// G4int globalC13annihil = 0;
// G4int globalC12annihil = 0;
// G4int globalgannihil = 0;
// G4int globaldannihil = 0;
// G4int globalposiannihil = 0;
// G4int globaleannihil = 0;

// //
// //
// G4String eProcessName_Otherwise;
// G4String posiProcessName_Otherwise;
// G4String dProcessName_Otherwise;
// G4String gProcessName_Otherwise;
// G4String C12ProcessName_Otherwise;
// G4String C13ProcessName_Otherwise;
// G4String pProcessName_Otherwise;
// G4String aProcessName_Otherwise;
// G4String ParticleName_hIoni;
// G4String ParticleName_eIoni;
// G4String ParticleName_hadElastic;
// G4String ParticleName_nCapture;
// G4String ParticleName_compt;
// G4String ParticleName_conv;
// G4String ParticleName_eBrem;
// G4String ParticleName_phot;
// G4String ParticleName_annihil;
// G4String ParticleName_otherwise;
// G4String ProcessName_Otherwise;
// G4String ParticleName_neutronInelastic;

// G4int TotalReact = 0;
// G4int TotnumberEvt = 0;

using namespace std;


RunAction::RunAction(EventAction* eventAction)
:fEventAction(eventAction)
{
  //  cout<<"Start runaction initialization"<<endl;
  // Create the generic analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
     // If the filename extension is not provided, the default file type (root)
     // will be used for all files specified without extension.
  analysisManager->SetVerboseLevel(1);

  // Default settings
  analysisManager->SetNtupleMerging(true);
     // Note: merging ntuples is available only with Root output
  analysisManager->SetFileName("Neutron");

  // Book histograms, ntuple


  // Creating ntuple
  if ( fEventAction ) {
    analysisManager->CreateNtuple("tree", "Hits");

    //
    // Neutron Detector 01
    analysisManager->CreateNtupleDColumn("TotalEdep01");  // column Id = 0
    // analysisManager->CreateNtupleIColumn("secNum"); 
    // analysisManager->CreateNtupleDColumn("ElectronEdep");
    // analysisManager->CreateNtupleDColumn("PositronEdep");
    analysisManager->CreateNtupleDColumn("ProtonEdep01"); //column Id = 1
    analysisManager->CreateNtupleDColumn("CarbonEdep01"); //column Id = 2
    // analysisManager->CreateNtupleDColumn("NucleusEdep"); 
    analysisManager->CreateNtupleDColumn("NeutronEdep01"); //column Id = 3
    analysisManager->CreateNtupleDColumn("NeutronKineticEnergyInDetector01"); //column Id = 4

    analysisManager->CreateNtupleDColumn("NeutronKineticEnergyFromSource"); //column Id = 5

    //
    // Neutron Detector 02
    analysisManager->CreateNtupleDColumn("TotalEdep02");  // column Id = 6
    // analysisManager->CreateNtupleIColumn("secNum"); 
    // analysisManager->CreateNtupleDColumn("ElectronEdep"); 
    // analysisManager->CreateNtupleDColumn("PositronEdep"); 
    analysisManager->CreateNtupleDColumn("ProtonEdep02"); //column Id = 7
    analysisManager->CreateNtupleDColumn("CarbonEdep02"); //column Id = 8
    // analysisManager->CreateNtupleDColumn("NucleusEdep"); 
    analysisManager->CreateNtupleDColumn("NeutronEdep02"); //column Id = 9
    analysisManager->CreateNtupleDColumn("NeutronKineticEnergyInDetector02"); //column Id = 10

    analysisManager->FinishNtuple();
  }

  // Set ntuple output file
  analysisManager->SetNtupleFileName(0, "./AmBe_Data/AmBe_Run90.root");
  analysisManager->CreateNtuple("neutrons", "Neutrons leaving the AmBe source");
  analysisManager->CreateNtupleDColumn("energy_MeV");
  analysisManager->FinishNtuple();
  analysisManager->SetNtupleFileName(1, AMBE_OUTPUT_FILE);
  // analysisManager->SetNtupleFileName(0, "./Test_XMeV.root");

  // cout<<"Set the root file "<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Reset();

  // Open an output file
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();

  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  // save histograms & ntuple
  //
  
  // cout<<"end of run process start"<<endl;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();

  // cout<< "write the root file"<<endl;

  analysisManager->CloseFile(false);
  // cout<<"close the root file"<<endl;
  const auto number = run -> GetNumberOfEvent();
  cout<<"number of Event : "<<number<<endl;

  // cout<<"run is done"<<endl;
    // Keep content of histos so that they are plotted.
    // The content will be reset at start of the next run.
}

//
// This part is for printing all of particle from each reaction just in case
//



  // cout<<"test reaction number : "<<reactNum<<" test total number : "<<Totnumber<<endl;
//   // cout<<"test total reaction number : "<<TotalReact<<" test total event number : "<<TotnumberEvt<<endl;
// cout<<endl;
// cout<<"Number of Other reaction : "<<globalOthers<<endl;
// cout<<"Number of hIoni : "<<globalhIoni<<endl;
// cout<<"Number of eIoni : "<<globaleIoni<<endl;
// cout<<"Number of hadElastic : "<<globalhadElastic<<endl;
// cout<<"Number of nCapture : "<<globalnCapture<<endl;
// cout<<"Number of Compton Scattering : "<<globalCompt<<endl;
// cout<<"Number of Gamma Conversion : "<<globalConv<<endl;
// cout<<"Number of Bremsstrahlung : "<<globaleBrem<<endl;
// cout<<"Number of annihilation : "<<globalannihil<<endl;
// cout<<"Number of Neutron Inelastic : "<<globalnInelastic<<endl;
// cout<<endl;

// cout<<"Process Name is "<<ProcessName<<endl;
// cout<<endl;

// cout<<"Particle Name of neutron inelastic : "<<nParticle<<endl;
// cout<<"electron in neutron inelastic : "<<globalCnt_elec<<endl;
// cout<<"positron in neutron inelastic : "<<globalCnt_posi<<endl;
// cout<<"proton in neutron inelastic : "<<globalCnt_pro<<endl;
// cout<<"carbon in neutron inelastic : "<<globalCnt_C12<<endl;
// cout<<endl;
// cout<<"--------------------------------------------------------------------------------------------"<<endl;
// cout<<endl;

// cout<<"Other Particle ("<<ParticleName_hIoni<<") from hIoni : "<<globalOhIoni<<endl;
// cout<<"Other Particle ("<<ParticleName_eIoni<<") from eIoni : "<<globaleOIoni<<endl;
// cout<<"Other Particle ("<<ParticleName_hadElastic<<") from hadElastic : "<<globalOhadElastic<<endl;
// cout<<"Other Particle ("<<ParticleName_nCapture<<") from nCapture : "<<globalOnCapture<<endl;
// cout<<"Other Particle ("<<ParticleName_compt<<") from compt : "<<globalOcompt <<endl;
// cout<<"Other Particle ("<<ParticleName_conv<<") from conv : "<<globalOconv<<endl;
// cout<<"Other Particle ("<<ParticleName_eBrem<<") from eBrem : "<<globalOeBrem <<endl;
// cout<<"Other Particle ("<<ParticleName_phot<<") from phot : "<<globalOphot<<endl;
// cout<<"Other Particle ("<<ParticleName_annihil<<") from annihil : "<<globalOannihil<<endl;
// cout<<"Other Particle ("<<ParticleName_neutronInelastic<<") from neutronInelastic : "<<globalneutronInelastic<<endl;
// cout<<"Other Particle ("<<ParticleName_otherwise<<") from Other reaction ("<<ProcessName_Otherwise<<")  : "<<globalOther<<endl;
// cout<<endl;

// cout<<"alpha from hIoni : "<<globalahIoni<<endl;
// cout<<"alpha from eIoni : "<<globalaeIoni<<endl;
// cout<<"alpha from hadElastic : "<<globalahadElastic<<endl;
// cout<<"alpha from nCapture : "<<globalanCapture<<endl;
// cout<<"alpha from compt : "<<globalacompt <<endl;
// cout<<"alpha from conv : "<<globalaconv<<endl;
// cout<<"alpha from eBrem : "<<globalaeBrem <<endl;
// cout<<"alpha from phot : "<<globalaphot<<endl;
// cout<<"alpha from annihil : "<<globalaannihil<<endl;
// cout<<"alpha from neutronInelastic : "<<globalaneutronInelastic<<endl;
// cout<<"alpha from Other reaction ("<<aProcessName_Otherwise<<") : "<<globalaOther<<endl;
// cout<<endl;

// cout<<"proton from hIoni : "<<globalphIoni<<endl;
// cout<<"proton from eIoni : "<<globalpeIoni<<endl;
// cout<<"proton from hadElastic : "<<globalphadElastic<<endl;
// cout<<"proton from nCapture : "<<globalpnCapture<<endl;
// cout<<"proton from compt : "<<globalpcompt <<endl;
// cout<<"proton from conv : "<<globalpconv<<endl;
// cout<<"proton from eBrem : "<<globalpeBrem <<endl;
// cout<<"proton from phot : "<<globalpphot<<endl;
// cout<<"proton from annihil : "<<globalpannihil<<endl;
// cout<<"proton from neutronInelastic : "<<globalpneutronInelastic<<endl;
// cout<<"proton from Other reaction ("<<pProcessName_Otherwise<<") : "<<globalpOther<<endl;
// cout<<endl;

// cout<<"C13 from hIoni : "<<globalC13hIoni<<endl;
// cout<<"C13 from eIoni : "<<globalC13eIoni<<endl;
// cout<<"C13 from hadElastic : "<<globalC13hadElastic<<endl;
// cout<<"C13 from nCapture : "<<globalC13nCapture<<endl;
// cout<<"C13 from compt : "<<globalC13compt <<endl;
// cout<<"C13 from conv : "<<globalC13conv<<endl;
// cout<<"C13 from eBrem : "<<globalC13eBrem <<endl;
// cout<<"C13 from phot : "<<globalC13phot<<endl;
// cout<<"C13 from annihil : "<<globalC13annihil<<endl;
// cout<<"C13 from neutronInelastic : "<<globalC13neutronInelastic<<endl;
// cout<<"C13 from Other reaction ("<<C13ProcessName_Otherwise<<") : "<<globalC13Other<<endl;
// cout<<endl;

// cout<<"C12 from hIoni : "<<globalC12hIoni<<endl;
// cout<<"C12 from eIoni : "<<globalC12eIoni<<endl;
// cout<<"C12 from hadElastic : "<<globalC12hadElastic<<endl;
// cout<<"C12 from nCapture : "<<globalC12nCapture<<endl;
// cout<<"C12 from compt : "<<globalC12compt <<endl;
// cout<<"C12 from conv : "<<globalC12conv<<endl;
// cout<<"C12 from eBrem : "<<globalC12eBrem <<endl;
// cout<<"C12 from phot : "<<globalC12phot<<endl;
// cout<<"C12 from annihil : "<<globalC12annihil<<endl;
// cout<<"C12 from neutronInelastic : "<<globalC12neutronInelastic<<endl;
// cout<<"C12 from Other reaction ("<<C12ProcessName_Otherwise<<") : "<<globalC12Other<<endl;
// cout<<endl;

// cout<<"gamma from hIoni : "<<globalghIoni<<endl;
// cout<<"gamma from eIoni : "<<globalgeIoni<<endl;
// cout<<"gamma from hadElastic : "<<globalghadElastic<<endl;
// cout<<"gamma from nCapture : "<<globalgnCapture<<endl;
// cout<<"gamma from compt : "<<globalgcompt <<endl;
// cout<<"gamma from conv : "<<globalgconv<<endl;
// cout<<"gamma from eBrem : "<<globalgeBrem <<endl;
// cout<<"gamma from phot : "<<globalgphot<<endl;
// cout<<"gamma from annihil : "<<globalgannihil <<endl;
// cout<<"gamma from neutronInelastic : "<<globalgneutronInelastic <<endl;
// cout<<"gamma from Other reaction ("<<gProcessName_Otherwise<<") : "<<globalgOther <<endl;
// cout<<endl;

// cout<<"deuteron from hIoni : "<< globaldhIoni<<endl;
// cout<<"deuteron from eIoni : "<< globaldeIoni<<endl;
// cout<<"deuteron from hadElastic : "<< globaldhadElastic<<endl;
// cout<<"deuteron from nCapture : "<< globaldnCapture<<endl;
// cout<<"deuteron from compt : "<< globaldcompt <<endl;
// cout<<"deuteron from conv : "<< globaldconv<<endl;
// cout<<"deuteron from eBrem : "<< globaldeBrem <<endl;
// cout<<"deuteron from phot : "<< globaldphot<<endl;
// cout<<"deuteron from annihil : "<< globaldannihil <<endl;
// cout<<"deuteron from neutronInelastic : "<< globaldneutronInelastic <<endl;
// cout<<"deuteron from Other reaction ("<<dProcessName_Otherwise<<") : "<< globaldOther <<endl;
// cout<<endl;

// cout<<"positron from hIoni : "<<globalposihIoni<<endl;
// cout<<"positron from eIoni : "<<globalposieIoni<<endl;
// cout<<"positron from hadElastic : "<<globalposihadElastic<<endl;
// cout<<"positron from nCapture : "<<globalposinCapture <<endl;
// cout<<"positron from compt : "<<globalposicompt <<endl;
// cout<<"positron from conv : "<<globalposiconv<<endl;
// cout<<"positron from eBrem : "<<globalposieBrem <<endl;
// cout<<"positron from phot : "<<globalposiphot<<endl;
// cout<<"positron from annihil : "<<globalposiannihil <<endl;
// cout<<"positron from neutronInelastic : "<<globalposineutronInelastic <<endl;
// cout<<"positron from Other reaction ("<<posiProcessName_Otherwise<<") : "<<globalposiOther<<endl;
// cout<<endl;

// cout<<"electron from hIoni : "<<globalehIoni<<endl;
// cout<<"electron from eIoni : "<<globaleeIoni<<endl;
// cout<<"electron from hadElastic : "<<globalehadElastic<<endl;
// cout<<"electron from nCapture : "<<globalenCapture<<endl;
// cout<<"electron from compt : "<<globalecompt <<endl;
// cout<<"electron from conv : "<<globaleconv<<endl;
// cout<<"electron from eBrem : "<<globaleeBrem <<endl;
// cout<<"electron from phot : "<<globalephot<<endl;
// cout<<"electron from annihil : "<<globaleannihil<<endl;
// cout<<"electron from neutronInelastic : "<<globaleneutronInelastic<<endl;
// cout<<"electron from Other reaction ("<<eProcessName_Otherwise<<") : "<<globaleOther<<endl;
// cout<<endl;
