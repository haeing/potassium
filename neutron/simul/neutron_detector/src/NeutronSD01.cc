
#include "NeutronSD01.hh"

#include "NeutronHit01.hh"
// #include "Constants.hh"

#include <fstream>
#include <iostream>
#include <string>

#include "G4AnalysisManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4HadronicProcess.hh"
#include "G4Neutron.hh"
#include "G4Nucleus.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4ios.hh"

using CLHEP::MeV;
// using std::ofstream;
// using std::string;
using namespace std;

// extern G4String ProcessName;
// extern G4String nParticle;

// extern G4String eProcessName_Otherwise;
// extern G4String posiProcessName_Otherwise;
// extern G4String dProcessName_Otherwise;
// extern G4String gProcessName_Otherwise;
// extern G4String C12ProcessName_Otherwise;
// extern G4String C13ProcessName_Otherwise;
// extern G4String pProcessName_Otherwise;
// extern G4String aProcessName_Otherwise;
// extern G4String ParticleName_hIoni;
// extern G4String ParticleName_eIoni;
// extern G4String ParticleName_hadElastic;
// extern G4String ParticleName_nCapture;
// extern G4String ParticleName_compt;
// extern G4String ParticleName_conv;
// extern G4String ParticleName_eBrem;
// extern G4String ParticleName_phot;
// extern G4String ParticleName_annihil;
// extern G4String ParticleName_otherwise;
// extern G4String ProcessName_Otherwise;
// extern G4String ParticleName_neutronInelastic;

