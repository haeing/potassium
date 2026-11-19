#include "PrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4IonTable.hh"
#include "G4Geantino.hh"

using std::cout;
using std::endl;

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  // G4ParticleDefinition* particle
    // = particleTable->FindParticle(particleName="geantino");
    // = particleTable->FindParticle(particleName="neutron");
    // = particleTable->FindParticle(particleName="alpha");
    // = particleTable->FindParticle(particleName="proton");
  // fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  // fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-(200) *cm));
    // fParticleGun->SetParticlePosition(G4ThreeVector(0,0,-(45 + 0.2)/2 *cm));
  fParticleGun->SetParticlePosition(G4ThreeVector(0,5 *cm,-(200 +5.25) *cm));


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  fParticleGun->SetParticleEnergy(0 *MeV);
  // fParticleGun->SetParticleEnergy(19 *MeV);
  // fParticleGun->SetParticleEnergy(7 *MeV);


  G4int Z = 95, A = 241; //Am241
  G4double ionCharge   = 0.*eplus;
  G4double excitEnergy = 0.*keV;

  G4ParticleDefinition* Am241
     = G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);

  fParticleGun->SetParticleDefinition(Am241);
  fParticleGun->SetParticleCharge(ionCharge);

  fParticleGun->GeneratePrimaryVertex(anEvent);

}
