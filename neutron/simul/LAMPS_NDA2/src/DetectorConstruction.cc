
#include "DetectorConstruction.hh"
#include <string>

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
#include "LAMPS_NDA_SD.hh"
#include "G4VSensitiveDetector.hh"

using namespace std;


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

  G4bool checkOverlaps = false;



  //
  // World
  //
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* BC408 = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  G4Material* Be9 = nist -> FindOrBuildMaterial("G4_Be");

  auto solidWorld = new G4Box("World", 2000*cm, 2000*cm, 2000*cm);  

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
  // Stage01
  //

    //
    // Veto 01

    G4Box *solidSt01Veto01[20];

    for(int i=0; i<20; i++)
    {
      solidSt01Veto01[i] = new G4Box("St01Veto01_" + to_string(i),
                                 (10 *cm)/2, (200 *cm)/2, (1 *cm)/2);

      logicSt01Veto01[i] = new G4LogicalVolume(solidSt01Veto01[i], BC408, "St01Veto01_" + to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, 0),
                        logicSt01Veto01[i],
                        "St01Veto01_" + to_string(i),
                        logicWorld,
                        false,
                        i+1,
                        checkOverlaps);
    }

    //
    // ND vertical01

    G4Box *solidSt01NDv01[20];

    for(int i=0; i<20; i++)
    {
      solidSt01NDv01[i] = new G4Box("St01NDv01_" +to_string(i),
                                (10 *cm)/2, (200 *cm)/2, (10 *cm)/2);

      logicSt01NDv01[i] = new G4LogicalVolume(solidSt01NDv01[i], BC408, "St01NDv01_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, 13.5 *cm),
                        logicSt01NDv01[i],
                        "St01NDv01_" +to_string(i),
                        logicWorld,
                        false,
                        21 +i,
                        checkOverlaps);      
    }

    //
    // ND horizontal01

    G4Box *solidSt01NDh01[20];

    for(int i=0; i<20; i++)
    {
      solidSt01NDh01[i] = new G4Box("St01NDh01_" +to_string(i),
                                (200 *cm)/2, (10 *cm)/2, (10 *cm)/2);

      logicSt01NDh01[i] = new G4LogicalVolume(solidSt01NDh01[i], BC408, "St01NDh01_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector(0, (-95 +10*i) *cm, 24. *cm),
                        logicSt01NDh01[i],
                        "St01NDh01_" +to_string(i),
                        logicWorld,
                        false,
                        41 +i,
                        checkOverlaps);      
    }    

    //
    // ND vertical02

    G4Box *solidSt01NDv02[20];

    for(int i=0; i<20; i++)
    {
      solidSt01NDv02[i] = new G4Box("St01NDv02_" +to_string(i),
                                (10 *cm)/2, (200 *cm)/2, (10 *cm)/2);

      logicSt01NDv02[i] = new G4LogicalVolume(solidSt01NDv02[i], BC408, "St01NDv02_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, 35. *cm),
                        logicSt01NDv02[i],
                        "St01NDv02_" +to_string(i),
                        logicWorld,
                        false,
                        61 +i,
                        checkOverlaps);      
    }

    //
    // ND horizontal02

    G4Box *solidSt01NDh02[20];

    for(int i=0; i<20; i++)
    {
      solidSt01NDh02[i] = new G4Box("St01NDh02_" +to_string(i),
                                (200 *cm)/2, (10 *cm)/2, (10 *cm)/2);

      logicSt01NDh02[i] = new G4LogicalVolume(solidSt01NDh02[i], BC408, "St01NDh02_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector(0, (-95 +10*i) *cm, 45.5 *cm),
                        logicSt01NDh02[i],
                        "St01NDh02_" +to_string(i),
                        logicWorld,
                        false,
                        81 +i,
                        checkOverlaps);      
    }    

  //
  // Stage02
  //

  double Stage02_offset = (45.5 + 34.5) *cm;

    //
    // Veto 01

    G4Box *solidSt02Veto01[20];

    for(int i=0; i<20; i++)
    {
      solidSt02Veto01[i] = new G4Box("St02Veto01_" + to_string(i),
                                 (10 *cm)/2, (200 *cm)/2, (1 *cm)/2);

      logicSt02Veto01[i] = new G4LogicalVolume(solidSt02Veto01[i], BC408, "St02Veto01_" + to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, Stage02_offset +0),
                        logicSt02Veto01[i],
                        "St02Veto01_" + to_string(i),
                        logicWorld,
                        false,
                        101 +i,
                        checkOverlaps);
    }

    //
    // ND vertical01

    G4Box *solidSt02NDv01[20];

    for(int i=0; i<20; i++)
    {
      solidSt02NDv01[i] = new G4Box("St02NDv01_" +to_string(i),
                                (10 *cm)/2, (200 *cm)/2, (10 *cm)/2);

      logicSt02NDv01[i] = new G4LogicalVolume(solidSt02NDv01[i], BC408, "St02NDv01_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, Stage02_offset +13.5 *cm),
                        logicSt02NDv01[i],
                        "St02NDv01_" +to_string(i),
                        logicWorld,
                        false,
                        121 +i,
                        checkOverlaps);      
    }

    //
    // ND horizontal01

    G4Box *solidSt02NDh01[20];

    for(int i=0; i<20; i++)
    {
      solidSt02NDh01[i] = new G4Box("St02NDh01_" +to_string(i),
                                (200 *cm)/2, (10 *cm)/2, (10 *cm)/2);

      logicSt02NDh01[i] = new G4LogicalVolume(solidSt02NDh01[i], BC408, "St02NDh01_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector(0, (-95 +10*i) *cm, Stage02_offset +24. *cm),
                        logicSt02NDh01[i],
                        "St02NDh01_" +to_string(i),
                        logicWorld,
                        false,
                        141 +i,
                        checkOverlaps);      
    }    

    //
    // ND vertical02

    G4Box *solidSt02NDv02[20];

    for(int i=0; i<20; i++)
    {
      solidSt02NDv02[i] = new G4Box("St02NDv02_" +to_string(i),
                                (10 *cm)/2, (200 *cm)/2, (10 *cm)/2);

      logicSt02NDv02[i] = new G4LogicalVolume(solidSt02NDv02[i], BC408, "St02NDv02_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector((-95 +10*i) *cm, 0, Stage02_offset +35. *cm),
                        logicSt02NDv02[i],
                        "St02NDv02_" +to_string(i),
                        logicWorld,
                        false,
                        161 +i,
                        checkOverlaps);      
    }

    //
    // ND horizontal02

    G4Box *solidSt02NDh02[20];

    for(int i=0; i<20; i++)
    {
      solidSt02NDh02[i] = new G4Box("St02NDh02_" +to_string(i),
                                (200 *cm)/2, (10 *cm)/2, (10 *cm)/2);

      logicSt02NDh02[i] = new G4LogicalVolume(solidSt02NDh02[i], BC408, "St02NDh02_" +to_string(i));

      new G4PVPlacement(nullptr,
                        G4ThreeVector(0, (-95 +10*i) *cm, Stage02_offset +45.5 *cm),
                        logicSt02NDh02[i],
                        "St02NDh02_" +to_string(i),
                        logicWorld,
                        false,
                        181 +i,
                        checkOverlaps);      
    }      


  return physWorld;
}