NeutronSD01::NeutronSD01(G4String name)
    : G4VSensitiveDetector(name) {
  collectionName.insert("NeutronCounter01");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronSD01::Initialize(G4HCofThisEvent* hce) {

  fHitsCollection01 = new NeutronHitsCollection01(SensitiveDetectorName, collectionName[0]);
  if (fHCID < 0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection01);
  }
  hce->AddHitsCollection(fHCID, fHitsCollection01);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NeutronSD01::ProcessHits(G4Step* step, G4TouchableHistory*) {
  
  auto Particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4int neutronID;
  G4double KE_neutron;

  if(Particle == "neutron")
  {
    neutronID = step -> GetTrack() -> GetTrackID();
    // cout<<"track ID of neutron : "<<neutronID<<endl;
  }

  // auto processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();

  auto preStepKen = step->GetPreStepPoint()->GetKineticEnergy();
  auto pstStepKen = step->GetPostStepPoint()->GetKineticEnergy();
  auto ID = step->GetTrack()->GetTrackID();
  auto ParentID = step->GetTrack()->GetParentID();

  auto analysisManager = G4AnalysisManager::Instance();
  auto hit = new NeutronHit01(copyNo);

    G4double edep = step->GetTotalEnergyDeposit();
    if(Particle == "proton" || Particle == "C12")
    {
      hit->AddEdep(edep);
    }

    if(step -> IsFirstStepInVolume ())
    {
      if(Particle == "neutron")
      {
        KE_neutron = step -> GetPreStepPoint() -> GetKineticEnergy();
        hit -> SetNeutronKineticEnergy(KE_neutron);
      }

    }

    // if(Particle == "proton")
    // {
    //   hit -> SetProtonEdep(edep);
    // }

    // if(Particle == "C12" || Particle == "C13")
    // {
    //   hit -> SetCarbon12Edep(edep);
    // }

  hit->SetInKEn(preStepKen);
  hit->SetFnKEn(pstStepKen);

  hit->SetID(ID);
  hit -> SetParentID(ParentID);
  hit -> SetPName(Particle);

  hit->SetNum(1);

  fHitsCollection01->insert(hit);

  return true;
}
//
//Logic of Secondary Particle
//


// const std::vector< const G4Track*>*secondaries = step -> GetSecondaryInCurrentStep();
// auto nbtrk = (*secondaries).size();
// if(!(nbtrk))
// {
//   return false;
// }
// cout<<"number of secondary particle size : "<<nbtrk<<endl;

// cout<<"preparation of secondary particle"<<endl;
//   for(int i=0; i<nbtrk; i++)
//   {
//     const G4Track* track = (*secondaries)[i];
//     G4String ParticleName = track -> GetParticleDefinition() -> GetParticleName();

//     double secKE = track -> GetKineticEnergy();
//     cout<<"Secondary Particle Name : "<<ParticleName<<endl;
//     // cout<<"KE of Secondary particle : "<<secKE<<endl;
//     edep += secKE;



//
  // Check the target nucleus and store the information in txt file

  //   G4int Z = 0, A = 0;

  // if(processtest == "neutronInelastic")
  //   {
  //     const auto process = step -> GetTrack() -> GetCreatorProcess();
  //     const auto hadronicProcess = dynamic_cast<const G4HadronicProcess*>(process);

  //     if (!hadronicProcess) {
  //     cout << "Error: hadronicProcess is nullptr" << endl;
  //     }

  //     const G4Nucleus* targetNucleus = hadronicProcess -> GetTargetNucleus();

  //     if (!targetNucleus) {
  //         cout << "Warning: targetNucleus is nullptr" << endl;
  //     }
  //     if(targetNucleus)
  //     {

  //      Z = targetNucleus -> GetZ_asInt();
  //      A = targetNucleus -> GetA_asInt();

  //     cout << "Neutron reacted with nucleus (Z= " << Z << ", A=" << A << ")" << endl;
  //     cout << "Track ID is "<<ID<<endl;
  //   }

  //   }
  //   cout<<endl;

  //   ofstream out("test.txt",ios::app);
  //   out<<"Particle : "<<Particle<<", Energy Deposition : "<<edep<<endl;
  //   if(processtest == "neutronInelastic")
  //   {
  //     out<<"Target for neutron Inelastic : A = "<<A<<", Z = "<<Z<< ", ID : "<<ID<<endl;
  //     out<<endl;
  //   }
  //   cout<<endl;

  //   out.close();




    // //
  // // Checking particle from reaction

  // // electron
  // if (Particle == "e-") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();

  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetehIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SeteeIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetehadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetenCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setecompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Seteconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SeteeBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setephot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Seteannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SeteneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     eProcessName_Otherwise = processtest;
  //     hit->SeteOther(1);
  //   }
  // }

  // // positron

  // else if (Particle == "e+") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposihIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposieIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposihadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposinCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setposicompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setposiconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposieBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setposiphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setposiannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetposineutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     posiProcessName_Otherwise = processtest;
  //     hit->SetposiOther(1);
  //   }
  // }

  // // deuteron

  // else if (Particle == "deuteron") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdhIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdeIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdhadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdnCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setdcompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setdconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdeBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setdphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setdannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetdneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     dProcessName_Otherwise = processtest;
  //     hit->SetdOther(1);
  //   }
  // }

  // // gamma

  // else if (Particle == "gamma") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetghIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetgeIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetghadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetgnCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setgcompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     //   cout<<"Particle's Name : "<<Particle<<endl;
  //     //   cout<<"Particle ID : "<<ID<<endl;
  //     //   cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     //   cout<<endl;

  //     hit->Setgconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetgeBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setgphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setgannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetgneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     gProcessName_Otherwise = processtest;
  //     hit->SetgOther(1);
  //   }
  // }

  // // C12

  // else if (Particle == "C12") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12hIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12eIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12hadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12nCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12compt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12conv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12eBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12phot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12annihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC12neutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     C12ProcessName_Otherwise = processtest;
  //     hit->SetC12Other(1);
  //   }
  // }

  // // C13

  // else if (Particle == "C13") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13hIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13eIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13hadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13nCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13compt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13conv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13eBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13phot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13annihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetC13neutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     C13ProcessName_Otherwise = processtest;
  //     hit->SetC13Other(1);
  //   }
  // }

  // // proton

  // else if (Particle == "proton") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetphIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetpeIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetphadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetpnCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setpcompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setpconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetpeBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setpphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setpannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetpneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     pProcessName_Otherwise = processtest;
  //     hit->SetpOther(1);
  //   }
  // }

  // // alpha

  // else if (Particle == "alpha") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetahIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetaeIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetahadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetanCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setacompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setaconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetaeBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setaphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->Setaannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     hit->SetaneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     aProcessName_Otherwise = processtest;
  //     hit->SetaOther(1);
  //   }
  // }

  // // Other particle

  // else {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();

  //   if (processtest == "hIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_hIoni = Particle;
  //     hit->SethIoni(1);
  //   }

  //   else if (processtest == "eIoni") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_eIoni = Particle;
  //     hit->SeteIoni(1);
  //   }

  //   else if (processtest == "hadElastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_hadElastic = Particle;
  //     hit->SethadElastic(1);
  //   }

  //   else if (processtest == "nCapture") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_nCapture = Particle;
  //     hit->SetnCapture(1);
  //   }

  //   else if (processtest == "compt") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_compt = Particle;
  //     hit->Setcompt(1);
  //   }

  //   else if (processtest == "conv") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_conv = Particle;
  //     hit->Setconv(1);
  //   }

  //   else if (processtest == "eBrem") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_eBrem = Particle;
  //     hit->SeteBrem(1);
  //   }

  //   else if (processtest == "phot") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_phot = Particle;
  //     hit->Setphot(1);
  //   }

  //   else if (processtest == "annihil") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_annihil = Particle;
  //     hit->Setannihil(1);
  //   }

  //   else if (processtest == "neutronInelastic") {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_neutronInelastic = Particle;
  //     hit->SetneutronInelastic(1);
  //   }

  //   else {
  //     // cout<<"Particle's Name : "<<Particle<<endl;
  //     // cout<<"Particle ID : "<<ID<<endl;
  //     // cout<<"Particle's Parent ID : "<<ParentID<<endl;
  //     // cout<<endl;

  //     ParticleName_otherwise = Particle;
  //     ProcessName_Otherwise = processtest;
  //     hit->SetOther(1);
  //   }
  // }

  // //
  // // Get energy deposition and checking neutron inelastic
  // // if(step->GetTrack()->GetParentID()!= 1) return 0;
  // if (Particle == "e-") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  //   G4double edep = step->GetTotalEnergyDeposit();
  //   hit->AddElectronEdep(edep);

  //   // cout<<"This process come from : "<<processtest<<endl;

  //   if (processtest == "neutronInelastic") {
  //     hit->SetNnInelaCount01(1);
  //   }

  // } else if (Particle == "e+") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  //   G4double edep = step->GetTotalEnergyDeposit();
  //   hit->AddPositronEdep(edep);

  //   // processtest = step -> GetTrack() -> GetCreatorProcess() -> GetProcessName();
  //   // cout<<"This process come from : "<<processtest<<endl;

  //   if (processtest == "neutronInelastic") {
  //     hit->SetNnInelaCount02(1);
  //   }
  // } else if (Particle == "proton" or Particle == "deuteron") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  //   G4double edep = step->GetTotalEnergyDeposit();
  //   hit->AddProtonEdep(edep);

  //   // processtest = step -> GetTrack() -> GetCreatorProcess() -> GetProcessName();
  //   // cout<<"This process come from : "<<processtest<<endl;

  //   if (processtest == "neutronInelastic") {
  //     hit->SetNnInelaCount03(1);
  //   }
  // } else if (Particle == "C12" || Particle == "C13") {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  //   G4double edep = step->GetTotalEnergyDeposit();
  //   hit->AddCarbon12Edep(edep);

  //   // processtest = step -> GetTrack() -> GetCreatorProcess() -> GetProcessName();
  //   // cout<<"This process come from : "<<processtest<<endl;

  //   if (processtest == "neutronInelastic") {
  //     hit->SetNnInelaCount04(1);
  //   }
  // } else {
  //   G4String processtest = step->GetTrack()->GetCreatorProcess()->GetProcessName();
  //   G4double edep = step->GetTotalEnergyDeposit();
  //   // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  //   hit->AddNucleusEdep(edep);

  //   // processtest = step -> GetTrack() -> GetCreatorProcess() -> GetProcessName();
  //   // cout<<"This process come from : "<<processtest<<endl;

  //   if (processtest == "neutronInelastic") {
  //     hit->SetNnInelaCount05(1);
  //     nParticle = Particle;
  //   }
  // }

