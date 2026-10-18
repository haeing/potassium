
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

//#include "G4Isotope.hh"
//#include "G4Element.hh"
#include "G4Material.hh"

#include "G4SDManager.hh"
#include "NeutronSD.hh"
#include "G4VSensitiveDetector.hh"


G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4Element* C   = new G4Element("Carbon","C", 6., 12.011*g/mole);
  G4Element* H   = new G4Element("Hydrogen","H", 1., 1.01*g/mole);

  G4Material* polyvinyltoluene = new G4Material("polyvinyltoluene", 1.032*g/cm3, 2, kStateSolid, 298*kelvin, 1*atmosphere);

  polyvinyltoluene->AddElement(C, 9);
  polyvinyltoluene->AddElement(H, 10);  

  G4bool checkOverlaps = true;



  //
  // World
  //
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* BC408 = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  auto solidWorld = new G4Box("World", 100*cm, 100*cm, 100*cm);  

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    world_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

  //
  // Scintillator
  //
  auto solidSc = new G4Box("Scintillator", 10*cm, 5*cm, 5*cm); 

  logicSc = new G4LogicalVolume(solidSc, BC408, "Scintillator");                                

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),          // at (0,0,0)
    logicSc,                 // its logical volume
    "Scintillator",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  //
  //always return the physical World
  //
  return physWorld;
}


void DetectorConstruction::ConstructSDandField()
{

  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname;

  auto NeutronDetector = new NeutronSD(SDname="/neutrondetector");
  sdManager->AddNewDetector(NeutronDetector);
  logicSc->SetSensitiveDetector(NeutronDetector);

}
