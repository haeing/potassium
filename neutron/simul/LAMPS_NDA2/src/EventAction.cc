#include "EventAction.hh"
#include "LAMPS_NDA_Hit.hh"
#include "RunAction.hh"
#include "EventData.hh"
#include "PairSpinEventInfo.hh"

// #include "Constants.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VHitsCollection.hh"
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include <map>
#include "TMath.h"

using namespace std;

G4VHitsCollection* GetHC(const G4Event* event, G4int collId) {
  auto hce = event->GetHCofThisEvent();
  if (!hce) {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
    return nullptr;
  }

  auto hc = hce->GetHC(collId);
  if (!hc) {
    G4ExceptionDescription msg;
    msg << "Hits collection " << collId << " of this event not found." << G4endl;
    G4Exception("EventAction::EndOfEventAction()",
                "Code001", JustWarning, msg);
  }
  return hc;
}
EventAction::EventAction() 
{
  RootFile = new TFile("./LAMPS_NDA2nd.root","recreate");
  tree = new TTree("tree","tree");

  tree -> Branch("n1KE",     Arr_n1KE,     "n1KE[200]/D");
  tree -> Branch("n1MomX",   Arr_n1MomX,   "n1MomX[200]/D");
  tree -> Branch("n1MomY",   Arr_n1MomY,   "n1MomY[200]/D");
  tree -> Branch("n1MomZ",   Arr_n1MomZ,   "n1MomZ[200]/D");
  tree -> Branch("n1Time",   Arr_n1Time,   "n1Time[200]/D");
  tree -> Branch("n1X",      Arr_n1X,      "n1X[200]/D");
  tree -> Branch("n1Y",      Arr_n1Y,      "n1Y[200]/D");
  tree -> Branch("n1Z",      Arr_n1Z,      "n1Z[200]/D");
  tree -> Branch("n1VrtxX", Arr_n1VrtxX, "n1VrtxX[200]/D");
  tree -> Branch("n1VrtxY", Arr_n1VrtxY, "n1VrtxY[200]/D");
  tree -> Branch("n1VrtxZ", Arr_n1VrtxZ, "n1VrtxZ[200]/D");
  tree -> Branch("n1Edep",   Arr_n1Edep,   "n1Edep[200]/D");
  tree -> Branch("n1Target", Arr_n1Target, "n1Target[200]/D");
  tree -> Branch("n1HitCount", Arr_n1HitCount, "n1HitCount[200]/D");
  tree -> Branch("n1TotalSpin", &TotalSpin, "n1TotalSpin/I");

  tree -> Branch("n2KE",     Arr_n2KE,     "n2KE[200]/D");
  tree -> Branch("n2MomX",   Arr_n2MomX,   "n2MomX[200]/D");
  tree -> Branch("n2MomY",   Arr_n2MomY,   "n2MomY[200]/D");
  tree -> Branch("n2MomZ",   Arr_n2MomZ,   "n2MomZ[200]/D");
  tree -> Branch("n2Time",   Arr_n2Time,   "n2Time[200]/D");
  tree -> Branch("n2X",      Arr_n2X,      "n2X[200]/D");
  tree -> Branch("n2Y",      Arr_n2Y,      "n2Y[200]/D");
  tree -> Branch("n2Z",      Arr_n2Z,      "n2Z[200]/D");
  tree -> Branch("n2VrtxX", Arr_n2VrtxX, "n2VrtxX[200]/D");
  tree -> Branch("n2VrtxY", Arr_n2VrtxY, "n2VrtxY[200]/D");
  tree -> Branch("n2VrtxZ", Arr_n2VrtxZ, "n2VrtxZ[200]/D");  
  tree -> Branch("n2Edep",   Arr_n2Edep,   "n2Edep[200]/D");
  tree -> Branch("n2Target", Arr_n2Target, "n2Target[200]/D"); 
  tree -> Branch("n2HitCount", Arr_n2HitCount, "n2HitCount[200]/D");
  tree -> Branch("n2TotalSpin", &TotalSpin, "n2TotalSpin/I"); 
}

EventAction::~EventAction()
{
  RootFile -> cd();
  tree -> Write();
  RootFile -> Close();
}


