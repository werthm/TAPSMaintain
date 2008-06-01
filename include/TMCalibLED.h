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

#include "TH1.h"
#include "TText.h"
#include "TLine.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"

#include "TMSeqCalibModule.h"


enum {
    ELED_Calib_Type_LG_LED1,
    ELED_Calib_Type_LG_LED2,
    ELED_Calib_Type_PbWO4_LED,
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

