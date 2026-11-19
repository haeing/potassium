#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

#include "TF1.h"

class G4ParticleGun;
class G4Event;
class G4Box;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    // method from the base class
    void GeneratePrimaries(G4Event*) override;

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun = nullptr; // pointer a to G4 gun class
    G4ParticleGun *gParticleGun = nullptr;

    TFile *RootFile = nullptr;
    TTree *tree = nullptr;

    int Entry = 0;

    double Tn1, Tn2, theta1, theta2, phi1, phi2;

    G4Box* fEnvelopeBox = nullptr;

    TF1 *MW;
};

#endif