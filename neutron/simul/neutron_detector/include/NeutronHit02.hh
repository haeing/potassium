#ifndef NeutronHit02_h
#define NeutronHit02_h 1

#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4VHit.hh"

class G4AttDef;
class G4AttValue;

class NeutronHit02 : public G4VHit {
 public:
  NeutronHit02() = default;
  NeutronHit02(int layerID);
  ~NeutronHit02() override = default;

  inline void* operator new(size_t);
  inline void operator delete(void* bHit);

  void SetEdep(G4double de) { fEdep = de; }
  void AddEdep(G4double de) { fEdep += de; }
  G4double GetEdep() const { return fEdep; }

  void SetInKEn(G4double a) { kEn = a; }
  G4double GetInKEn() const { return kEn; }

  void SetFnKEn(G4double b) { Kdep = b; }
  G4double GetFnKEn() const { return Kdep; }

  void SetNum(G4int c) { Number = c; }
  G4int GetNum() const { return Number; }

  void SetID(G4int d) { ID = d; }
  G4int GetID() const { return ID; }

  void SetParentID(G4int dd) {pID = dd;}
  G4int GetParentID() const { return pID; };

  void SetpNum(G4int e) { proton = e; }
  G4int GetpNum() const { return proton; }

  void SeteNum(G4int f) { electron = f; }
  G4int GeteNum() const { return electron; }

  void SetCNum(G4int g) { carbon = g; }
  G4int GetCNum() { return carbon; }

  void SetPName(G4String gg) {ParticleName = gg;}
  G4String GetPName() { return ParticleName; }

  void SetNeutronKineticEnergy(G4double hhh) { nKE = hhh;}
  G4double GetNeutronKineticEnergy() { return nKE; }

  //

  void SetElectronEdep(G4double h) { electronEdep = h; }
  void AddElectronEdep(G4double h) { electronEdep += h; }
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

  //

  void SetNhIoni(G4int n) { nhioni = n; }
  G4int GetNhIoni() { return nhioni; }

  void SetNeIoini(G4int m) { neioni = m; }
  G4int GetNeIoni() { return neioni; }

  void SetNhadElastic(G4int o) { nhadelastic = o; }
  G4int GetNhadElastic() { return nhadelastic; }

  void SetNnCapture(G4int p) { nncapture = p; }
  G4int GetNnCapture() { return nncapture; }

  void SetNCompt(G4int q) { ncompt = q; }
  G4int GetNCompt() {return ncompt; }

  void SetNConv(G4int r) { nconv = r; }
  G4int GetNConv() {return nconv; }

  void SetNReaction(G4int s) { nreaction = s; }
  G4int GetNReaction() { return nreaction; }

  void SetNeBrem(G4int t) { nebrem = t; }
  G4int GetNeBrem() { return nebrem; }

  void SetNphot(G4int u) { nphot = u; }
  G4int GetNphot() { return nphot; }

  void SetNannihil(G4int v) { nannihil = v; }
  G4int GetNannihil() { return nannihil; }

  void SetNnInelastic(G4int w) { nninela = w; }
  G4int GetNnInelastic() { return nninela;}

  //

  void SetNnInelaCount01(G4int x) { count01 = x; }
  G4int GetNnInelaCount01() { return count01; }

  void SetNnInelaCount02(G4int y) { count02 = y; }
  G4int GetNnInelaCount02() { return count02; }

  void SetNnInelaCount03(G4int z) { count03 = z; }
  G4int GetNnInelaCount03() { return count03; }

  void SetNnInelaCount04(G4int aa) { count04 = aa; }
  G4int GetNnInelaCount04() { return count04; }

