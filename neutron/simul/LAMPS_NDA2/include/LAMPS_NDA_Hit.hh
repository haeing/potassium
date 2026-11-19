#ifndef LAMPS_NDA_Hit_h
#define LAMPS_NDA_Hit_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class LAMPS_NDA_Hit : public G4VHit {
 public:
  LAMPS_NDA_Hit() = default;
  LAMPS_NDA_Hit(int layerID);
  ~LAMPS_NDA_Hit() override = default;

  inline void* operator new(size_t);
  inline void operator delete(void* aHit);

  void SetID(G4int id) {ID = id;}
  G4int GetID() {return ID;}

  void SetParentID(G4int pid) {pID = pid;}
  G4int GetParentID() {return pID;}

  void SetHitCount(G4int hcount) {hCount = hcount;}
  G4int GetHitCount() {return hCount;}

  void SetTargetNucleus(G4int tnucleus) {tNucleus = tnucleus;}
  G4int GetTargetNucleus() {return tNucleus;}

  void SetCopyNo(G4int copyno) {CopyNo = copyno;}
  G4int GetCopyNo() {return CopyNo;}

  void SetHitPos(G4ThreeVector &hpos) {hPos = hpos;}
  G4ThreeVector GetHitPos() {return hPos;}

  void SetHitTime(G4double htime) {hTime = htime;}
  G4double GetHitTime() {return hTime;}

  void SetPreKE(G4double preke) {preKE = preke;}
  G4double GetPreKE() {return preKE;}

  void SetPstKE(G4double pstke) {pstKE = pstke;}
  G4double GetPstKE() {return pstKE;}

  void SetPreMom(G4ThreeVector &premom) {preMom = premom;}
  G4ThreeVector GetPreMom() {return preMom;}

  void SetPstMom(G4ThreeVector &pstmom) {pstMom = pstmom;}
  G4ThreeVector GetPstMom() {return pstMom;}

  void SetEdep(G4double edep) {Edep = edep;}
  G4double GetEdep() {return Edep;}

  void SetVrtxPos(G4ThreeVector &vrtxpos) {VrtxPos = vrtxpos;}
  G4ThreeVector GetVrtxPos() {return VrtxPos;}

  void SetVrtxKE(G4double vrtxke) {VrtxKE = vrtxke;}
  G4double GetVrtxKE() {return VrtxKE;}

  void SetSecAtomic(G4int secatomic) {SecAtomic = secatomic;}
  G4int GetSecAtomic() {return SecAtomic;}

  void SetParticleName(G4String pname) {PName = pname;}
  G4String GetParticleName() {return PName;}

  void SetStepLength(G4double length) {Length = length;}
  G4double GetStepLength() {return Length;}

  
 private:
  
  G4int fLayerID = -1;

  G4int ID = -1;
  G4int pID = -1;
  G4int hCount = -1;
  G4int tNucleus = -1;
  G4int CopyNo = -1;
  G4ThreeVector hPos{-9999., -9999., -9999.};
  G4double hTime = 0.;
  G4double preKE = -1.;
  G4double pstKE = -1.;
  G4ThreeVector preMom{-9999., -9999., -9999.};
  G4ThreeVector pstMom{-9999., -9999., -9999.};
  G4double Edep = 0.;
  G4ThreeVector VrtxPos{-9999., -9999., -9999.};
  G4double VrtxKE = -1.;
  G4int SecAtomic = -1;
  G4String PName = "";
  G4double Length = -1.;
};

using NDAHitsCollection = G4THitsCollection<LAMPS_NDA_Hit>;

extern G4ThreadLocal G4Allocator<LAMPS_NDA_Hit>* NDAHitAllocator;

inline void* LAMPS_NDA_Hit::operator new(size_t) {
  if (!NDAHitAllocator) {
    NDAHitAllocator = new G4Allocator<LAMPS_NDA_Hit>;
  }
  return (void*)NDAHitAllocator->MallocSingle();
}

inline void LAMPS_NDA_Hit::operator delete(void* aHit) {
  NDAHitAllocator->FreeSingle((LAMPS_NDA_Hit*)aHit);
}

#endif
