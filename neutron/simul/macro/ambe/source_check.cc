#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4UserSteppingAction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <stdexcept>

class Geometry : public G4VUserDetectorConstruction {
 public:
  G4VPhysicalVolume* Construct() override {
    auto nist = G4NistManager::Instance();
    auto world = new G4LogicalVolume(new G4Box("World",10*m,10*m,10*m),
        nist->FindOrBuildMaterial("G4_AIR"),"World");
    auto physical = new G4PVPlacement(nullptr,{},world,"World",nullptr,false,0,true);
    auto source = new G4LogicalVolume(new G4Tubs("Source",0,2.5*mm,2.5*mm,0,360*deg),
        nist->FindOrBuildMaterial("G4_Be"),"Source");
    new G4PVPlacement(nullptr,{0,5*cm,-205.25*cm},source,"Source",world,false,0,true);
    return physical;
  }
};

// Record neutron energy on its first exit from the bare Be source.
// All tracks are stopped at source exit, excluding room return.
class SourceSpectrum : public G4UserSteppingAction {
 public:
  explicit SourceSpectrum(TTree& tree): tree_(tree) {
    tree_.Branch("energy_MeV", &energy_, "energy_MeV/D");
  }
  void UserSteppingAction(const G4Step* step) override {
    auto pre=step->GetPreStepPoint()->GetPhysicalVolume();
    auto post=step->GetPostStepPoint()->GetPhysicalVolume();
    if(!pre || pre->GetName()!="Source" || (post && post->GetName()=="Source")) return;
    auto track=step->GetTrack();
    if(track->GetDefinition()->GetParticleName()=="neutron") {
      energy_=step->GetPostStepPoint()->GetKineticEnergy()/MeV;
      tree_.Fill();
    }
    track->SetTrackStatus(fStopAndKill);
  }
 private:
  TTree& tree_;
  double energy_=0;
};

int main(int argc,char** argv) {
  try {
    if(argc!=4) {
      std::cerr << "Usage: AmBeSourceCheck EVENTS SEED OUTPUT.root\n";
      return 2;
    }
    const int events=std::stoi(argv[1]);
    const long seed=std::stol(argv[2]);
    if(events<=0 || seed<=0) throw std::runtime_error("EVENTS and SEED must be positive");
    TFile output(argv[3], "RECREATE");
    if(output.IsZombie()) throw std::runtime_error("Cannot open ROOT output");
    TTree tree("neutrons", "Neutrons leaving the AmBe source");
    tree.SetDirectory(nullptr);
    G4Random::setTheEngine(new CLHEP::MTwistEngine);
    G4Random::setTheSeed(seed);
    auto run=new G4RunManager;
    run->SetUserInitialization(new Geometry);
    run->SetUserInitialization(new PhysicsList);
    run->SetUserAction(new PrimaryGeneratorAction);
    run->SetUserAction(new SourceSpectrum(tree));
    run->Initialize();
    if(G4UImanager::GetUIpointer()->ApplyCommand(
        "/process/had/rdm/thresholdForVeryLongDecayTime 5000 year")!=0)
      throw std::runtime_error("Cannot set decay time threshold");
    run->BeamOn(events);
    output.cd();
    tree.Write();
    output.Close();
    delete run;
  } catch(const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