  void SetNnInelaCount05(G4int ab) { count05 = ab; }
  G4int GetNnInelaCount05() { return count05; }

//
//electron

void SetehIoni(G4int ac) { ehIoni = ac; }
G4int GetehIoni() {return ehIoni; }

void SeteeIoni(G4int ad) { eeIoni = ad; }
G4int GeteeIoni() { return eeIoni; }

void SetehadElastic(G4int ae) {ehadElastic = ae; }
G4int GetehadElastic() {return ehadElastic; }

void SetenCapture(G4int af) {enCapture = af;}
G4int GetenCapture() {return enCapture;}

void Setecompt(G4int ag) {ecompt = ag;}
G4int Getecompt() {return ecompt;}

void Seteconv(G4int ah) {econv = ah;}
G4int Geteconv() {return econv;}

void SeteeBrem(G4int ai) {eeBrem = ai;}
G4int GeteeBrem() {return eeBrem;}

void Setephot(G4int aj) {ephot = aj;}
G4int Getephot() {return ephot;}

void SeteneutronInelastic(G4int ak) {eneutronInelastic = ak;}
G4int GeteneutronInelastic() {return eneutronInelastic;}

void SeteOther(G4int al) {eOther = al;}
G4int GeteOther() {return eOther;}

void Seteannihil(G4int al0) {eannihil = al0;}
G4int Geteannihil() {return eannihil;}

//
//positron

void SetposihIoni(G4int am) { posihIoni = am; }
G4int GetposihIoni() {return posihIoni; }

void SetposieIoni(G4int an) { posieIoni = an; }
G4int GetposieIoni() { return posieIoni; }

void SetposihadElastic(G4int ao) {posihadElastic = ao; }
G4int GetposihadElastic() {return posihadElastic; }

void SetposinCapture(G4int ap) {posinCapture = ap;}
G4int GetposinCapture() {return posinCapture;}

void Setposicompt(G4int aq) {posicompt = aq;}
G4int Getposicompt() {return posicompt;}

void Setposiconv(G4int ar) {posiconv = ar;}
G4int Getposiconv() {return posiconv;}

void SetposieBrem(G4int as) {posieBrem = as;}
G4int GetposieBrem() {return posieBrem;}

void Setposiphot(G4int at) {posiphot = at;}
G4int Getposiphot() {return posiphot;}

void SetposineutronInelastic(G4int au) {posineutronInelastic = au;}
G4int GetposineutronInelastic() {return posineutronInelastic;}

void SetposiOther(G4int av) {posiOther = av;}
G4int GetposiOther() {return posiOther;}

void Setposiannihil(G4int av0) {posiannihil = av0;}
G4int Getposiannihil() {return posiannihil;}

//
//deutron

void SetdhIoni(G4int aw) { dhIoni = aw; }
G4int GetdhIoni() {return dhIoni; }

void SetdeIoni(G4int ax) { deIoni = ax; }
G4int GetdeIoni() { return deIoni; }

void SetdhadElastic(G4int ay) {dhadElastic = ay; }
G4int GetdhadElastic() {return dhadElastic; }

void SetdnCapture(G4int az) {dnCapture = az;}
G4int GetdnCapture() {return dnCapture;}

void Setdcompt(G4int ba) {dcompt = ba;}
G4int Getdcompt() {return dcompt;}

void Setdconv(G4int bb) {dconv = bb;}
G4int Getdconv() {return dconv;}

void SetdeBrem(G4int bc) {deBrem = bc;}
G4int GetdeBrem() {return deBrem;}

void Setdphot(G4int bd) {dphot = bd;}
G4int Getdphot() {return dphot;}

void SetdneutronInelastic(G4int be) {dneutronInelastic = be;}
G4int GetdneutronInelastic() {return dneutronInelastic;}

void SetdOther(G4int bf) {dOther = bf;}
G4int GetdOther() {return dOther;}

void Setdannihil(G4int bf0) {dannihil = bf0;}
G4int Getdannihil() {return dannihil;}

//
//gamma

void SetghIoni(G4int bg) { ghIoni = bg; }
G4int GetghIoni() {return ghIoni; }

void SetgeIoni(G4int bh) { geIoni = bh; }
G4int GetgeIoni() { return geIoni; }

void SetghadElastic(G4int bi) {ghadElastic = bi; }
G4int GetghadElastic() {return ghadElastic; }

void SetgnCapture(G4int bj) {gnCapture = bj;}
G4int GetgnCapture() {return gnCapture;}

void Setgcompt(G4int bk) {gcompt = bk;}
G4int Getgcompt() {return gcompt;}

void Setgconv(G4int bl) {gconv = bl;}
G4int Getgconv() {return gconv;}

void SetgeBrem(G4int bm) {geBrem = bm;}
G4int GetgeBrem() {return geBrem;}

void Setgphot(G4int bn) {gphot = bn;}
G4int Getgphot() {return gphot;}

void SetgneutronInelastic(G4int bo) {gneutronInelastic = bo;}
G4int GetgneutronInelastic() {return gneutronInelastic;}

void SetgOther(G4int bp) {gOther = bp;}
G4int GetgOther() {return gOther;}

void Setgannihil(G4int bp0) {gannihil = bp0;}
G4int Getgannihil() {return gannihil;}

//
//C12

void SetC12hIoni(G4int bq) { C12hIoni = bq; }
G4int GetC12hIoni() {return C12hIoni; }

void SetC12eIoni(G4int br) { C12eIoni = br; }
G4int GetC12eIoni() { return C12eIoni; }

void SetC12hadElastic(G4int bs) {C12hadElastic = bs; }
G4int GetC12hadElastic() {return C12hadElastic; }

void SetC12nCapture(G4int bt) {C12nCapture = bt;}
G4int GetC12nCapture() {return C12nCapture;}

void SetC12compt(G4int bu) {C12compt = bu;}
G4int GetC12compt() {return C12compt;}

void SetC12conv(G4int bv) {C12conv = bv;}
G4int GetC12conv() {return C12conv;}

void SetC12eBrem(G4int bw) {C12eBrem = bw;}
G4int GetC12eBrem() {return C12eBrem;}

void SetC12phot(G4int bx) {C12phot = bx;}
G4int GetC12phot() {return C12phot;}

void SetC12neutronInelastic(G4int by) {C12neutronInelastic = by;}
G4int GetC12neutronInelastic() {return C12neutronInelastic;}

void SetC12Other(G4int bz) {C12Other = bz;}
G4int GetC12Other() {return C12Other;}  

void SetC12annihil(G4int bz0) {C12annihil = bz0;}
G4int GetC12annihil() {return C12annihil;}

//
//C13

void SetC13hIoni(G4int ca) { C13hIoni = ca; }
G4int GetC13hIoni() {return C13hIoni; }

void SetC13eIoni(G4int cb) { C13eIoni = cb; }
G4int GetC13eIoni() { return C13eIoni; }

void SetC13hadElastic(G4int cc) {C13hadElastic = cc; }
G4int GetC13hadElastic() {return C13hadElastic; }

void SetC13nCapture(G4int cd) {C13nCapture = cd;}
G4int GetC13nCapture() {return C13nCapture;}

void SetC13compt(G4int ce) {C13compt = ce;}
G4int GetC13compt() {return C13compt;}

void SetC13conv(G4int cf) {C13conv = cf;}
G4int GetC13conv() {return C13conv;}

void SetC13eBrem(G4int cg) {C13eBrem = cg;}
G4int GetC13eBrem() {return C13eBrem;}

void SetC13phot(G4int ch) {C13phot = ch;}
G4int GetC13phot() {return C13phot;}

void SetC13neutronInelastic(G4int ci) {C13neutronInelastic = ci;}
G4int GetC13neutronInelastic() {return C13neutronInelastic;}

void SetC13Other(G4int cj) {C13Other = cj;}
G4int GetC13Other() {return C13Other;}  

void SetC13annihil(G4int cj0) {C13annihil = cj0;}
G4int GetC13annihil() {return C13annihil;}

//
//proton

void SetphIoni(G4int ck) { phIoni = ck; }
G4int GetphIoni() {return phIoni; }

void SetpeIoni(G4int cl) { peIoni = cl; }
G4int GetpeIoni() { return peIoni; }

void SetphadElastic(G4int cm) {phadElastic = cm; }
G4int GetphadElastic() {return phadElastic; }

void SetpnCapture(G4int cn) {pnCapture = cn;}
G4int GetpnCapture() {return pnCapture;}

void Setpcompt(G4int co) {pcompt = co;}
G4int Getpcompt() {return pcompt;}

void Setpconv(G4int cp) {pconv = cp;}
G4int Getpconv() {return pconv;}

void SetpeBrem(G4int cq) {peBrem = cq;}
G4int GetpeBrem() {return peBrem;}

void Setpphot(G4int cr) {pphot = cr;}
G4int Getpphot() {return pphot;}

void SetpneutronInelastic(G4int cs) {pneutronInelastic = cs;}
G4int GetpneutronInelastic() {return pneutronInelastic;}

void SetpOther(G4int ct) {pOther = ct;}
G4int GetpOther() {return pOther;}  

void Setpannihil(G4int ct0) {pannihil = ct0;}
G4int Getpannihil() {return pannihil;}

//
//alpha

void SetahIoni(G4int cu) { ahIoni = cu; }
G4int GetahIoni() {return ahIoni; }

void SetaeIoni(G4int cv) { aeIoni = cv; }
G4int GetaeIoni() { return aeIoni; }

void SetahadElastic(G4int cw) {ahadElastic = cw; }
G4int GetahadElastic() {return ahadElastic; }

void SetanCapture(G4int cx) {anCapture = cx;}
G4int GetanCapture() {return anCapture;}

void Setacompt(G4int cy) {acompt = cy;}
G4int Getacompt() {return acompt;}

void Setaconv(G4int cz) {aconv = cz;}
G4int Getaconv() {return aconv;}

void SetaeBrem(G4int da) {aeBrem = da;}
G4int GetaeBrem() {return aeBrem;}

void Setaphot(G4int db) {aphot = db;}
G4int Getaphot() {return aphot;}

void SetaneutronInelastic(G4int dc) {aneutronInelastic = dc;}
G4int GetaneutronInelastic() {return aneutronInelastic;}

void SetaOther(G4int dd) {aOther = dd;}
G4int GetaOther() {return aOther;} 

void Setaannihil(G4int dd0) {aannihil = dd0;}
G4int Getaannihil() {return aannihil;}

//
//Other

void SethIoni(G4int de) { hIoni = de; }
G4int GethIoni() {return hIoni; }

void SeteIoni(G4int df) { eIoni = df; }
G4int GeteIoni() { return eIoni; }

void SethadElastic(G4int dg) {hadElastic = dg; }
G4int GethadElastic() {return hadElastic; }

void SetnCapture(G4int dh) {nCapture = dh;}
G4int GetnCapture() {return nCapture;}

void Setcompt(G4int di) {compt = di;}
G4int Getcompt() {return compt;}

void Setconv(G4int dj) {conv = dj;}
G4int Getconv() {return conv;}

void SeteBrem(G4int dk) {eBrem = dk;}
G4int GeteBrem() {return eBrem;}

void Setphot(G4int dl) {phot = dl;}
G4int Getphot() {return phot;}

void SetneutronInelastic(G4int dm) {neutronInelastic = dm;}
G4int GetneutronInelastic() {return neutronInelastic;}

void SetOther(G4int dn) {Other = dn;}
G4int GetOther() {return Other;} 

void Setannihil(G4int dn0) {annihil = dn0;}
G4int Getannihil() {return annihil;}


