#ifndef EventData_h
#define EventData_h 1

#include <map>

class EventData
{
    public :
        static std::map<int, bool> PrimEvent;
        static std::map<int, bool> PrimProton;
        static int VolumeStatus1;
        static int VolumeStatus2;
        static bool IsFirstProton1;
        static bool IsFirstProton2;

        static int NeutronVolumeStatus;
        static int NeutronVolumeStatus1;
        static int NeutronVolumeStatus2;

        static int SDn1;
        static int SDn2;
        static int Evtn1;
        static int Evtn2;

        static bool n1Check;
        static bool n2Check;

        static int TotalN1Count;
        static int TotalN2Count;

        static void Clear();
        static void CountClear();
        static void RunClear();
};

#endif