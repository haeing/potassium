
#include "LAMPS_NDA_SD.hh"
#include "LAMPS_NDA_Hit.hh"
#include "EventData.hh"

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

#include "TMath.h"
#include "G4UnitsTable.hh"
#include "TLorentzVector.h"
#include "TMath.h"

using CLHEP::MeV;
using namespace std;

G4ThreeVector DetHit(G4int CopyNumber, G4ThreeVector RealPos)
{
  G4ThreeVector DetHitPos;
  G4double St02_offset = (45.5 +34.5);

  if((CopyNumber > 0)&&(CopyNumber <= 20)) //Veto01
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -1), RealPos.y(), 0);
  }
  else if((CopyNumber > 20)&&(CopyNumber <= 40)) //St01NDv01
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -21), RealPos.y(), 13.5);
  }
  else if((CopyNumber > 40)&&(CopyNumber <= 60)) //St01NDh01
  {
    DetHitPos = G4ThreeVector(RealPos.x(), -95 +10*(CopyNumber -41), 24.);
  }
  else if((CopyNumber > 60)&&(CopyNumber <= 80)) //St01NDv02
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -61), RealPos.y(), 35.);
  }  
  else if((CopyNumber > 80)&&(CopyNumber <= 100)) //St01NDh02
  {
    DetHitPos = G4ThreeVector(RealPos.x(), -95 +10*(CopyNumber -81), 45.5);
  }  
  else if((CopyNumber > 100)&&(CopyNumber <= 120)) //Veto02
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -101), RealPos.y(), (0 +St02_offset));
  }  
  else if((CopyNumber > 120)&&(CopyNumber <= 140)) //St02NDv01
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -121), RealPos.y(), (13.5 +St02_offset));
  }  
  else if((CopyNumber > 140)&&(CopyNumber <= 160)) //St02NDh01
  {
    DetHitPos = G4ThreeVector(RealPos.x(), -95 +10*(CopyNumber -141), (24. +St02_offset));
  }  
  else if((CopyNumber > 160)&&(CopyNumber <= 180)) //St02NDv02
  {
    DetHitPos = G4ThreeVector(-95 +10*(CopyNumber -161), RealPos.y(), (35. +St02_offset));
  }   
  else if((CopyNumber > 180)&&(CopyNumber <= 200)) //St01NDh02
  {
    DetHitPos = G4ThreeVector(RealPos.x(), -95 +10*(CopyNumber -181), (45.5 +St02_offset));
  }   

  return DetHitPos;
}


