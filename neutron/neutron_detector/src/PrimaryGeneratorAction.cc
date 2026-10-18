#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

using std::cout;
using std::endl;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    cout<<"initialize the gun"<<endl;
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="neutron");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

  /*auto ranval = G4UniformRand();
  cout<<"Random Value : "<<ranval<<endl;
  auto minE = 0.1*MeV;
  auto maxE = 1.6*MeV;

  auto randE = minE + ranval*(maxE - minE);
  cout<<"Random Energy : "<<randE<<endl;
  fParticleGun->SetParticleEnergy(randE*MeV);*/
  fParticleGun->SetParticleEnergy(1.8*MeV);
  cout<<"end of initialize the gun"<<endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

//cout<<"generate the gun"<<endl;
  fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-5*cm));

  fParticleGun->GeneratePrimaryVertex(anEvent);
  //cout<<"end of generating the gun"<<endl;
}