 private:
  G4int annihil = 0.;
  G4int aannihil = 0.;
  G4int pannihil = 0.;
  G4int C13annihil = 0.;
  G4int C12annihil = 0.;  
  G4int gannihil = 0.;
  G4int dannihil = 0.;  
  G4int posiannihil = 0.;
  G4int eannihil = 0.;  

  G4int hIoni = 0.;
  G4int eIoni = 0.;
  G4int hadElastic = 0.;
  G4int nCapture = 0.;
  G4int compt = 0.;
  G4int conv = 0.;
  G4int eBrem = 0.;
  G4int phot = 0.;
  G4int neutronInelastic = 0.;
  G4int Other = 0.;

  G4int ahIoni = 0.;
  G4int aeIoni = 0.;
  G4int ahadElastic = 0.;
  G4int anCapture = 0.;
  G4int acompt = 0.;
  G4int aconv = 0.;
  G4int aeBrem = 0.;
  G4int aphot = 0.;
  G4int aneutronInelastic = 0.;
  G4int aOther = 0.;

  G4int phIoni = 0.;
  G4int peIoni = 0.;
  G4int phadElastic = 0.;
  G4int pnCapture = 0.;
  G4int pcompt = 0.;
  G4int pconv = 0.;
  G4int peBrem = 0.;
  G4int pphot = 0.;
  G4int pneutronInelastic = 0.;
  G4int pOther = 0.;