void DetectorConstruction::ConstructSDandField()
{

  auto sdManager = G4SDManager::GetSDMpointer();
  G4String SDname01, SDname02;
  G4String VetoName01[20], St01NDv01Name[20], St01NDh01Name[20], St01NDv02Name[20], St01NDh02Name[20], 
           VetoName02[20], St02NDv01Name[20], St02NDh01Name[20], St02NDv02Name[20], St02NDh02Name[20];

  LAMPS_NDA_SD *St01Veto[20], *St01NDv01[20], *St01NDh01[20], *St01NDv02[20], *St01NDh02[20],
               *St02Veto[20], *St02NDv01[20], *St02NDh01[20], *St02NDv02[20], *St02NDh02[20];


  for(int i=0; i<20; i++)
  {
    St01Veto[i] = new LAMPS_NDA_SD(VetoName01[i] = "/St01Veto_" +to_string(i));
    sdManager->AddNewDetector(St01Veto[i]);
    logicSt01Veto01[i]->SetSensitiveDetector(St01Veto[i]);  

    St01NDv01[i] = new LAMPS_NDA_SD(St01NDv01Name[i] = "/St01NDv01_" +to_string(i));
    sdManager->AddNewDetector(St01NDv01[i]);
    logicSt01NDv01[i]->SetSensitiveDetector(St01NDv01[i]); 

    St01NDh01[i] = new LAMPS_NDA_SD(St01NDh01Name[i] = "/St01NDh01_" +to_string(i));
    sdManager->AddNewDetector(St01NDh01[i]);
    logicSt01NDh01[i]->SetSensitiveDetector(St01NDh01[i]); 

    St01NDv02[i] = new LAMPS_NDA_SD(St01NDv02Name[i] = "/St01NDv02_" +to_string(i));
    sdManager->AddNewDetector(St01NDv02[i]);
    logicSt01NDv02[i]->SetSensitiveDetector(St01NDv02[i]); 

    St01NDh02[i] = new LAMPS_NDA_SD(St01NDh02Name[i] = "/St01NDh02_" +to_string(i));
    sdManager->AddNewDetector(St01NDh02[i]);
    logicSt01NDh02[i]->SetSensitiveDetector(St01NDh02[i]); 

    //

    St02Veto[i] = new LAMPS_NDA_SD(VetoName02[i] = "/St02Veto_" +to_string(i));
    sdManager->AddNewDetector(St02Veto[i]);
    logicSt02Veto01[i]->SetSensitiveDetector(St02Veto[i]); 

    St02NDv01[i] = new LAMPS_NDA_SD(St02NDv01Name[i] = "/St02NDv01_" +to_string(i));
    sdManager->AddNewDetector(St02NDv01[i]);
    logicSt02NDv01[i]->SetSensitiveDetector(St02NDv01[i]); 

    St02NDh01[i] = new LAMPS_NDA_SD(St02NDh01Name[i] = "/St02NDh01_" +to_string(i));
    sdManager->AddNewDetector(St02NDh01[i]);
    logicSt02NDh01[i]->SetSensitiveDetector(St02NDh01[i]); 

    St02NDv02[i] = new LAMPS_NDA_SD(St02NDv02Name[i] = "/St02NDv02_" +to_string(i));
    sdManager->AddNewDetector(St02NDv02[i]);
    logicSt02NDv02[i]->SetSensitiveDetector(St02NDv02[i]); 

    St02NDh02[i] = new LAMPS_NDA_SD(St02NDh02Name[i] = "/St02NDh02_" +to_string(i));
    sdManager->AddNewDetector(St02NDh02[i]);
    logicSt02NDh02[i]->SetSensitiveDetector(St02NDh02[i]);                                       
  }

}
