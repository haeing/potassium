#include "PrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4IonTable.hh"
#include "G4Geantino.hh"

#include "TGenPhaseSpace.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"

#include "TRandom3.h"
#include "G4Neutron.hh"
#include "PairSpinEventInfo.hh"

using std::cout;
using std::endl;

double MigdalWatson(double* xx, double* par)
{
    double E = xx[0];

    double ann = -18.5;

    double epsil0 = 41.5/TMath::Sq(ann);

    return TMath::Sqrt(E)/(E + epsil0);
}

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  gParticleGun = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

  G4ParticleDefinition* particle1 = particleTable->FindParticle("neutron");
  fParticleGun->SetParticleDefinition(particle1);
  
  G4ParticleDefinition* particle2 = G4Neutron::NeutronDefinition();
  gParticleGun -> SetParticleDefinition(particle2);

  MW = new TF1("Migdal_Watson", MigdalWatson, 0, 6, 0); //relative energy        

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete gParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //
  // Total Spin Assignment

  G4int PairSpin = (G4UniformRand() < 0.87) ? 0 : 1;

  anEvent -> SetUserInformation(new PairSpinEventInfo(PairSpin));

  //
  // reaction for 6He + p -> p + a + 2n

  // gRandom = new TRandom(0);   // random event generation  

  //
  // Primary Position
  double R = 3.1/2. *cm;
  double L = 13 *cm;

  double x0 = 0. *cm;
  double y0 = 0. *cm;
  double z0 = (-1003.5 -0.5) *cm;

  double u1 = G4UniformRand();
  double u2 = G4UniformRand();
  double u3 = G4UniformRand(); 

  double r = R *TMath::Sqrt(u1);
  double angle = 2*TMath::Pi()*u2;

  double z = z0 +L*(u3 -0.5);
  double x = x0 +r*TMath::Cos(angle);
  double y = y0 +r*TMath::Sin(angle);

  //

  const double mp  = 938.2720813;
  const double mn  = 939.5654133;
  const double ma = 3727.379378;
  const double m6He = 5605.54;

  double TperA = 200.;
  double Tbeam = 4.0*TperA;
  double Ebeam = ma + Tbeam;
  double pbeam = TMath::Sqrt(TMath::Sq(Ebeam) - TMath::Sq(ma));

  double betabeam = pbeam/TMath::Sqrt(TMath::Sq(ma) + TMath::Sq(pbeam));
  TVector3 betabeamV(0.,0., betabeam);

  // TRandom3 r1, r2, r3;
  // int timer = (int)time(0);
  double pa[3], pnn[3];

  // r1.SetSeed(timer);
  // r2.SetSeed(timer +1);
  // r3.SetSeed(timer +2);

  //

  for(int i=0; i<3; i++)
  {
    pa[i] = G4RandGauss::shoot(0.,30.);
    pnn[i] = -pa[i];
  }

  // pa[0] = r1.Gaus(0., 20.);
  // pnn[0] = -pa[0];

  // pa[1] = r2.Gaus(0., 20.);
  // pnn[1] = -pa[1];

  // pa[2] = r3.Gaus(0., 20.);
  // pnn[2] = -pa[2];    

  double Ea = TMath::Sqrt(TMath::Sq(ma) +TMath::Sq(pa[0]) +TMath::Sq(pa[1]) +TMath::Sq(pa[2]));
  
  TLorentzVector beam(pa[0], pa[1], pa[2], Ea);
  beam.Boost(betabeamV);

  TLorentzVector target(0., 0., 0., mp);
  TLorentzVector P_init = beam + target;

  //

  double Erel0 = MW -> GetRandom();  
  double M2n = 2*mn + Erel0;
  double E2n = TMath::Sqrt(TMath::Sq(M2n) +TMath::Sq(pnn[0]) +TMath::Sq(pnn[1]) +TMath::Sq(pnn[2]));  

  TLorentzVector P2n(pnn[0], pnn[1], pnn[2], E2n);
  P2n.Boost(betabeamV);

  Double_t masses2[2] = {mn, mn};

  TGenPhaseSpace decay2;

  if(!decay2.SetDecay(P2n, 2, masses2))
  {
      cout<<"Second decay failed"<<endl;
      return;
  }

  //
  // Decay for 6He(p, pa)2n(-> n + n)

  double weight2 = decay2.Generate();

  TLorentzVector n1 = *decay2.GetDecay(0);
  TLorentzVector n2 = *decay2.GetDecay(1);

  Tn1 = n1.E() - mn;
  theta1 = n1.Theta();
  phi1 = n1. Phi();  

  G4ThreeVector dir1(TMath::Sin(theta1)*TMath::Cos(phi1), TMath::Sin(theta1)*TMath::Sin(phi1), TMath::Cos(theta1));
 
  fParticleGun->SetParticleEnergy(Tn1 *MeV);
  fParticleGun->SetParticleMomentumDirection(dir1);
  fParticleGun->SetParticlePosition(G4ThreeVector(x, y, z));  
  
  //

  Tn2 = n2.E() - mn;  
  theta2 = n2.Theta();
  phi2 = n2. Phi(); 

  G4ThreeVector dir2(TMath::Sin(theta2)*TMath::Cos(phi2), TMath::Sin(theta2)*TMath::Sin(phi2), TMath::Cos(theta2));

  gParticleGun->SetParticleEnergy(Tn2 *MeV);
  gParticleGun->SetParticleMomentumDirection(dir2);
  gParticleGun->SetParticlePosition(G4ThreeVector(x, y, z)); 

  //

  fParticleGun->GeneratePrimaryVertex(anEvent);
  gParticleGun->GeneratePrimaryVertex(anEvent);
}
