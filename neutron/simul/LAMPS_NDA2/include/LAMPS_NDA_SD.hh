
#ifndef LAMPS_NDA_SD_h
#define LAMPS_NDA_SD_h 1

#include "G4VSensitiveDetector.hh"
#include "LAMPS_NDA_Hit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class LAMPS_NDA_SD : public G4VSensitiveDetector
{
  public:
    LAMPS_NDA_SD();
    LAMPS_NDA_SD(G4String name);
    ~LAMPS_NDA_SD() override = default;

    void Initialize(G4HCofThisEvent*HCE) override;
    G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist) override;

  private:
    NDAHitsCollection* fHitsCollection01 = nullptr;
    G4int fHCID = -1;
};


#endif
