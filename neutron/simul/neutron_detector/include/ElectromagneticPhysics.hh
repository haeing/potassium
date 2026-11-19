
#ifndef ElectromagneticPhysics_h
#define ElectromagneticPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ElectromagneticPhysics : public G4VPhysicsConstructor
{
  public: 
    ElectromagneticPhysics(const G4String& name = "standard");
   ~ElectromagneticPhysics() override = default;

  public: 
    // This method is dummy for physics
    void ConstructParticle() override {};
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    void ConstructProcess() override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

