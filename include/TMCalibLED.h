// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibLED                                                           //
//                                                                      //
// Module for the LED calibration.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSCalib_TMCalibLED
#define TAPSCalib_TMCalibLED

#include "TH1.h"
#include "TText.h"
#include "TLine.h"

#include "TMSeqCalibModule.h"


class TMCalibLED : public TMSeqCalibModule
{

private:
    Double_t fLevel;                            // threshold level
    TH1F* hClone;                               // histogram pointer used for cloning
    
    Double_t FindThreshold(TH1F* h);

public:
    TMCalibLED() : TMSeqCalibModule() { }
    TMCalibLED(const Char_t* name, UInt_t id);
    virtual ~TMCalibLED();
    
    virtual void Init();
    virtual void Process(Int_t index);
    virtual void Quit();

    ClassDef(TMCalibLED, 0) // LED calibration module
};

#endif

