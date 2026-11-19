#include "EventData.hh"
#include <map>

std::map<int, bool> EventData::PrimEvent;
std::map<int, bool> EventData::PrimProton;
int EventData::VolumeStatus1 = -1;
int EventData::VolumeStatus2 = -1;
bool EventData::IsFirstProton1 = false;
bool EventData::IsFirstProton2 = false;

int EventData::NeutronVolumeStatus = -1;
int EventData::NeutronVolumeStatus1 = -1;
int EventData::NeutronVolumeStatus2 = -1;

int EventData::SDn1 = 0;
int EventData::SDn2 = 0;
int EventData::Evtn1 = 0;
int EventData::Evtn2 = 0;

bool EventData::n1Check = false;
bool EventData::n2Check = false;

int EventData::TotalN1Count = 0;
int EventData::TotalN2Count = 0;

void EventData::Clear()
{
    PrimEvent.clear();
    PrimProton.clear();

    VolumeStatus1 = -1;
    VolumeStatus2 = -1;
    IsFirstProton1 = false;
    IsFirstProton2 = false;

    NeutronVolumeStatus1 = -1;
    NeutronVolumeStatus2 = -1;
    NeutronVolumeStatus = -1;

    TotalN1Count = 0;
    TotalN2Count = 0;    

    n1Check = false;
    n2Check = false;
}

void EventData::CountClear()
{
    SDn1 = 0;
    SDn2 = 0;
    Evtn1 = 0;
    Evtn2 = 0;
}

void EventData::RunClear()
{
    TotalN1Count = 0;
    TotalN2Count = 0;
}