void EventAction::BeginOfEventAction(const G4Event*) {

  auto sdManager = G4SDManager::GetSDMpointer();

  for(int i=0; i<20; i++)
  {
    St01Veto[i] = sdManager -> GetCollectionID("St01Veto_"+to_string(i)+"/LAMPS_NDA");
    St01NDv01[i] = sdManager -> GetCollectionID("St01NDv01_"+to_string(i)+"/LAMPS_NDA");
    St01NDh01[i] = sdManager -> GetCollectionID("St01NDh01_"+to_string(i)+"/LAMPS_NDA");
    St01NDv02[i] = sdManager -> GetCollectionID("St01NDv02_"+to_string(i)+"/LAMPS_NDA");
    St01NDh02[i] = sdManager -> GetCollectionID("St01NDh02_"+to_string(i)+"/LAMPS_NDA");

    St02Veto[i] = sdManager -> GetCollectionID("St02Veto_"+to_string(i)+"/LAMPS_NDA");
    St02NDv01[i] = sdManager -> GetCollectionID("St02NDv01_"+to_string(i)+"/LAMPS_NDA");
    St02NDh01[i] = sdManager -> GetCollectionID("St02NDh01_"+to_string(i)+"/LAMPS_NDA");
    St02NDv02[i] = sdManager -> GetCollectionID("St02NDv02_"+to_string(i)+"/LAMPS_NDA");
    St02NDh02[i] = sdManager -> GetCollectionID("St02NDh02_"+to_string(i)+"/LAMPS_NDA");    
  }

  for(int i=0; i<200; i++)
  {
    Arr_n1KE[i] = -9999.;
    Arr_n1MomX[i] = -9999.;
    Arr_n1MomY[i] = -9999.;
    Arr_n1MomZ[i] = -9999.;
    Arr_n1Time[i] = -9999.;
    Arr_n1X[i] = -9999.;
    Arr_n1Y[i] = -9999.;
    Arr_n1Z[i] = -9999.;
    Arr_n1VrtxX[i] = -9999.;
    Arr_n1VrtxY[i] = -9999.;
    Arr_n1VrtxZ[i] = -9999.;
    Arr_n1Edep[i] = -9999.;
    Arr_n1Target[i] = -9999.;
    Arr_n1HitCount[i] = -9999.;

    Arr_n2KE[i] = -9999.;
    Arr_n2MomX[i] = -9999.;
    Arr_n2MomY[i] = -9999.;
    Arr_n2MomZ[i] = -9999.;
    Arr_n2Time[i] = -9999.;
    Arr_n2X[i] = -9999.;
    Arr_n2Y[i] = -9999.;
    Arr_n2Z[i] = -9999.;
    Arr_n2VrtxX[i] = -9999.;
    Arr_n2VrtxY[i] = -9999.;
    Arr_n2VrtxZ[i] = -9999.;    
    Arr_n2Edep[i] = -9999.;
    Arr_n2Target[i] = -9999.;
    Arr_n2HitCount[i] = -9999.;
  }

  TotalSpin = -1;

  HitRec.clear();

  EventN01.clear();
  EventN02.clear();

  EventSec01.clear();
  EventSec02.clear();

  EventN01Edep.clear();
  EventN02Edep.clear();

  EventRecN01.clear();
  EventRecN02.clear();

  EventData::Clear();
}

