
#ifndef NeutronSD_h
#define NeutronSD_h 1

#include "G4VSensitiveDetector.hh"
#include "NeutronHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class NeutronSD : public G4VSensitiveDetector
{
  public:
    NeutronSD();
    NeutronSD(G4String name);
    ~NeutronSD() override = default;

    void Initialize(G4HCofThisEvent*HCE) override;
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;

  private:
    NeutronHitsCollection* fHitsCollection = nullptr;
    G4int fHCID = -1;
};


#endif
