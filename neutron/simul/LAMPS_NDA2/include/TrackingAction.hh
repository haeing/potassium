#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class EventAction;
class TrackingMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

  public:  
    TrackingAction(EventAction*);
   ~TrackingAction() override;
   
    void  PreUserTrackingAction(const G4Track*) override;
    void PostUserTrackingAction(const G4Track*) override;
    
    void SetFullChain(G4bool flag) { fFullChain = flag;};
    void SetTimeWindow(G4double , G4double);
    
  private:
    EventAction*        fEvent = nullptr;
    TrackingMessenger*  fTrackMessenger = nullptr;
    
    G4double fCharge = 0., fMass = 0.;        
    G4bool   fFullChain = true;

    G4double fTimeBirth = 0.,   fTimeEnd = 0.;
    G4double fTimeWindow1 = 0., fTimeWindow2 = 0.;
};



#endif