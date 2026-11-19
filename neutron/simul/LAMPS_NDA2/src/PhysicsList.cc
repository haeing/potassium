#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4NuclideTable.hh"

#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsINCLXX.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysicsXS.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

using namespace std;

PhysicsList::PhysicsList()
{
    G4int verb = 1;
    SetVerboseLevel(verb);

    //Hadron Elastic process
    RegisterPhysics(new G4HadronElasticPhysicsHP(verb));

    //Hadron Inelastic process and neutron Capture
    RegisterPhysics(new G4HadronPhysicsFTFP_BERT_HP(verb));


    // auto inclNeutron = new G4HadronPhysicsINCLXX( "INCLphysics", true, true, true );
    // RegisterPhysics(inclNeutron);

    //Ion Elastic process
    RegisterPhysics(new G4IonElasticPhysics(verb));

    //Ion Inelastic process
    RegisterPhysics(new G4IonPhysicsXS(verb));

    //EM process
    RegisterPhysics(new G4EmStandardPhysics(verb));

    //Decay process
    RegisterPhysics(new G4DecayPhysics());

    //Radioactive Decay process
    RegisterPhysics(new G4RadioactiveDecayPhysics());
}

// void PhysicsList::ConstructParticle()
// {
//   G4BosonConstructor  pBosonConstructor;
//   pBosonConstructor.ConstructParticle();

//   G4LeptonConstructor pLeptonConstructor;
//   pLeptonConstructor.ConstructParticle();

//   G4MesonConstructor pMesonConstructor;
//   pMesonConstructor.ConstructParticle();

//   G4BaryonConstructor pBaryonConstructor;
//   pBaryonConstructor.ConstructParticle();

//   G4IonConstructor pIonConstructor;
//   pIonConstructor.ConstructParticle();

//   G4ShortLivedConstructor pShortLivedConstructor;
//   pShortLivedConstructor.ConstructParticle();  
// }

void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
