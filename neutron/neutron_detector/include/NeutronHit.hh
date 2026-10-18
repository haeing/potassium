#ifndef NeutronHit_h
#define NeutronHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

class NeutronHit : public G4VHit
{
  public:
    NeutronHit() = default;
    NeutronHit(int layerID);
    ~NeutronHit() override = default;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
   
    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetInKEn(G4double a) {kEn = a;}
    G4double GetInKEn() const { return kEn;}

    void SetFnKEn(G4double b) {Kdep = b;}
    G4double GetFnKEn() const {return Kdep;}

    void SetNum(G4int c) {Number = c;}
    G4int GetNum() const {return Number;}

    void SetID(G4int d) {ID = d;}
    G4int GetID() const {return ID;}

    void SetpNum(G4int e) {proton = e;}
    G4int GetpNum() const {return proton;}

    void SeteNum(G4int f) {electron = f;}
    G4int GeteNum() const {return electron;}

    void SetCNum(G4int g) {carbon = g;}
    G4int GetCNum() {return carbon;}

    void SetElectronEdep(G4double h) {electronEdep = h;}
    void AddElectronEdep(G4double h) {electronEdep += h;}
    G4double GetElectronEdep() { return electronEdep; }

    void SetPositronEdep(G4double i) { positronEdep = i; }
    void AddPositronEdep(G4double i) { positronEdep += i; }
    G4double GetPositronEdep() { return positronEdep; }

    void SetProtonEdep(G4double j) { protonEdep = j; }
    void AddProtonEdep(G4double j) { protonEdep += j; }
    G4double GetProtonEdep() { return protonEdep; }

    void SetCarbon12Edep(G4double k) { carbon12Edep = k; }
    void AddCarbon12Edep(G4double k) { carbon12Edep += k; }
    G4double GetCarbon12Edep() { return carbon12Edep; }

    void SetNucleusEdep(G4double l) { nucleusEdep = l; }
    void AddNucleusEdep(G4double l) { nucleusEdep = l; }
    G4double GetNucleusEdep() { return nucleusEdep; }


  private:

    G4double fEdep = 0.;
    G4double kEn = 0.;
    G4double Kdep =-1.;
    int fLayerID = -1;
    G4int Number = 0;
    G4int ID = -1;
    G4int proton = 0;
    G4int electron = 0;
    G4int carbon = 0;

    G4double electronEdep = 0;
    G4double positronEdep = 0;
    G4double protonEdep = 0;
    G4double carbon12Edep = 0;
    G4double nucleusEdep = 0;


};

using NeutronHitsCollection = G4THitsCollection<NeutronHit>;

extern G4ThreadLocal G4Allocator<NeutronHit>* NeutronHitAllocator;

inline void* NeutronHit::operator new(size_t)
{
  if (!NeutronHitAllocator) {
       NeutronHitAllocator = new G4Allocator<NeutronHit>;
  }
  return (void*)NeutronHitAllocator->MallocSingle();
}

inline void NeutronHit::operator delete(void* aHit)
{
  NeutronHitAllocator->FreeSingle((NeutronHit*) aHit);
}

#endif