void EventAction::EndOfEventAction(const G4Event* event) 
{

  if (event->GetEventID() % 1000 == 0)
  // if(event->GetEventID() % 1 == 0)
  { 
    cout << " Event ID : " << event->GetEventID() << endl;  
  }

  auto analysisManager = G4AnalysisManager::Instance();

  G4VHitsCollection *St01VetoHC[20];
  G4VHitsCollection *St01NDv01HC[20];
  G4VHitsCollection *St01NDh01HC[20];
  G4VHitsCollection *St01NDv02HC[20];
  G4VHitsCollection *St01NDh02HC[20];

  G4VHitsCollection *St02VetoHC[20];
  G4VHitsCollection *St02NDv01HC[20];
  G4VHitsCollection *St02NDh01HC[20];
  G4VHitsCollection *St02NDv02HC[20];
  G4VHitsCollection *St02NDh02HC[20];

  G4int St01VetoHCsize[20];
  G4int St01NDv01HCsize[20];
  G4int St01NDh01HCsize[20];
  G4int St01NDv02HCsize[20];
  G4int St01NDh02HCsize[20];

  G4int St02VetoHCsize[20];
  G4int St02NDv01HCsize[20];
  G4int St02NDh01HCsize[20];
  G4int St02NDv02HCsize[20];
  G4int St02NDh02HCsize[20];  

  for(int i=0; i<20; i++)
  {
    St01VetoHC[i] = GetHC(event, St01Veto[i]);
    if(!St01VetoHC[i])
    {
      cout<<"No Hit Collection for St01VetoHC in End of EventAction"<<endl;
      return;
    }
    St01VetoHCsize[i] = St01VetoHC[i] -> GetSize();

    St01NDv01HC[i] = GetHC(event, St01NDv01[i]);
    if(!St01NDv01HC[i])
    {
      cout<<"No Hit Collection for St01NDv01HC in End of EventAction"<<endl;
      return;
    }
    St01NDv01HCsize[i] = St01NDv01HC[i] -> GetSize();

    St01NDh01HC[i] = GetHC(event, St01NDh01[i]);
    if(!St01NDh01HC[i])
    {
      cout<<"No Hit Collection for St01NDh01HC in End of EventAction"<<endl;
      return;
    }
    St01NDh01HCsize[i] = St01NDh01HC[i] -> GetSize();

    St01NDv02HC[i] = GetHC(event, St01NDv02[i]);
    if(!St01NDv02HC[i])
    {
      cout<<"No Hit Collection for St01NDv02HC in End of EventAction"<<endl;
      return;
    }
    St01NDv02HCsize[i] = St01NDv02HC[i] -> GetSize();

    St01NDh02HC[i] = GetHC(event, St01NDh02[i]);
    if(!St01NDh02HC[i])
    {
      cout<<"No Hit Collection for St01NDh02HC in End of EventAction"<<endl;
      return;
    }
    St01NDh02HCsize[i] = St01NDh02HC[i] -> GetSize();

    //

    St02VetoHC[i] = GetHC(event, St02Veto[i]);
    if(!St02VetoHC[i])
    {
      cout<<"No Hit Collection for St02VetoHC in End of EventAction"<<endl;
      return;
    }
    St02VetoHCsize[i] = St02VetoHC[i] -> GetSize();
    
    St02NDv01HC[i] = GetHC(event, St02NDv01[i]);
    if(!St02NDv01HC[i])
    {
      cout<<"No Hit Collection for St02NDv01HC in End of EventAction"<<endl;
      return;
    }
    St02NDv01HCsize[i] = St02NDv01HC[i] -> GetSize();

    St02NDh01HC[i] = GetHC(event, St02NDh01[i]);
    if(!St02NDh01HC[i])
    {
      cout<<"No Hit Collection for St02NDh01HC in End of EventAction"<<endl;
      return;
    }
    St02NDh01HCsize[i] = St02NDh01HC[i] -> GetSize();

    St02NDv02HC[i] = GetHC(event, St02NDv02[i]);
    if(!St02NDv02HC[i])
    {
      cout<<"No Hit Collection for St02NDv02HC in End of EventAction"<<endl;
      return;
    }
    St02NDv02HCsize[i] = St02NDv02HC[i] -> GetSize();

    St02NDh02HC[i] = GetHC(event, St02NDh02[i]);    
    if(!St02NDh02HC[i])
    {
      cout<<"No Hit Collection for St02NDh02HC in End of EventAction"<<endl;
      return;
    }
    St02NDh02HCsize[i] = St02NDh02HC[i] -> GetSize();
  }     


  //
  // Setup for Hit data

  G4int id, pid, hcount, target, secatomic, copyno;
  G4double time, preke, pstke, edep, vrtxke, length;
  G4ThreeVector hitpos, premom, pstmom, vrtxpos;
  G4String pname;

  //=====================================================
  // hole01
  //=====================================================

  for(int i=0; i<20; i++)
  { 

    //Stage01 Veto
    for(int j=0; j<St01VetoHCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";      

      auto hit = static_cast<LAMPS_NDA_Hit*>(St01VetoHC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }



    //Stage01 NDv01
    for(int j=0; j<St01NDv01HCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";        

      auto hit = static_cast<LAMPS_NDA_Hit*>(St01NDv01HC[i] -> GetHit(j));
    
      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }



    //Stage01 NDh01
    for(int j=0; j<St01NDh01HCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";

      auto hit = static_cast<LAMPS_NDA_Hit*>(St01NDh01HC[i] -> GetHit(j));   
    
      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }


    //Stage01 NDv02
    for(int j=0; j<St01NDv02HCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St01NDv02HC[i] -> GetHit(j));
      
      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }



    //Stage01 NDh02
    for(int j=0; j<St01NDh02HCsize[i]; j++)
    {    
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St01NDh02HC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }


    //
    //Stage02 Veto
    for(int j=0; j<St02VetoHCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St02VetoHC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }



    //Stage02 NDv01
    for(int j=0; j<St02NDv01HCsize[i]; j++)
    {   
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St02NDv01HC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }



    //Stage02 NDh01
    for(int j=0; j<St02NDh01HCsize[i]; j++)
    {       
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St02NDh01HC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }


    //Stage02 NDv02
    for(int j=0; j<St02NDv02HCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St02NDv02HC[i] -> GetHit(j));

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }


    //Stage02 NDh02
    for(int j=0; j<St02NDh02HCsize[i]; j++)
    {
      id = -1;
      pid = -1;
      hcount = -1;
      target = -1;
      secatomic = -1;
      copyno = -1;

      time = 0.;
      preke = -1.;
      pstke = -1.;
      edep = 0.;
      vrtxke = -1.;
      length = -1.;

      hitpos = G4ThreeVector(-9999., -9999., -9999.);
      premom = G4ThreeVector(-9999., -9999., -9999.);
      pstmom = G4ThreeVector(-9999., -9999., -9999.);
      vrtxpos = G4ThreeVector(-9999., -9999., -9999.);

      pname = "";  

      auto hit = static_cast<LAMPS_NDA_Hit*>(St02NDh02HC[i] -> GetHit(j)); 

      id = hit -> GetID();
      pid = hit -> GetParentID();
      hcount = hit -> GetHitCount();
      target = hit -> GetTargetNucleus();
      secatomic = hit -> GetSecAtomic();
      copyno = hit -> GetCopyNo() -1;

      time = hit -> GetHitTime();
      preke = hit -> GetPreKE();
      pstke = hit -> GetPstKE();
      edep = hit -> GetEdep();
      vrtxke = hit -> GetVrtxKE();
      length = hit -> GetStepLength();

      hitpos = hit -> GetHitPos();
      premom = hit -> GetPreMom();
      pstmom = hit -> GetPstMom();
      vrtxpos = hit -> GetVrtxPos();

      pname = hit -> GetParticleName();      

      if(time > 0)
      {
        HitRec.push_back({id, pid, hcount, target, secatomic, copyno, time, preke, pstke, edep, vrtxke, length, hitpos, premom, pstmom, vrtxpos, pname}); 
      }
    }     
  }

  //=====================================================
  // hole02
  //=====================================================  

  //
  //Data separation

  //For primary neutron
  for(int i=0; i<HitRec.size(); i++)
  {
    if(HitRec[i].ID == 1)
    {
      if(HitRec[i].Time < 2000)
      {
        EventN01.push_back({HitRec[i].ID, HitRec[i].pID, HitRec[i].Hitcount, HitRec[i].Target, HitRec[i].SecAtomic, HitRec[i].CopyNo,
                            HitRec[i].Time, HitRec[i].preKE, HitRec[i].pstKE, HitRec[i].Edep, HitRec[i].vrtxKE, HitRec[i].Length, 
                            HitRec[i].HitPos, HitRec[i].preMom, HitRec[i].pstMom, HitRec[i].vrtxPos});
      }
    }
    else if(HitRec[i].ID == 2)
    {
      if(HitRec[i].Time < 2000)
      {
        EventN02.push_back({HitRec[i].ID, HitRec[i].pID, HitRec[i].Hitcount, HitRec[i].Target, HitRec[i].SecAtomic, HitRec[i].CopyNo,
                            HitRec[i].Time, HitRec[i].preKE, HitRec[i].pstKE, HitRec[i].Edep, HitRec[i].vrtxKE, HitRec[i].Length, 
                            HitRec[i].HitPos, HitRec[i].preMom, HitRec[i].pstMom, HitRec[i].vrtxPos});
      }
    }    
  }

  //

  sort(EventN01.begin(), EventN01.end(),
      [](const sHitRec &a, const sHitRec &b)
      {
        return a.Hitcount < b.Hitcount;
      });


  sort(EventN02.begin(), EventN02.end(),
      [](const sHitRec &a, sHitRec &b)
      {
        return a.Hitcount < b.Hitcount;
      });

  
  //For Edep from secondary01
  if(EventN01.size() > 0)
  {
    EventSec01.resize(EventN01.size());
  }

  if(EventN01.size() > 0)
  for(int i=0; i<EventN01.size(); i++)
  {
    for(int j=0; j<HitRec.size(); j++)
    {
      if((EventN01[i].HitPos == HitRec[j].vrtxPos)&&(HitRec[j].pID == 1)&&(EventN01[i].CopyNo == HitRec[j].CopyNo)&&(HitRec[j].Time < 2000))
      {
        EventSec01[i].push_back({HitRec[j].ID, HitRec[j].pID, HitRec[j].Hitcount, HitRec[j].Target, HitRec[j].SecAtomic, HitRec[j].CopyNo,
                                 HitRec[j].Time, HitRec[j].preKE, HitRec[j].pstKE, HitRec[j].Edep, HitRec[j].vrtxKE, HitRec[j].Length, 
                                 HitRec[j].HitPos, HitRec[j].preMom, HitRec[j].pstMom, HitRec[j].vrtxPos, HitRec[j].ParticleName});
      }

      if((HitRec[j].pID != 0)&&(HitRec[j].pID != 1)&&(HitRec[j].pID != 2))
      {
        for(const auto &data : EventSec01[i])
        {
          if((data.ID == HitRec[j].pID)&&(EventN01[i].CopyNo == HitRec[j].CopyNo)&&(HitRec[j].Time < 2000))
          {
            EventSec01[i].push_back({HitRec[j].ID, HitRec[j].pID, HitRec[j].Hitcount, HitRec[j].Target, HitRec[j].SecAtomic, HitRec[j].CopyNo,
                                     HitRec[j].Time, HitRec[j].preKE, HitRec[j].pstKE, HitRec[j].Edep, HitRec[j].vrtxKE, HitRec[j].Length, 
                                     HitRec[j].HitPos, HitRec[j].preMom, HitRec[j].pstMom, HitRec[j].vrtxPos, HitRec[j].ParticleName});
            break;
          }
        }
      }
    }
  }

  //

  if(EventN01.size() > 0)
  {
    EventN01Edep.resize(EventN01.size());
  }

  if(EventN01.size() > 0)
  for(int i=0; i<EventN01.size(); i++)
  {
    if(EventSec01[i].size() == 0)
    {
      continue;
    }

    for(int j=0; j<EventSec01[i].size(); j++)
    {
      EventN01Edep[i] += EventSec01[i][j].Edep;
    }
  }


  //


  if(EventN01.size() > 0)
  {
    G4int BefCopyNo = -1;
    G4int CurCopyNo = -1;
    G4int FirstIndx = -1;

    for(int i=0; i<EventN01.size(); i++)
    {
      CurCopyNo = EventN01[i].CopyNo;

      if(BefCopyNo != CurCopyNo)
      {
        auto it = find_if(EventRecN01.begin(), EventRecN01.end(), 
                          [CurCopyNo](const sEventRec &data)
                          {
                            return data.CopyNo == CurCopyNo;
                          });

        if(it == EventRecN01.end())
        {
          EventRecN01.push_back({EventN01[i].ID, EventN01[i].pID, EventN01[i].Hitcount, EventN01[i].Target, EventN01[i].SecAtomic, EventN01[i].CopyNo,
                                 EventN01[i].Time, EventN01[i].preKE, EventN01[i].pstKE, EventN01Edep[i], EventN01[i].vrtxKE, EventN01[i].Length,
                                 EventN01[i].HitPos, EventN01[i].preMom, EventN01[i].pstMom, EventN01[i].vrtxPos});
          FirstIndx = EventRecN01.size() -1;
        }
        else
        {
          FirstIndx = -1;
        }
      }
      else if(BefCopyNo == CurCopyNo)
      {
        if(FirstIndx >= 0)
        {
          EventRecN01[FirstIndx].Edep += EventN01Edep[i];
        }
      }

      BefCopyNo = CurCopyNo;
    }
  }  


  //For Edep from secondary02
  if(EventN02.size() > 0)
  {
    EventSec02.resize(EventN02.size());
  }

  if(EventN02.size() > 0)
  for(int i=0; i<EventN02.size(); i++)
  {
    for(int j=0; j<HitRec.size(); j++)
    {
      if((EventN02[i].HitPos == HitRec[j].vrtxPos)&&(HitRec[j].pID == 2)&&(EventN02[i].CopyNo == HitRec[j].CopyNo)&&(HitRec[j].Time < 2000))
      {
        EventSec02[i].push_back({HitRec[j].ID, HitRec[j].pID, HitRec[j].Hitcount, HitRec[j].Target, HitRec[j].SecAtomic, HitRec[j].CopyNo,
                                 HitRec[j].Time, HitRec[j].preKE, HitRec[j].pstKE, HitRec[j].Edep, HitRec[j].vrtxKE, HitRec[j].Length, 
                                 HitRec[j].HitPos, HitRec[j].preMom, HitRec[j].pstMom, HitRec[j].vrtxPos, HitRec[j].ParticleName});
      }

      if((HitRec[j].pID != 0)&&(HitRec[j].pID != 1)&&(HitRec[j].pID != 2))
      {
        for(const auto &data : EventSec02[i])
        {
          if((data.ID == HitRec[j].pID)&&(EventN02[i].CopyNo == HitRec[j].CopyNo)&&(HitRec[j].Time < 2000))
          {
            EventSec02[i].push_back({HitRec[j].ID, HitRec[j].pID, HitRec[j].Hitcount, HitRec[j].Target, HitRec[j].SecAtomic, HitRec[j].CopyNo,
                                     HitRec[j].Time, HitRec[j].preKE, HitRec[j].pstKE, HitRec[j].Edep, HitRec[j].vrtxKE, HitRec[j].Length, 
                                     HitRec[j].HitPos, HitRec[j].preMom, HitRec[j].pstMom, HitRec[j].vrtxPos, HitRec[j].ParticleName});
            break;
          }
        }
      }
    }
  }  

  //

  if(EventN02.size() > 0)
  {
    EventN02Edep.resize(EventN02.size());
  }

  if(EventN02.size() > 0)
  for(int i=0; i<EventN02.size(); i++)
  {
    if(EventSec02[i].size() == 0)
    {
      continue;
    }

    for(int j=0; j<EventSec02[i].size(); j++)
    {
      EventN02Edep[i] += EventSec02[i][j].Edep;
    }
  }  

  //

  if(EventN02.size() > 0)
  {
    G4int BefCopyNo = -1;
    G4int CurCopyNo = -1;
    G4int FirstIndx = -1;

    for(int i=0; i<EventN02.size(); i++)
    {
      CurCopyNo = EventN02[i].CopyNo;

      if(BefCopyNo != CurCopyNo)
      {
        auto it = find_if(EventRecN02.begin(), EventRecN02.end(), 
                          [CurCopyNo](const sEventRec &data)
                          {
                            return data.CopyNo == CurCopyNo;
                          });

        if(it == EventRecN02.end())
        {
          EventRecN02.push_back({EventN02[i].ID, EventN02[i].pID, EventN02[i].Hitcount, EventN02[i].Target, EventN02[i].SecAtomic, EventN02[i].CopyNo,
                                 EventN02[i].Time, EventN02[i].preKE, EventN02[i].pstKE, EventN02Edep[i], EventN02[i].vrtxKE, EventN02[i].Length,
                                 EventN02[i].HitPos, EventN02[i].preMom, EventN02[i].pstMom, EventN02[i].vrtxPos});
          FirstIndx = EventRecN02.size() -1;
        }
        else
        {
          FirstIndx = -1;
        }
      }
      else if(BefCopyNo == CurCopyNo)
      {
        if(FirstIndx >= 0)
        {
          EventRecN02[FirstIndx].Edep += EventN02Edep[i];
        }
      }

      BefCopyNo = CurCopyNo;
    }
  }
 

  //=====================================================
  // hole03
  //===================================================== 


  //
  // Data storing

  //For Total Spin

  const auto *info = dynamic_cast<const PairSpinEventInfo*>(event -> GetUserInformation());

  if(info)
  {
    TotalSpin = info -> GetTotalSpin();
  }

  //For primary neutron01

  if(EventRecN01.size() > 0)
  for(int i=0; i<EventRecN01.size(); i++)
  {
    Arr_n1KE[EventRecN01[i].CopyNo] = EventRecN01[i].preKE;
    Arr_n1MomX[EventRecN01[i].CopyNo] = EventRecN01[i].preMom.x();
    Arr_n1MomY[EventRecN01[i].CopyNo] = EventRecN01[i].preMom.y();
    Arr_n1MomZ[EventRecN01[i].CopyNo] = EventRecN01[i].preMom.z();
    Arr_n1Time[EventRecN01[i].CopyNo] = EventRecN01[i].Time;
    Arr_n1X[EventRecN01[i].CopyNo] = EventRecN01[i].HitPos.x();
    Arr_n1Y[EventRecN01[i].CopyNo] = EventRecN01[i].HitPos.y();
    Arr_n1Z[EventRecN01[i].CopyNo] = EventRecN01[i].HitPos.z();
    Arr_n1VrtxX[EventRecN01[i].CopyNo] = EventRecN01[i].vrtxPos.x();
    Arr_n1VrtxY[EventRecN01[i].CopyNo] = EventRecN01[i].vrtxPos.y();
    Arr_n1VrtxZ[EventRecN01[i].CopyNo] = EventRecN01[i].vrtxPos.z();
    Arr_n1Edep[EventRecN01[i].CopyNo] = EventRecN01[i].Edep;
    Arr_n1Target[EventRecN01[i].CopyNo] = EventRecN01[i].Target;
    Arr_n1HitCount[EventRecN01[i].CopyNo] = EventRecN01[i].Hitcount;
  }


  if(EventRecN02.size() > 0)
  for(int i=0; i<EventRecN02.size(); i++)
  {
    Arr_n2KE[EventRecN02[i].CopyNo] = EventRecN02[i].preKE;
    Arr_n2MomX[EventRecN02[i].CopyNo] = EventRecN02[i].preMom.x();
    Arr_n2MomY[EventRecN02[i].CopyNo] = EventRecN02[i].preMom.y();
    Arr_n2MomZ[EventRecN02[i].CopyNo] = EventRecN02[i].preMom.z();
    Arr_n2Time[EventRecN02[i].CopyNo] = EventRecN02[i].Time;
    Arr_n2X[EventRecN02[i].CopyNo] = EventRecN02[i].HitPos.x();
    Arr_n2Y[EventRecN02[i].CopyNo] = EventRecN02[i].HitPos.y();
    Arr_n2Z[EventRecN02[i].CopyNo] = EventRecN02[i].HitPos.z();
    Arr_n2VrtxX[EventRecN02[i].CopyNo] = EventRecN02[i].vrtxPos.x();
    Arr_n2VrtxY[EventRecN02[i].CopyNo] = EventRecN02[i].vrtxPos.y();
    Arr_n2VrtxZ[EventRecN02[i].CopyNo] = EventRecN02[i].vrtxPos.z();    
    Arr_n2Edep[EventRecN02[i].CopyNo] = EventRecN02[i].Edep;
    Arr_n2Target[EventRecN02[i].CopyNo] = EventRecN02[i].Target;
    Arr_n2HitCount[EventRecN02[i].CopyNo] = EventRecN02[i].Hitcount;
  }  

  tree -> Fill();
}


