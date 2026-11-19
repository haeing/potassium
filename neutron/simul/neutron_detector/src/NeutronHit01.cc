#include "NeutronHit01.hh"
#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"


G4ThreadLocal G4Allocator<NeutronHit01>* NeutronHitAllocator01;

NeutronHit01::NeutronHit01(int layerID)
: fLayerID(layerID)
{}

