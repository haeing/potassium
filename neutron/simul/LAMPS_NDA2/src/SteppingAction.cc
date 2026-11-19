#include "SteppingAction.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <string>

using namespace std;;
                           

SteppingAction::SteppingAction()
{ 
    
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{

}