//   //
//   // This is for Neutron Detector 01
//   auto hc01 = GetHC(event, NeutronHCID01);
//   if (!hc01)
//     return;

//   auto test = hc01->GetSize();

//   G4double initKen = 0.;
//   G4double finKen = 0.;
//   G4double Kdep = 0.;
//   G4double TotKdep = 0.;
//   G4int number = 0;

//   vector<int> ID(1);
//   vector<int>::iterator it;
//   G4int id = -1;
//   G4int pid = -1;
//   G4String Pname = "non";
//   vector<double> globalEdep(1);
//   vector<G4int> parentID(1);
//   vector<G4String> ParticleName(1);
//   vector<double> protonEdep(1);
//   vector<double> CarbonEdep(1);
//   G4double NeutronKineticEnergy = 0. *MeV;
//   G4double nKE = 0.;

//   for (int i = 0; i < test; i++) {
//     G4double edep = 0;
//     // G4double proton_edep = 0.;
//     // G4double carbon_edep = 0.;

//     auto hit = static_cast<NeutronHit01*>(hc01->GetHit(i));

//     initKen = hit->GetInKEn();
//     finKen = hit->GetFnKEn();
//     number = hit->GetNum();
//     edep = hit->GetEdep();
//     nKE = hit -> GetNeutronKineticEnergy();
//     NeutronKineticEnergy += nKE;
    
