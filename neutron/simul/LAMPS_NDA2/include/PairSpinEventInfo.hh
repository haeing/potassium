#ifndef PairSpinEventInfo_h
#define PairSpinEventInfo_h 1

#include "G4VUserEventInformation.hh"
#include "globals.hh"
#include "G4ios.hh"

using namespace std;

class PairSpinEventInfo : public G4VUserEventInformation
{
    public : 
        explicit PairSpinEventInfo(G4int spin)
        : fTotalSpin(spin) {}

        ~PairSpinEventInfo() override = default;

        G4int GetTotalSpin() const
        {
            return fTotalSpin;
        }

        void Print() const override
        {
            cout<<"Pair Total Spin : "<<fTotalSpin<<endl;
        }

    private :
        G4int fTotalSpin;
};

#endif