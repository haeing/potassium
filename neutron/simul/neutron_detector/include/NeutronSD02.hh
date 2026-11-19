
#ifndef NeutronSD02_h
#define NeutronSD02_h 1

#include "G4VSensitiveDetector.hh"
#include "NeutronHit02.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class NeutronSD02 : public G4VSensitiveDetector
{
  public:
    NeutronSD02();
    NeutronSD02(G4String name);
    ~NeutronSD02() override = default;

    void Initialize(G4HCofThisEvent*HCE) override;
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;

  private:
    NeutronHitsCollection02* fHitsCollection02 = nullptr;
    G4int fHCID = -1;
};


#endif
