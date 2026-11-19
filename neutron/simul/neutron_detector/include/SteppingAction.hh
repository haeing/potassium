#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <set>

class EventAction;
class G4Event;

class SteppingAction : public G4UserSteppingAction
{
  public:
    SteppingAction();
   ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step*) override;
    
  private:
    const G4Event* fEvent = nullptr;
    G4int fEventID = -1;
    G4int fRunID = -1;
    std::set<G4int> fEscapedNeutrons;
};



#endif
