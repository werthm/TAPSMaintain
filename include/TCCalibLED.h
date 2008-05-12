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

#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"

#include "TMModule.h"


class TMCalibLED : public TMModule
{

private:
    TRootEmbeddedCanvas* fEmbCanvas;            // the embedded canvas
    TManvas* fCanvas;                           // the canvas

public:
    TMCalibLED() : TMModule() { }
    TMCalibLED(const Char_t* name, UInt_t id);
    virtual ~TMCalibLED();

    virtual void Start();

    ClassDef(TMCalibLED, 0) // LED calibration module
};

#endif

