#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

#include "G4ThreeVector.hh"
#include <map>

using namespace std;

class RunAction;

/// Event action class

struct sHitRec
{
  G4int ID;
  G4int pID;
  G4int Hitcount;
  G4int Target;
  G4int SecAtomic;
  G4int CopyNo;

  G4double Time;
  G4double preKE;
  G4double pstKE;
  G4double Edep;
  G4double vrtxKE;
  G4double Length;

  G4ThreeVector HitPos;
  G4ThreeVector preMom;
  G4ThreeVector pstMom;
  G4ThreeVector vrtxPos;

  G4String ParticleName;
};

struct sEventRec
{
  G4int ID;
  G4int pID;
  G4int Hitcount;
  G4int Target;
  G4int SecAtomic;
  G4int CopyNo;

  G4double Time;
  G4double preKE;
  G4double pstKE;
  G4double Edep;
  G4double vrtxKE;
  G4double Length;

  G4ThreeVector HitPos;
  G4ThreeVector preMom;
  G4ThreeVector pstMom;
  G4ThreeVector vrtxPos;  
};

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    // ~EventAction() override = default;
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

  private:

  int St01Veto[20];
  int St01NDv01[20];
  int St01NDh01[20];
  int St01NDv02[20];
  int St01NDh02[20];

  int St02Veto[20];
  int St02NDv01[20];
  int St02NDh01[20];
  int St02NDv02[20];
  int St02NDh02[20];

  //

  vector<sHitRec> HitRec;

  vector<sHitRec> EventN01;
  vector<sHitRec> EventN02;

  vector<vector<sHitRec>> EventSec01;
  vector<vector<sHitRec>> EventSec02;

  vector<G4double> EventN01Edep;
  vector<G4double> EventN02Edep;

  vector<sEventRec> EventRecN01;
  vector<sEventRec> EventRecN02;

  //

  G4double Arr_n1KE[200];
  G4double Arr_n1MomX[200];
  G4double Arr_n1MomY[200];
  G4double Arr_n1MomZ[200];
  G4double Arr_n1Time[200];
  G4double Arr_n1X[200];
  G4double Arr_n1Y[200];
  G4double Arr_n1Z[200];
  G4double Arr_n1VrtxX[200];
  G4double Arr_n1VrtxY[200];
  G4double Arr_n1VrtxZ[200];
  G4double Arr_n1Edep[200];
  G4double Arr_n1Target[200];
  G4double Arr_n1HitCount[200];

  G4double Arr_n2KE[200];
  G4double Arr_n2MomX[200];
  G4double Arr_n2MomY[200];
  G4double Arr_n2MomZ[200];
  G4double Arr_n2Time[200];
  G4double Arr_n2X[200];
  G4double Arr_n2Y[200];
  G4double Arr_n2Z[200];
  G4double Arr_n2VrtxX[200];
  G4double Arr_n2VrtxY[200];
  G4double Arr_n2VrtxZ[200];  
  G4double Arr_n2Edep[200];
  G4double Arr_n2Target[200];  
  G4double Arr_n2HitCount[200];

  G4int TotalSpin;

  TFile *RootFile = nullptr;
  TTree *tree = nullptr;    
};


#endif