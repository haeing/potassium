#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"


class RunAction;

/// Event action class

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void TotalKineticEnergy(G4double KE);

  private:
  int NeutronHCID01 = 0;
  int NeutronHCID02 = 0;
  G4double totnum =0.;
  G4double secnum =0.;
    int count = 0;
  G4double TotKE = 0.;
    //string particle ;

};


#endif