//     id = hit->GetID();
//     pid = hit->GetParentID();
//     Pname = hit->GetPName();

//     // proton_edep = hit -> GetProtonEdep();
//     // carbon_edep = hit -> GetCarbon12Edep();

//   it = find(ID.begin(), ID.end(), id);

//   if(it == ID.end())
//   {
//     ID.push_back(id);
//     globalEdep.push_back(edep);
//     parentID.push_back(pid);
//     ParticleName.push_back(Pname);

//     // protonEdep.push_back(proton_edep);
//     // CarbonEdep.push_back(carbon_edep);
//   }
//   else
//   {
//     int n = it - ID.begin();
//     globalEdep[n] += edep;

//     // protonEdep[n] += proton_edep;
//     // CarbonEdep[n] += carbon_edep;

//   }
//   }
//   G4double TotalEdep = 0 *MeV;
//   G4double NeutronEdep = 0 *MeV;
//   G4double TotalProtonEdep = 0.*MeV;
//   G4double TotalCarbonEdep = 0.*MeV;

//   for(int i=0; i<globalEdep.size()-1; i++)
//   {
//     if(globalEdep[i+1] > 133 *eV)
//     {
//       TotalEdep += globalEdep[i+1];
//     }
//   }
  

// //
// // Extraction Energy Depostion from Neutron
// // cout<<"size of ID : "<<ID.size()<<endl;
// // cout<<"size of Edep : "<<globalEdep.size()<<endl;
// // cout<<"size of parent ID : "<<parentID.size()<<endl;
// // cout<<"size of particle name : "<<ParticleName.size()<<endl;
// // for(int i=0; i<ID.size(); i++)
// // {
// //   cout<<"ID["<<i<<"] : "<<ID[i]<<endl;
// //   cout<<"parent ID["<<i<<"] : "<<parentID[i]<<endl;
// //   cout<<"particle name["<<i<<"] : "<<ParticleName[i]<<endl;
// //   cout<<"energy deposition["<<i<<"] : "<<globalEdep[i]<<endl;
// // }
// vector<G4String>::iterator neutron_it;
// vector<G4String>::iterator proton_it;
// vector<G4String>::iterator c12_it;
// vector<G4String>::iterator c13_it;

// vector<G4String>::iterator iterator = ParticleName.begin();
// vector<vector<G4String>::iterator> proton_it_vector(1);
// vector<vector<G4String>::iterator> c12_it_vector(1);
// vector<vector<G4String>::iterator> c13_it_vector(1);

// for(int k = 0; k<ID.size()-1; k++)
// {

// neutron_it = find(iterator, ParticleName.end(),"neutron");
// if(neutron_it == ParticleName.end())
// {
//   break;
// }

// //
// //For Neutron Total Edep
// if(neutron_it != ParticleName.end())
// {
  
// G4int n = neutron_it - ParticleName.begin();
// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
// for(int i=0; i<ID.size()-1; i++)
// {
  
//   if((ID[n] == parentID[i]) && (globalEdep[i] > 133 *eV))
//   {

//     NeutronEdep += globalEdep[i];
    
//   }
// }
 
// }

// //
// //For Proton Edep

// if((proton_it != ParticleName.end()) && (neutron_it != ParticleName.end()))
// {
  
  
// G4int n = neutron_it - ParticleName.begin();

// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
// for(int i=0; i<ID.size() -1; i++)
// {
//   proton_it = find(iterator, ParticleName.end(), "proton");
  
//   // cout<<"1st : "<<*proton_it <<endl;
//   // cout<<"2nd : "<<*ParticleName.end() <<endl;
//   if(proton_it == ParticleName.end())
//   {
//     break;
//   }
  
//   if(proton_it != ParticleName.end())
//   {
//     G4int p = proton_it - ParticleName.begin();
    
//   }
//   G4int p = proton_it - ParticleName.begin();

//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   if((ID[n] == parentID[p]) && (globalEdep[p] > 133 *eV))
//   {
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalProtonEdep += globalEdep[p];
//   }
//   iterator = proton_it +1;
// }
 
// }

// iterator = ParticleName.begin();

// //
// //For Carbon Edep
// if(((c12_it != ParticleName.end())||(c13_it != ParticleName.end()))&&(neutron_it != ParticleName.end()))
// {
  
// G4int n = neutron_it - ParticleName.begin();

// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;

