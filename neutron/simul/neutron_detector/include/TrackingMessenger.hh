#ifndef TrackingMessenger_h
#define TrackingMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class TrackingAction;
class G4UIcmdWithABool;
class G4UIcommand;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingMessenger: public G4UImessenger
{
  public:
    TrackingMessenger(TrackingAction*);
   ~TrackingMessenger() override;
    
    void SetNewValue(G4UIcommand*, G4String) override;
    
  private:
    TrackingAction*   fTrackingAction = nullptr;    
    G4UIcmdWithABool* fTrackingCmd    = nullptr;
    G4UIcommand*      fTimeWindowCmd  = nullptr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif