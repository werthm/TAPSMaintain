/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2017
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

#include "TMSeqCalibModule.h"

class TF1;
class TLine;

enum {
    kCosmics_Calib_Type_BAF2_LG,
    kCosmics_Calib_Type_BAF2_LGS,
    kCosmics_Calib_Type_BAF2_SG,
    kCosmics_Calib_Type_BAF2_SGS,
    kCosmics_Calib_Type_VETO,
    kCosmics_Calib_Type_PWO,
    kCosmics_Calib_Type_PWO_S,
    kCosmics_Calib_Type_PWO_VETO,
    kCosmics_Calib_Type_PWO_VETO_S
};

class TMCalibCosmics : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fHName[256];                         // name of the ADC spectra
    Double_t fCStart;                           // Cosmic peak fit start
    Double_t fCEnd;                             // Cosmic peak fit end
    Double_t fPeakRange;                        // +/- range of peak fitting
    Double_t fPeakLimMin;                       // peak position minimum limit
    Double_t fPeakLimMax;                       // peak position maximum limit
    Double_t fPeakWidthFac;                     // peak width maximum factor

    TH1F* fHClone;                              // histogram pointer used for cloning
    TF1* fPedFunc;                              // function used for pedestal fitting
    TF1* fBgFunc;                               // cosmics peak background function
    TF1* fTotalFunc;                            // total cosmic peak function
    TLine* fPedLine;                            // line indicating the pedestal position
    TLine* fPeakLine;                           // line indicating the peak position

    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fHNameEntry;                   // name of the ADC spectra
    TGHorizontalFrame* fRangeFrame;             // frame for the range GUI elements
    TGTextEntry* fCStartEntry;                  // start value of fit range
    TGTextEntry* fCEndEntry;                    // end value of fit range
    TGCheckButton* fBatchMode;                  // batch mode check box

    Double_t CalculateGain(Double_t ped, Double_t peak);

public:
    TMCalibCosmics() : TMSeqCalibModule() { }
    TMCalibCosmics(const Char_t* name, UInt_t id);
    virtual ~TMCalibCosmics();

    virtual void Init();
    virtual void ReadConfig();
    virtual void Cleanup();
    virtual void Process(Int_t index, Bool_t redo);
    virtual void Quit();
    virtual void SaveResults(const Char_t* filename);

    void UpdateDetectorType(Int_t id);

    ClassDef(TMCalibCosmics, 0) // Cosmics calibration module
};

#endif

