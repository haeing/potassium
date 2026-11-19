#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

  private:

  G4LogicalVolume* logicSc01 = nullptr;
  G4LogicalVolume* logicSc02 = nullptr;

  G4LogicalVolume *logicSt01Veto01[20];
  G4LogicalVolume *logicSt01NDv01[20];
  G4LogicalVolume *logicSt01NDh01[20];
  G4LogicalVolume *logicSt01NDv02[20];
  G4LogicalVolume *logicSt01NDh02[20];  

  G4LogicalVolume *logicSt02Veto01[20];
  G4LogicalVolume *logicSt02NDv01[20];
  G4LogicalVolume *logicSt02NDh01[20];
  G4LogicalVolume *logicSt02NDv02[20];
  G4LogicalVolume *logicSt02NDh02[20];    

};


#endif