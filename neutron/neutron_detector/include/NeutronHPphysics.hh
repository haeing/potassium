#ifndef NeutronHPphysics_h
#define NeutronHPphysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

class G4GenericMessenger;

class NeutronHPphysics : public G4VPhysicsConstructor
{
  public:
    NeutronHPphysics(const G4String& name="neutron");
   ~NeutronHPphysics() override;

  public:
    void ConstructParticle() override { };
    void ConstructProcess()  override;
    
  private:
    void DefineCommands();
      
    G4bool              fThermal = true;
    G4GenericMessenger* fMessenger = nullptr;
};

#endif