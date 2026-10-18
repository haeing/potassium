#include "PhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "NeutronHPphysics.hh"
#include "G4HadronPhysicsFTFP_BERT.hh" 
#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics.hh" 
#include "G4DecayPhysics.hh" 

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4VModularPhysicsList.hh"
#include "G4RunManager.hh"

#include "G4EmBuilder.hh"
PhysicsList::PhysicsList()
{
  SetVerboseLevel(0);
  
  //add new units
  //
  new G4UnitDefinition( "mm2/g",  "mm2/g", "Surface/Mass", mm2/g);
  new G4UnitDefinition( "um2/mg", "um2/mg","Surface/Mass", um*um/mg);  
    
  // Neutron Physics
  RegisterPhysics( new NeutronHPphysics("neutronHP"));  
  //RegisterPhysics( new FTFP_BERT() );
  //RegisterPhysics( new G4HadronPhysicsFTFP_BERT()); 
  RegisterPhysics( new G4EmStandardPhysics() );
  RegisterPhysics( new G4DecayPhysics() );

  //G4RunManager::GetRunManager()->SetUserInitialization(physicsList); 
  //AddTransportation();

}

void PhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  


}

void PhysicsList::SetCuts()
{
  //SetCutValue(0*mm, "proton");
}