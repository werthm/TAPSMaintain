// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibCosmics                                                       //
//                                                                      //
// Module for the cosmics energy calibration.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSCalib_TMCalibCosmics
#define TAPSCalib_TMCalibCosmics

#include "TH1.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"

#include "TMSeqCalibModule.h"


enum {
    ECosmics_Calib_Type_BAF2_LG,
    ECosmics_Calib_Type_BAF2_SG,
    ECosmics_Calib_Type_PBWO4_LG,
    ECosmics_Calib_Type_VETO,
    ECosmics_Calib_Type_MANUAL
};


class TMCalibCosmics : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fHName[256];                         // name of the ADC spectra
    Double_t fXStart;                           // x-Axis start
    Double_t fXEnd;                             // x-Axis end
    
    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fHNameEntry;                   // name of the ADC spectra
    TGHorizontalFrame* fAxisRangeFrame;         // frame for the x-Axis range GUI elements
    TGTextEntry* fXAxisStart;                   // start value of the x-Axis
    TGTextEntry* fXAxisEnd;                     // end value of the x-Axis
    TGCheckButton* fBatchMode;                  // batch mode check box
    
public:
    TMCalibCosmics() : TMSeqCalibModule() { }
    TMCalibCosmics(const Char_t* name, UInt_t id);
    virtual ~TMCalibCosmics();
    
    virtual void Init();
    virtual void ReadConfig();
    virtual void Process(Int_t index);
    virtual void Redo();
    virtual void Quit();
    
    void UpdateDetectorType(Int_t id);

    ClassDef(TMCalibCosmics, 0) // Cosmics calibration module
};

#endif