LAMPS_NDA_SD::LAMPS_NDA_SD(G4String name)
    : G4VSensitiveDetector(name) {
  collectionName.insert("LAMPS_NDA");
  // cout<<"Registration for SD"<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LAMPS_NDA_SD::Initialize(G4HCofThisEvent* hce) {

  fHitsCollection01 = new NDAHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (fHCID < 0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection01);
  }
  hce->AddHitsCollection(fHCID, fHitsCollection01);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool LAMPS_NDA_SD::ProcessHits(G4Step* step, G4TouchableHistory*) {

  auto Particle = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  auto track = step -> GetTrack();
  auto pre = step -> GetPreStepPoint();
  auto post = step -> GetPostStepPoint();

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();

  auto preStepKen = step->GetPreStepPoint()->GetKineticEnergy();
  auto pstStepKen = step->GetPostStepPoint()->GetKineticEnergy();
  auto ID = step->GetTrack()->GetTrackID();
  auto ParentID = step->GetTrack()->GetParentID();

  auto analysisManager = G4AnalysisManager::Instance();
  LAMPS_NDA_Hit *hit = nullptr;

  if(ParentID == 0)
  {
    const auto process = step -> GetPostStepPoint() -> GetProcessDefinedStep();

    const std::vector< const G4Track*>*secondaries = step -> GetSecondaryInCurrentStep();
    auto nbtrk = (*secondaries).size();

    if(process)
    {
      auto processName = process -> GetProcessName();

      const auto hProcess = dynamic_cast<const G4HadronicProcess*>(process);
      if(!hProcess)
      {
        if(processName != "Transportation")
        {
          cout<<"Warning : hadronic process is nullptr"<<endl;
          cout<<"Process Name is : "<<processName<<endl;
        }
        return false;
      }

      const G4Nucleus *targetNucleus = hProcess -> GetTargetNucleus();
      if(!targetNucleus)
      {
        cout<<"Warning : target nulceus is nullptr"<<endl;
        return false;
      }
      else if(targetNucleus)
      {
        G4int Z = targetNucleus -> GetZ_asInt();
        G4int A = targetNucleus -> GetA_asInt();

        if(ID == 1)
        {
          hit = new LAMPS_NDA_Hit(copyNo);

          EventData::TotalN1Count++;
          G4ThreeVector HitPos = step -> GetPostStepPoint() -> GetPosition()/CLHEP::cm;
          G4double HitTime = step -> GetTrack() -> GetGlobalTime()/CLHEP::ns;
          G4double preKE = step -> GetPreStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
          G4double pstKE = step -> GetPostStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
          G4ThreeVector preMom = step -> GetPreStepPoint() -> GetMomentum()/CLHEP::MeV;
          G4ThreeVector pstMom = step -> GetPostStepPoint() -> GetMomentum()/CLHEP::MeV;
          G4ThreeVector nVrtxPos = step -> GetTrack() -> GetVertexPosition()/CLHEP::cm;    

          hit -> SetID(ID);
          hit -> SetParentID(ParentID);
          hit -> SetHitCount(EventData::TotalN1Count);
          hit -> SetTargetNucleus(Z);
          hit -> SetCopyNo(copyNo);

          hit -> SetHitPos(HitPos);
          hit -> SetHitTime(HitTime);
          hit -> SetPreKE(preKE);
          hit -> SetPstKE(pstKE);
          hit -> SetPreMom(preMom);
          hit -> SetPstMom(pstMom);
          hit -> SetVrtxPos(nVrtxPos);
        }
        else if(ID == 2)
        {
          hit = new LAMPS_NDA_Hit(copyNo);

          EventData::TotalN2Count++;
          G4ThreeVector HitPos = step -> GetPostStepPoint() -> GetPosition()/CLHEP::cm;
          G4double HitTime = step -> GetTrack() -> GetGlobalTime()/CLHEP::ns;
          G4double preKE = step -> GetPreStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
          G4double pstKE = step -> GetPostStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
          G4ThreeVector preMom = step -> GetPreStepPoint() -> GetMomentum()/CLHEP::MeV;
          G4ThreeVector pstMom = step -> GetPostStepPoint() -> GetMomentum()/CLHEP::MeV;
          G4ThreeVector nVrtxPos = step -> GetTrack() -> GetVertexPosition()/CLHEP::cm;    

          hit -> SetID(ID);
          hit -> SetParentID(ParentID);
          hit -> SetHitCount(EventData::TotalN2Count);
          hit -> SetTargetNucleus(Z);
          hit -> SetCopyNo(copyNo);

          hit -> SetHitPos(HitPos);
          hit -> SetHitTime(HitTime);
          hit -> SetPreKE(preKE);
          hit -> SetPstKE(pstKE);
          hit -> SetPreMom(preMom);
          hit -> SetPstMom(pstMom);  
          hit -> SetVrtxPos(nVrtxPos);        
        }        
      }
    }    
  }
  else if(ParentID != 0)
  {
    G4double charge = track -> GetParticleDefinition() -> GetPDGCharge();

    if(charge != 0.)
    {
      hit = new LAMPS_NDA_Hit(copyNo);

      G4ThreeVector HitPos = step -> GetPostStepPoint() -> GetPosition()/CLHEP::cm;
      G4double HitTime = step -> GetTrack() -> GetGlobalTime()/CLHEP::ns;
      G4double preKE = step -> GetPreStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
      G4double pstKE = step -> GetPostStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
      G4ThreeVector preMom = step -> GetPreStepPoint() -> GetMomentum()/CLHEP::MeV;
      G4ThreeVector pstMom = step -> GetPostStepPoint() -> GetMomentum()/CLHEP::MeV;      
      G4double Edep = step -> GetTotalEnergyDeposit()/CLHEP::MeV;
      G4ThreeVector VrtxPos = step -> GetTrack() -> GetVertexPosition()/CLHEP::cm;
      G4double VrtxKE = step -> GetTrack() -> GetVertexKineticEnergy()/CLHEP::MeV;
      G4int SecZ = step -> GetTrack() -> GetParticleDefinition() -> GetAtomicNumber();
      G4int SecA = step -> GetTrack() -> GetParticleDefinition() -> GetAtomicMass();
      G4int SecAtomic = 10*SecA +SecZ;
      G4double Length = step -> GetStepLength()/CLHEP::cm;

      hit -> SetID(ID);
      hit -> SetParentID(ParentID);
      hit -> SetCopyNo(copyNo);

      hit -> SetHitPos(HitPos);
      hit -> SetHitTime(HitTime);
      hit -> SetPreKE(preKE);
      hit -> SetPstKE(pstKE);
      hit -> SetPreMom(preMom);
      hit -> SetPstMom(pstMom);
      hit -> SetEdep(Edep);
      hit -> SetVrtxPos(VrtxPos);
      hit -> SetVrtxKE(VrtxKE);
      hit -> SetSecAtomic(SecAtomic);
      hit -> SetParticleName(Particle);
      hit -> SetStepLength(Length);
    }
    else
    {
      if(Particle == "gamma")
      {
        hit = new LAMPS_NDA_Hit(copyNo);

        G4ThreeVector HitPos = step -> GetPostStepPoint() -> GetPosition()/CLHEP::cm;
        G4double HitTime = step -> GetTrack() -> GetGlobalTime()/CLHEP::ns;
        G4double preKE = step -> GetPreStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
        G4double pstKE = step -> GetPostStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
        G4ThreeVector preMom = step -> GetPreStepPoint() -> GetMomentum()/CLHEP::MeV;
        G4ThreeVector pstMom = step -> GetPostStepPoint() -> GetMomentum()/CLHEP::MeV;      
        G4ThreeVector VrtxPos = step -> GetTrack() -> GetVertexPosition()/CLHEP::cm;
        G4double VrtxKE = step -> GetTrack() -> GetVertexKineticEnergy()/CLHEP::MeV;

        hit -> SetID(ID);
        hit -> SetParentID(ParentID);
        hit -> SetCopyNo(copyNo);

        hit -> SetHitPos(HitPos);
        hit -> SetHitTime(HitTime);
        hit -> SetPreKE(preKE);
        hit -> SetPstKE(pstKE);
        hit -> SetPreMom(preMom);
        hit -> SetPstMom(pstMom);
        hit -> SetEdep(0.);
        hit -> SetVrtxPos(VrtxPos);  
        hit -> SetVrtxKE(VrtxKE);     
        hit -> SetSecAtomic(5); 
      }
      // else if(Particle == "neutron")
      else
      {
        hit = new LAMPS_NDA_Hit(copyNo);

        G4ThreeVector HitPos = step -> GetPostStepPoint() -> GetPosition()/CLHEP::cm;
        G4double HitTime = step -> GetTrack() -> GetGlobalTime()/CLHEP::ns;
        G4double preKE = step -> GetPreStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
        G4double pstKE = step -> GetPostStepPoint() -> GetKineticEnergy()/CLHEP::MeV;
        G4ThreeVector preMom = step -> GetPreStepPoint() -> GetMomentum()/CLHEP::MeV;
        G4ThreeVector pstMom = step -> GetPostStepPoint() -> GetMomentum()/CLHEP::MeV;      
        G4ThreeVector VrtxPos = step -> GetTrack() -> GetVertexPosition()/CLHEP::cm;
        G4double VrtxKE = step -> GetTrack() -> GetVertexKineticEnergy()/CLHEP::MeV;
        G4int SecZ = step -> GetTrack() -> GetParticleDefinition() -> GetAtomicNumber();
        G4int SecA = step -> GetTrack() -> GetParticleDefinition() -> GetAtomicMass();
        G4int SecAtomic = 10*SecA +SecZ;
        G4double Length = step -> GetStepLength()/CLHEP::cm;

        hit -> SetID(ID);
        hit -> SetParentID(ParentID);
        hit -> SetCopyNo(copyNo);

        hit -> SetHitPos(HitPos);
        hit -> SetHitTime(HitTime);
        hit -> SetPreKE(preKE);
        hit -> SetPstKE(pstKE);
        hit -> SetPreMom(preMom);
        hit -> SetPstMom(pstMom);
        hit -> SetEdep(0.);
        hit -> SetVrtxPos(VrtxPos);      
        hit -> SetVrtxKE(VrtxKE);  
        hit -> SetSecAtomic(SecAtomic);
        hit -> SetStepLength(Length);
      }
    }
  }
 
  if(hit)
  {
    fHitsCollection01->insert(hit);
    return true;
  }

  return false;
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

