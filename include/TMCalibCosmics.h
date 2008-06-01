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


#ifndef TAPSMaintain_TMCalibCosmics
#define TAPSMaintain_TMCalibCosmics

#include "TH1.h"
#include "TF1.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TLine.h"

#include "TMSeqCalibModule.h"


enum {
    ECosmics_Calib_Type_BAF2_LG,
    ECosmics_Calib_Type_BAF2_SG,
    ECosmics_Calib_Type_PBWO4_LG,
    ECosmics_Calib_Type_VETO
};


class TMCalibCosmics : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fHName[256];                         // name of the ADC spectra
    Double_t fCStart;                           // Cosmic peak fit start
    Double_t fCEnd;                             // Cosmic peak fit end
    
    TH1F* fHClone;                              // histogram pointer used for cloning
    TF1* fPedFunc;                              // function used for pedestal fitting
    TF1* fPeakFunc;                             // function used for cosmic peak fitting
    TF1* fBgFunc;                               // cosmics peak background function
    TF1* fTotalFunc;                            // total cosmic peak function
    
    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fHNameEntry;                   // name of the ADC spectra
    TGHorizontalFrame* fRangeFrame;             // frame for the range GUI elements
    TGTextEntry* fCStartEntry;                  // start value of fit range
    TGTextEntry* fCEndEntry;                    // end value of fit range
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
    virtual void SaveResults(const Char_t* filename);
    
    void UpdateDetectorType(Int_t id);

    ClassDef(TMCalibCosmics, 0) // Cosmics calibration module
};

#endif

