/*************************************************************************
 * Author: Dominik Werthmueller, 2011-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibTDC                                                           //
//                                                                      //
// Module for the TDC gain calibration.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMCalibTDC
#define TAPSMaintain_TMCalibTDC

#include "TMSeqCalibModule.h"

class TF1;
class TLine;
class TGraphErrors;

enum {
    kTDC_Calib_Type_BAF2,
    kTDC_Calib_Type_VETO
};

class TMCalibTDC : public TMSeqCalibModule
{

private:
    // Configuration parameters
    Char_t fHName[256];                         // name of the ADC spectra
    Char_t fPath[256];                          // path of ROOT files

    Int_t fNfit;                                // number of fits
    TFile** fFiles;                             //[fNfit] array of files
    TF1** fPeakFunc;                            //[fNfit] array of fitting functions
    TF1* fLinFunc;                              // linear function
    TLine** fPeakLine;                          //[fNfit] array of lines indicating the peak position
    TGraphErrors* fGraph;                       // graph for gain calculation

    // Config dialog elements
    TGComboBox* fTypeCombo;                     // LED type selection combo box
    TGTextEntry* fHNameEntry;                   // name of the ADC spectra
    TGTextEntry* fLocation;                     // location of ROOT files
    TGCheckButton* fBatchMode;                  // batch mode check box

public:
    TMCalibTDC() : TMSeqCalibModule(), fNfit(0), fFiles(0), fPeakFunc(0), fLinFunc(0), 
                                       fPeakLine(0), fGraph(0), fTypeCombo(0), fHNameEntry(0), 
                                       fLocation(0), fBatchMode(0){ }
    TMCalibTDC(const Char_t* name, UInt_t id);
    virtual ~TMCalibTDC();
    
    virtual void Init();
    virtual void ReadConfig();
    virtual void Cleanup();
    virtual void Process(Int_t index);
    virtual void Redo();
    virtual void Quit();
    virtual void SaveResults(const Char_t* filename);
    
    void UpdateDetectorType(Int_t id);

    ClassDef(TMCalibTDC, 0) // TDC calibration module
};

#endif

