#include "LAMPS_NDA_Hit.hh"
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


G4ThreadLocal G4Allocator<LAMPS_NDA_Hit>* NDAHitAllocator;

LAMPS_NDA_Hit::LAMPS_NDA_Hit(int layerID)
: fLayerID(layerID)
{}

