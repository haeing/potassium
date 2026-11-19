
#ifndef NeutronSD01_h
#define NeutronSD01_h 1

#include "G4VSensitiveDetector.hh"
#include "NeutronHit01.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class NeutronSD01 : public G4VSensitiveDetector
{
  public:
    NeutronSD01();
    NeutronSD01(G4String name);
    ~NeutronSD01() override = default;

    void Initialize(G4HCofThisEvent*HCE) override;
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;

  private:
    NeutronHitsCollection01* fHitsCollection01 = nullptr;
    G4int fHCID = -1;
};


#endif