// //
// //C12
// for(int i=0; i<ID.size() -1; i++)
// {
//   c12_it = find(iterator, ParticleName.end(), "C12");
  
//   if(c12_it == ParticleName.end())
//   {
//     break;
//   }
//   G4int c12;
//   if(c12_it != ParticleName.end())
//   {
//     // cout<<"c12 index : "<<c12_it - ParticleName.begin()<<endl;
//     // cout<<"Is this really c12? : "<<ParticleName[c12_it - ParticleName.begin()]<<endl;
    
//     c12 = c12_it - ParticleName.begin();
    

//   }
  
//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   if((ID[n] == parentID[c12]) && (globalEdep[c12] > 133 *eV))
//   {
  
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalCarbonEdep += globalEdep[c12];
//   }

//   iterator = c12_it +1;
// }

// iterator = ParticleName.begin();

// //
// //C13
// for(int i=0; i<ID.size() -1; i++)
// {
//   c13_it = find(iterator, ParticleName.end(), "C13");
  
//   if(c13_it == ParticleName.end())
//   {
//     break;
//   }
  
//   G4int c13;
//   if(c13_it != ParticleName.end())
//   {
    
//     c13 = c13_it - ParticleName.begin();
    
//   }

//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   // cout<<"global edep : "<<globalEdep[c13]<<endl;
//   if((ID[n] == parentID[c13]) && (globalEdep[c13] > 133 *eV))
//   {
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalCarbonEdep += globalEdep[c13];
//   }

//   iterator = c13_it +1;
// }
 
// }

// iterator = neutron_it +1;
// }

//   //
//   // This is for Neutron Detector 02
//   auto hc02 = GetHC(event, NeutronHCID02);
//   if (!hc02)
//     return;

//   auto test02 = hc02->GetSize();

//   G4double initKen02 = 0.;
//   G4double finKen02 = 0.;
//   G4double Kdep02 = 0.;
//   G4double TotKdep02 = 0.;
//   G4int number02 = 0;

//   vector<int> ID02(1);
//   vector<int>::iterator it02;
//   G4int id02 = -1;
//   G4int pid02 = -1;
//   G4String Pname02 = "non";
//   vector<double> globalEdep02(1);
//   vector<G4int> parentID02(1);
//   vector<G4String> ParticleName02(1);
//   vector<double> protonEdep02(1);
//   vector<double> CarbonEdep02(1);
//   G4double NeutronKineticEnergy02 = 0. *MeV;
//   G4double nKE02 = 0.;

//   for (int i = 0; i < test02; i++) {
//     G4double edep02 = 0;
//     // G4double proton_edep = 0.;
//     // G4double carbon_edep = 0.;

//     auto hit02 = static_cast<NeutronHit02*>(hc02->GetHit(i));

//     initKen02 = hit02->GetInKEn();
//     finKen02 = hit02->GetFnKEn();
//     number02 = hit02->GetNum();
//     edep02 = hit02->GetEdep();
//     nKE02 = hit02 -> GetNeutronKineticEnergy();
//     NeutronKineticEnergy02 += nKE02;
    
//     id02 = hit02->GetID();
//     pid02 = hit02->GetParentID();
//     Pname02 = hit02->GetPName();

//     // proton_edep = hit -> GetProtonEdep();
//     // carbon_edep = hit -> GetCarbon12Edep();

//   it02 = find(ID02.begin(), ID02.end(), id02);

//   if(it02 == ID02.end())
//   {
//     ID02.push_back(id02);
//     globalEdep02.push_back(edep02);
//     parentID02.push_back(pid02);
//     ParticleName02.push_back(Pname02);

//     // protonEdep.push_back(proton_edep);
//     // CarbonEdep.push_back(carbon_edep);
//   }
//   else
//   {
//     int n02 = it02 - ID02.begin();
//     globalEdep02[n02] += edep02;

//     // protonEdep[n] += proton_edep;
//     // CarbonEdep[n] += carbon_edep;

//   }
//   }
//   G4double TotalEdep02 = 0 *MeV;
//   G4double NeutronEdep02 = 0 *MeV;
//   G4double TotalProtonEdep02 = 0. *MeV;
//   G4double TotalCarbonEdep02 = 0. *MeV;

//   for(int i=0; i<globalEdep02.size()-1; i++)
//   {
//     if(globalEdep02[i+1] > 133 *eV)
//     {
//       TotalEdep02 += globalEdep02[i+1];
//     }
//   }
  

// //
// // Extraction Energy Depostion from Neutron
// // cout<<"size of ID : "<<ID.size()<<endl;
// // cout<<"size of Edep : "<<globalEdep.size()<<endl;
// // cout<<"size of parent ID : "<<parentID.size()<<endl;
// // cout<<"size of particle name : "<<ParticleName.size()<<endl;
// // for(int i=0; i<ID.size(); i++)
// // {
// //   cout<<"ID["<<i<<"] : "<<ID[i]<<endl;
// //   cout<<"parent ID["<<i<<"] : "<<parentID[i]<<endl;
// //   cout<<"particle name["<<i<<"] : "<<ParticleName[i]<<endl;
// //   cout<<"energy deposition["<<i<<"] : "<<globalEdep[i]<<endl;
// // }
// vector<G4String>::iterator neutron_it02;
// vector<G4String>::iterator proton_it02;
// vector<G4String>::iterator c12_it02;
// vector<G4String>::iterator c13_it02;

// vector<G4String>::iterator iterator02 = ParticleName02.begin();
// vector<vector<G4String>::iterator> proton_it_vector02(1);
// vector<vector<G4String>::iterator> c12_it_vector02(1);
// vector<vector<G4String>::iterator> c13_it_vector02(1);

// for(int k = 0; k<ID02.size() -1; k++)
// {

// neutron_it02 = find(iterator02, ParticleName02.end(),"neutron");
// if(neutron_it02 == ParticleName02.end())
// {
//   break;
// }

