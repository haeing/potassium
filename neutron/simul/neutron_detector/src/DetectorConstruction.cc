
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"

#include "G4SDManager.hh"
#include "NeutronSD01.hh"
#include "NeutronSD02.hh"
#include "G4VSensitiveDetector.hh"


G4VPhysicalVolume* DetectorConstruction::Construct()
{

  G4Element* C   = new G4Element("Carbon","C", 6., 12.011*g/mole);
  G4Element* H   = new G4Element("Hydrogen","H", 1., 1.01*g/mole);

  auto O = new G4Element("Oxygen","O",8, 16.00 *g/mole);
  auto Be = new G4Element("Beryllium","Be" ,  4.,  9.01*g/mole);

  auto BeO = new G4Material("BeO", 3.05*g/cm3, 2);
  BeO->AddElement(Be, 1);
  BeO->AddElement( O, 1);

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
  G4Material* Be9 = nist -> FindOrBuildMaterial("G4_Be");

  auto solidWorld = new G4Box("World", 1000*cm, 1000*cm, 1000*cm);  

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
  // Scintillator 01(below one)
  //
  auto solidSc01 = new G4Box("Scintillator01", (20*cm)/2, (10*cm)/2, (10*cm)/2); 

  logicSc01 = new G4LogicalVolume(solidSc01, BC408, "Scintillator01");                                

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),          // at (0,0,0)
    logicSc01,                 // its logical volume
    "Scintillator01",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking


  //
  // Scintillator 02(above one)
  //
  auto solidSc02 = new G4Box("Scintillator02", (20*cm)/2, (10*cm)/2, (10*cm)/2); 

  logicSc02 = new G4LogicalVolume(solidSc02, BC408, "Scintillator02");                                

  new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(0,10 *cm, 0),          // at (0,0,0)
    logicSc02,                 // its logical volume
    "Scintillator02",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking



  //Be for Am-Be source
  auto Be_solid
    = new G4Tubs("Am-Be Source",0, (5 *mm)/2, (0.5 *cm)/2, 0., 360 *deg);
  
  auto Be_logic
    = new G4LogicalVolume(Be_solid, Be9, "Am-Be Source");

  // new G4PVPlacement(0, G4ThreeVector(0,0, -(200) *cm), Be_logic,
  new G4PVPlacement(0, G4ThreeVector(0,5 *cm, -(200 +5.25) *cm), Be_logic,
                    "Am-Be source", logicWorld, false, 0, checkOverlaps);

  return physWorld;
}


void DetectorConstruction::ConstructSDandField()
{

  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname01, SDname02;

  auto NeutronDetector01 = new NeutronSD01(SDname01="/neutrondetector01");
  sdManager->AddNewDetector(NeutronDetector01);
  logicSc01->SetSensitiveDetector(NeutronDetector01);

  auto NeutronDetector02 = new NeutronSD02(SDname02="/neutrondetector02");
  sdManager->AddNewDetector(NeutronDetector02);
  logicSc02->SetSensitiveDetector(NeutronDetector02);

}
