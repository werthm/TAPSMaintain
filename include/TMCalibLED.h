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


#ifndef TAPSMaintain_TMCalibLED
#define TAPSMaintain_TMCalibLED

#include <cstdlib>
#include "TH1.h"
#include "TText.h"
#include "TLine.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"

#include "TMSeqCalibModule.h"
#include "TMUtils.h"


enum {
    ELED_Calib_Type_LG_LED1,
    ELED_Calib_Type_LG_LED2,
    ELED_Calib_Type_MANUAL
};


class TMCalibLED : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fUncutHName[256];                    // name of the uncut ADC spectra
    Char_t fCutHName[256];                      // name of the LED cut ADC spectra
    Double_t fXStart;                           // x-Axis start
    Double_t fXEnd;                             // x-Axis end
    Double_t fLevel;                            // threshold level
    Double_t fRingVoltages[11];                 // threshold voltages set per ring
    
    TH1F* fHClone;                              // histogram pointer used for cloning

    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fUncut;                        // name of the uncut ADC spectra
    TGTextEntry* fCut;                          // name of the LED cut ADC spectra
    TGHorizontalFrame* fAxisRangeFrame;         // frame for the x-Axis range GUI elements
    TGTextEntry* fXAxisStart;                   // start value of the x-Axis
    TGTextEntry* fXAxisEnd;                     // end value of the x-Axis
    TGTextEntry* fThresholdLevel;               // threshold level input
    TGCheckButton* fBatchMode;                  // batch mode check box
    TGNumberEntry* fRing1Voltage;               // threshold voltage set for the ring 1
    TGNumberEntry* fRing2Voltage;               // threshold voltage set for the ring 2
    TGNumberEntry* fRing3Voltage;               // threshold voltage set for the ring 3
    TGNumberEntry* fRing4Voltage;               // threshold voltage set for the ring 4
    TGNumberEntry* fRing5Voltage;               // threshold voltage set for the ring 5
    TGNumberEntry* fRing6Voltage;               // threshold voltage set for the ring 6
    TGNumberEntry* fRing7Voltage;               // threshold voltage set for the ring 7
    TGNumberEntry* fRing8Voltage;               // threshold voltage set for the ring 8
    TGNumberEntry* fRing9Voltage;               // threshold voltage set for the ring 9
    TGNumberEntry* fRing10Voltage;              // threshold voltage set for the ring 10
    TGNumberEntry* fRing11Voltage;              // threshold voltage set for the ring 11
    
    Double_t FindThreshold(TH1F* h);

public:
    TMCalibLED() : TMSeqCalibModule() { }
    TMCalibLED(const Char_t* name, UInt_t id);
    virtual ~TMCalibLED();
    
    virtual void Init();
    virtual void ReadConfig();
    virtual void Process(Int_t index);
    virtual void Redo();
    virtual void Quit();
    virtual void SaveResults(const Char_t* filename);
    
    void UpdateLEDType(Int_t id);

    ClassDef(TMCalibLED, 0) // LED calibration module
};

#endif

