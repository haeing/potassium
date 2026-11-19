
#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

#include "G4EmstandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
 ~PhysicsList() override = default;

public:
  void ConstructProcess() override;
  void SetCuts()override;

private:
   G4VPhysicsConstructor* fHadronElastic    = nullptr;
   G4VPhysicsConstructor* fHadronInelastic  = nullptr;
   G4VPhysicsConstructor* fIonElastic       = nullptr;
   G4VPhysicsConstructor* fIonInelastic     = nullptr;
   G4VPhysicsConstructor* fGammaNuclear     = nullptr;
   G4VPhysicsConstructor* fElectromagnetic  = nullptr;
   G4VPhysicsConstructor* fDecay            = nullptr;
   G4VPhysicsConstructor* fRadioactiveDecay = nullptr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