  G4int C13hIoni = 0.;
  G4int C13eIoni = 0.;
  G4int C13hadElastic = 0.;
  G4int C13nCapture = 0.;
  G4int C13compt = 0.;
  G4int C13conv = 0.;
  G4int C13eBrem = 0.;
  G4int C13phot = 0.;
  G4int C13neutronInelastic = 0.;
  G4int C13Other = 0.;

  G4int C12hIoni = 0.;
  G4int C12eIoni = 0.;
  G4int C12hadElastic = 0.;
  G4int C12nCapture = 0.;
  G4int C12compt = 0.;
  G4int C12conv = 0.;
  G4int C12eBrem = 0.;
  G4int C12phot = 0.;
  G4int C12neutronInelastic = 0.;
  G4int C12Other = 0.;

  G4int ghIoni = 0.;
  G4int geIoni = 0.;
  G4int ghadElastic = 0.;
  G4int gnCapture = 0.;
  G4int gcompt = 0.;
  G4int gconv = 0.;
  G4int geBrem = 0.;
  G4int gphot = 0.;
  G4int gneutronInelastic = 0.;
  G4int gOther = 0.;

  G4int dhIoni = 0.;
  G4int deIoni = 0.;
  G4int dhadElastic = 0.;
  G4int dnCapture = 0.;
  G4int dcompt = 0.;
  G4int dconv = 0.;
  G4int deBrem = 0.;
  G4int dphot = 0.;
  G4int dneutronInelastic = 0.;
  G4int dOther = 0.;