// //
// //For Neutron Total Edep
// if(neutron_it02 != ParticleName02.end())
// {
  
// G4int n02 = neutron_it02 - ParticleName02.begin();
// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
// for(int i=0; i<ID02.size() -1; i++)
// {
  
//   if((ID02[n02] == parentID02[i]) && (globalEdep02[i] > 133 *eV))
//   {

//     NeutronEdep02 += globalEdep02[i];
    
//   }
// }
 
// }

// //
// //For Proton Edep

// if((proton_it02 != ParticleName02.end()) && (neutron_it02 != ParticleName02.end()))
// {
  
  
// G4int n02 = neutron_it02 - ParticleName02.begin();

// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;
// for(int i=0; i<ID02.size() -1; i++)
// {
//   proton_it02 = find(iterator02, ParticleName02.end(), "proton");
  
//   // cout<<"1st : "<<*proton_it <<endl;
//   // cout<<"2nd : "<<*ParticleName.end() <<endl;
//   if(proton_it02 == ParticleName02.end())
//   {
//     break;
//   }
  
//   if(proton_it02 != ParticleName02.end())
//   {
//     G4int p02 = proton_it02 - ParticleName02.begin();
    
//   }
//   G4int p02 = proton_it02 - ParticleName02.begin();

//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   if((ID02[n02] == parentID[p02]) && (globalEdep02[p02] > 133 *eV))
//   {
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalProtonEdep02 += globalEdep02[p02];
//   }
//   iterator02 = proton_it02 +1;
// }
 
// }

// iterator02 = ParticleName02.begin();

// //
// //For Carbon Edep
// if(((c12_it02 != ParticleName02.end())||(c13_it02 != ParticleName02.end()))&&(neutron_it02 != ParticleName02.end()))
// {
  
// G4int n02 = neutron_it02 - ParticleName02.begin();

// // for(int i=0; i<ParticleName.size(); i++)
// // {
// //   cout<<"particle name : "<<ParticleName[i]<<endl;
// // }

// // cout<<"where is neutron : "<<n<<endl;

// // cout<<"Is this really neutron? : "<<ParticleName[n]<<endl;

// //
// //C12
// for(int i=0; i<ID02.size() -1; i++)
// {
//   c12_it02 = find(iterator02, ParticleName02.end(), "C12");
  
//   if(c12_it02 == ParticleName02.end())
//   {
//     break;
//   }
//   G4int c1202;
//   if(c12_it02 != ParticleName02.end())
//   {
//     // cout<<"c12 index : "<<c12_it - ParticleName.begin()<<endl;
//     // cout<<"Is this really c12? : "<<ParticleName[c12_it - ParticleName.begin()]<<endl;
    
//     c1202 = c12_it02 - ParticleName02.begin();
    

//   }
  
//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   if((ID02[n02] == parentID02[c1202]) && (globalEdep02[c1202] > 133 *eV))
//   {
  
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalCarbonEdep02 += globalEdep02[c1202];
//   }

//   iterator02 = c12_it02 +1;
// }

// iterator02 = ParticleName02.begin();

// //
// //C13
// for(int i=0; i<ID02.size() -1; i++)
// {
//   c13_it02 = find(iterator02, ParticleName02.end(), "C13");
  
//   if(c13_it02 == ParticleName02.end())
//   {
//     break;
//   }
  
//   G4int c1302;
//   if(c13_it02 != ParticleName02.end())
//   {
    
//     c1302 = c13_it02 - ParticleName02.begin();
    
//   }

//   // cout<<"id check : "<<ID[n]<<endl;
//   // cout<<"parent check : "<<parentID[i]<<endl;
//   // cout<<"number of loop : "<<i<<endl;
//   // cout<<"global edep : "<<globalEdep[c13]<<endl;
//   if((ID02[n02] == parentID02[c1302]) && (globalEdep02[c1302] > 133 *eV))
//   {
//     // cout<<"daughter particle is "<<ParticleName[i]<<endl;
//     // cout<<"depostied energy of daughter particle : "<<globalEdep[i]<<endl;

//     TotalCarbonEdep02 += globalEdep02[c1302];
//   }

//   iterator02 = c13_it02 +1;
// }
 
// }

// iterator02 = neutron_it02 +1;
// }

// //
// //Neutron Detector 01
// if(TotalEdep/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(0, TotalEdep/MeV);
// }

//   // analysisManager->FillNtupleDColumn(2, globalElectron);
//   // analysisManager->FillNtupleDColumn(3, globalPositron);
// if(TotalProtonEdep/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(1, TotalProtonEdep/MeV);
// }

// if(TotalCarbonEdep/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(2, TotalCarbonEdep/MeV);
// }

// if(NeutronEdep/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(3, NeutronEdep/MeV);
// }
//   // analysisManager->FillNtupleDColumn(6, globalNucleus);

// if(NeutronKineticEnergy/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(4, NeutronKineticEnergy/MeV);
// }
  


// //
// //Neutron Detector 02

// if(TotalEdep02/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(6, TotalEdep02/MeV);
// }

// if(TotalProtonEdep02/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(7, TotalProtonEdep02/MeV);
// }

// if(TotalCarbonEdep02/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(8, TotalCarbonEdep02/MeV);
// }
//   // analysisManager->FillNtupleDColumn(2, globalElectron);
//   // analysisManager->FillNtupleDColumn(3, globalPositron);
//   // analysisManager->FillNtupleDColumn(6, globalNucleus);

// if(NeutronEdep02/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(9, NeutronEdep02/MeV);
// }

// if(NeutronKineticEnergy02/MeV > 133 *eV)
// {
//   analysisManager->FillNtupleDColumn(10, NeutronKineticEnergy02/MeV);
// }

