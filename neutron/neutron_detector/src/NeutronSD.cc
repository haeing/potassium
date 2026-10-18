
#include "NeutronSD.hh"
#include "NeutronHit.hh"
//#include "Constants.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"

#include "G4Neutron.hh"
#include "G4RunManager.hh"

using std::cout;
using std::endl;
using CLHEP::MeV;

NeutronSD::NeutronSD(G4String name)
: G4VSensitiveDetector(name)
{
  collectionName.insert("NeutronCounter");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NeutronSD::Initialize(G4HCofThisEvent* hce)
{
  //cout<<"initialize the SD"<<endl;
  fHitsCollection
    = new NeutronHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  //cout<<"Get the Collection ID"<<endl;
  hce->AddHitsCollection(fHCID,fHitsCollection);
  //cout<<"Add the Hit Collection"<<endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool NeutronSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // cout<<"hit processing start"<<endl;

  auto Particle = step -> GetTrack() -> GetParticleDefinition() -> GetParticleName();
if(Particle == "neutron")
{
  return false;
}

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();

  auto preStepKen = step -> GetPreStepPoint() -> GetKineticEnergy();
  auto pstStepKen = step -> GetPostStepPoint() -> GetKineticEnergy();
  auto ID = step -> GetTrack() -> GetTrackID();
  G4double edep;
  if(Particle != "e-")
  {
      edep = step->GetTotalEnergyDeposit();
  }
  else
  {
    edep = 0;
  }

  auto ElectronEdep = 0;
  auto PositronEdep = 0;
  auto ProtonEdep = 0; 
  auto C12Edep = 0;
  auto nucleus = 0;
  auto test = 0;
  auto test01 = 0;

  // cout<<"initialzation variable"<<endl;


  //
  //
  auto analysisManager = G4AnalysisManager::Instance();
  auto hit = new NeutronHit(copyNo);

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

    // if(ParticleName == "e-")
    // {
    //   cout<<"there are "<< ParticleName<<endl;
    //   test = track -> GetKineticEnergy();
    //   if(!test)
    //   {
    //     cout<<"there are no deposit energy of "<<ParticleName<<endl;
    //     break;
    //   }
    //   else
    //   {
    //     ElectronEdep = track -> GetStep() -> GetTotalEnergyDeposit();
    //     cout<<"Energy Deposition of electron : "<<ElectronEdep<<endl;
    //   }
    // }

    // else if(ParticleName == "e+")
    // {
    //   cout<<"there are "<< ParticleName<<endl;
    //   test = track -> GetKineticEnergy();
    //   if(!test)
    //   {
    //     cout<<"there are no deposit energy of "<<ParticleName<<endl;
    //     break;
    //   }
    //   else
    //   {
    //     PositronEdep = track -> GetStep() -> GetTotalEnergyDeposit();
    //     cout<<"Energy Deposition of positron : "<<PositronEdep<<endl;
    //   }
    // }

    // else if(ParticleName == "proton")
    // {
    //   cout<<"there are "<< ParticleName<<endl;
    //   test = track -> GetKineticEnergy();
    //   cout<<"kinetic energy of "<<ParticleName<<" : "<<test<<endl;

    //   // test01 = track -> GetStep() -> GetTotalEnergyDeposit();
    //   // cout<<"kinetic energy of step : "<<test01<<endl;

    //   // G4double preKin = track -> GetStep() -> GetPreStepPoint() -> GetKineticEnergy();
    //   // cout<<"kinetic energy of preStep : "<<preKin<<endl;

    //   // G4double pstKin = track -> GetStep() -> GetPostStepPoint() -> GetKineticEnergy();
    //   // cout<<"kinetic energy of postStep : "<<pstKin<<endl;
    //   if(!test)
    //   {
    //     cout<<"there are no deposit energy of "<<ParticleName<<endl;
    //     break;
    //   }
    //   else
    //   {
    //     ProtonEdep = track -> GetStep() -> GetTotalEnergyDeposit();
    //     cout<<"Energy Deposition of proton : "<<ProtonEdep<<endl;
    //   }
    // }

    // else if(ParticleName == "C12")
    // {
    //   cout<<"there are "<< ParticleName<<endl;
    //   test = track -> GetKineticEnergy();
    //   cout<<"Kinetic Enrgy of "<<ParticleName<<" : "<<test<<endl;
    //   if(!(test))
    //   {
    //     cout<<"there are no deposit energy of "<<ParticleName<<endl;
    //     break;
    //   }
    //   else
    //   {
    //     C12Edep = track -> GetStep() -> GetTotalEnergyDeposit();
    //     cout<<"Energy Deposition of C12 : "<<C12Edep<<endl;
    //   }
    // }

    // else
    //  {
    //   cout<<"there are "<< ParticleName<<endl;
    //   test = track -> GetKineticEnergy();
    //   if(!(test))
    //   {
    //     cout<<"there are no deposit energy of "<<ParticleName<<endl;
    //     break;
    //   }
    //   else
    //   {
    //     nucleus = track -> GetStep() -> GetTotalEnergyDeposit();
    //     cout<<" Energy Deposition of other nucleus : "<<nucleus<<endl;
    //   }
    //  }
   
  // }

  // cout<< "total energy deposition : "<< edep << endl;
  // cout<< "PreStep kinetic energy : "<< preStepKen <<endl;
  // cout<< "PostStep kinetic enrgy : "<< pstStepKen <<endl;

// cout<<"Store in hit collection"<<endl;
  hit -> SetInKEn(preStepKen);
  hit -> SetFnKEn(pstStepKen);
  hit -> AddEdep(edep);
  hit -> SetID(ID);
  hit -> SetNum(1);


if(Particle == "e-")
{
  // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  hit -> AddElectronEdep(edep);
}
else if(Particle == "e+")
{
  // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  hit -> AddPositronEdep(edep);
}
else if(Particle == "proton")
{
  // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  hit -> AddProtonEdep(edep);
}
else if(Particle == "C12")
{
  // cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  hit -> AddCarbon12Edep(edep);
}
else
{
  cout<<"Energy Deposition of "<<Particle<<" : "<<edep<<endl;
  hit -> AddNucleusEdep(edep);
}
// cout<<"store in hit collection : energy deposition"<<endl;

  fHitsCollection->insert(hit);
  cout<<endl;

  return true;

}

  /*cout<<"hit processing start"<<endl;
  auto ParOTrack = step -> GetTrack() -> GetParticleDefinition() -> GetParticleName();
  if(ParOTrack == "neutron")
  {
    cout<<"This step is for neutron :("<<endl;
    return false;
  }
  else
  {
  const std::vector< const G4Track*>*secondaries = step -> GetSecondaryInCurrentStep();
  auto nbtrk = (*secondaries).size();

  if(!(nbtrk))
  {
    cout<<"there are no secondary particle :("<<endl;
    return false;
  }
  else
  {
  cout<<"The Number of Secondary : "<<nbtrk<<endl;

  int count = 0;
  int proNum = 0;
  const G4Track* track;
  const G4Track* secTrack;

  for(int i=0; i<nbtrk; i++)
  {
    track = (*secondaries)[i];
    G4String ParticleName = track -> GetParticleDefinition() -> GetParticleName();

    if(ParticleName == "proton")
    {
      cout<<"there are proton !!"<<endl;
      proNum = count;
      secTrack = (*secondaries)[proNum];
      break;
    }
    else if(ParticleName == "e-")
    {
      cout<<"there are electron !!"<<endl;
      proNum = count;
      secTrack = (*secondaries)[proNum];
      break;
    }
    else if(ParticleName == "C12")
    {
      cout<<"there are Carbon !!"<<endl;
      proNum = count;
      secTrack = (*secondaries)[proNum];
      break;
    }
     
      count += 1;
    
  }

  auto touchable = step->GetPreStepPoint()->GetTouchable();
  auto physical = touchable->GetVolume();
  auto copyNo = physical->GetCopyNo();

  auto trkKen = secTrack -> GetKineticEnergy();
  auto VtrkKen = secTrack -> GetVertexKineticEnergy();
  //auto ID = step -> GetTrack() -> GetTrackID();

  auto test01 = step -> GetPreStepPoint() -> GetKineticEnergy();
  auto test02 = step -> GetPostStepPoint() -> GetKineticEnergy();
  auto test03 = step ->GetTrack() -> GetStep() -> GetPreStepPoint() -> GetKineticEnergy();
  auto test04 = step ->GetTrack() -> GetStep() -> GetPostStepPoint() -> GetKineticEnergy();

  auto test05 = step -> GetTrack() -> GetKineticEnergy();
  auto test06 = step -> GetTrack() -> GetVertexKineticEnergy();
  cout<<"neutron track energy : "<<test05<<", neutron vertex track kinetic energy : "<<test06<<endl;

  auto ID = secTrack -> GetTrackID();

  cout<<"Kinetic Energy in Track : "<<trkKen<<endl;
  cout<<"Kintic Energy in Vertex of Track : "<<VtrkKen<<endl;
  cout<<"Track ID : "<<ID<<endl;

  cout<<"normal prestep energy : "<<test01<<" step-track-step prestep energy : "<<test03<<endl;
  cout<<"normal poststep energy : "<<test02<<" step-track-step poststep energy : "<<test04<<endl;
  
  //
  //
  auto analysisManager = G4AnalysisManager::Instance();
  auto hit = new NeutronHit(copyNo);

  hit -> SetInKEn(VtrkKen);
  hit -> SetFnKEn(trkKen);
  hit -> SetNum(1);


  fHitsCollection->insert(hit);

  cout<<"register the hit collection"<<endl;
  cout<<"one hit process is done"<<endl;

  return true;
  }
  }
}*/