  G4int posihIoni = 0.;
  G4int posieIoni = 0.;
  G4int posihadElastic = 0.;
  G4int posinCapture = 0.;
  G4int posicompt = 0.;
  G4int posiconv = 0.;
  G4int posieBrem = 0.;
  G4int posiphot = 0.;
  G4int posineutronInelastic = 0.;
  G4int posiOther = 0.;

  G4int ehIoni = 0.;
  G4int eeIoni = 0.;
  G4int ehadElastic = 0.;
  G4int enCapture = 0.;
  G4int ecompt = 0.;
  G4int econv = 0.;
  G4int eeBrem = 0.;
  G4int ephot = 0.;
  G4int eneutronInelastic = 0.;
  G4int eOther = 0.;

//

  G4double fEdep = 0.;
  G4double kEn = 0.;
  G4double Kdep = -1.;
  G4int fLayerID = -1;
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

  G4int nhioni = 0;
  G4int neioni = 0;
  G4int nhadelastic = 0;
  G4int nncapture = 0;
  G4int nreaction = 0;
  G4int ncompt = 0;
  G4int nconv = 0;
  G4int nebrem = 0;
  G4int nphot = 0;
  G4int nannihil = 0;
  G4int nninela = 0;

  G4int count01 = 0;
  G4int count02 = 0;
  G4int count03 = 0;
  G4int count04 = 0;
  G4int count05 = 0;

  G4int pID = -1;
  G4String ParticleName = "non";
  G4double nKE = 0.;
};

using NeutronHitsCollection02 = G4THitsCollection<NeutronHit02>;

extern G4ThreadLocal G4Allocator<NeutronHit02>* NeutronHitAllocator02;

inline void* NeutronHit02::operator new(size_t) {
  if (!NeutronHitAllocator02) {
    NeutronHitAllocator02 = new G4Allocator<NeutronHit02>;
  }
  return (void*)NeutronHitAllocator02->MallocSingle();
}

inline void NeutronHit02::operator delete(void* bHit) {
  NeutronHitAllocator02->FreeSingle((NeutronHit02*)bHit);
}

#endif
