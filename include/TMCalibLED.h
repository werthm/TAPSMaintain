/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2017
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

#include "TMSeqCalibModule.h"

class TGNumberEntry;
class TLine;
class TH1;

enum {
    kLED_Calib_Type_LG_LED1,
    kLED_Calib_Type_LG_LED2,
    kLED_Calib_Type_LGS_LED1,
    kLED_Calib_Type_LGS_LED2,
    kLED_Calib_Type_MANUAL
};

class TMCalibLED : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fUncutHName[256];                    // name of the uncut ADC spectra
    Char_t fCutHName[256];                      // name of the LED cut ADC spectra
    Double_t fXStart;                           // x-Axis start
    Double_t fXEnd;                             // x-Axis end
    Int_t fRebin;                               // rebin
    Double_t fVoltage;                          // threshold voltage

    Double_t fThreshold;                        // threshold
    TH1* fHClone;                               // histogram pointer used for cloning
    TH1* fHDeriv;                               // histogram pointer used for derived histo
    TF1* fFunc;                                 // fitting function
    TLine* fLine;                               // marker line

    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fUncut;                        // name of the uncut ADC spectra
    TGTextEntry* fCut;                          // name of the LED cut ADC spectra
    TGHorizontalFrame* fAxisRangeFrame;         // frame for the x-Axis range GUI elements
    TGTextEntry* fXAxisStart;                   // start value of the x-Axis
    TGTextEntry* fXAxisEnd;                     // end value of the x-Axis
    TGTextEntry* fRebinEntry;                   // rebin input
    TGCheckButton* fBatchMode;                  // batch mode check box
    TGNumberEntry* fVoltageEntry;               // threshold voltage

public:
    TMCalibLED() : TMSeqCalibModule() { }
    TMCalibLED(const Char_t* name, UInt_t id);
    virtual ~TMCalibLED();

    virtual void Init();
    virtual void ReadConfig();
    virtual void Process(Int_t index, Bool_t redo);
    virtual void Quit();
    virtual void SaveResults(const Char_t* filename);

    void UpdateLEDType(Int_t id);

    ClassDef(TMCalibLED, 0) // LED calibration module
};

#endif

