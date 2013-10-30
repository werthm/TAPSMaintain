// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCheckSpectra                                                       //
//                                                                      //
// Module providing efficient checking of the various TAPS spectra.     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMCheckSpectra 
#define TAPSMaintain_TMCheckSpectra

#include "RQ_OBJECT.h"
#include "TH1.h"
#include "TKey.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGNumberEntry.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TStyle.h"

#include "TMModule.h"
#include "TMUtils.h"


enum {
    kSpec_Empty,
    kSpec_BaF2_LG,
    kSpec_BaF2_LG_CFD,
    kSpec_BaF2_LG_LED1,
    kSpec_BaF2_LG_LED2,
    kSpec_BaF2_LGS,
    kSpec_BaF2_LGS_CFD,
    kSpec_BaF2_LGS_LED1,
    kSpec_BaF2_LGS_LED2,
    kSpec_BaF2_SG,
    kSpec_BaF2_SGS,  
    kSpec_BaF2_Time,  
    kSpec_Veto,
    kSpec_Veto_Time,
    kSpec_PWO,
    kSpec_PWO_S,
    kSpec_PWO_Time,
    kSpec_PWO_Veto,
    kSpec_PWO_Veto_S,
    kSpec_PWO_Veto_Time
};


static const Char_t specNames[20][64] = {
     {"nothing"},
     {"BaF2_LG_%03d"},
     {"BaF2_LG_CFD_%03d"},
     {"BaF2_LG_LED1_%03d"},
     {"BaF2_LG_LED2_%03d"},
     {"BaF2_LGS_%03d"},
     {"BaF2_LGS_CFD_%03d"},
     {"BaF2_LGS_LED1_%03d"},
     {"BaF2_LGS_LED2_%03d"},
     {"BaF2_SG_%03d"},
     {"BaF2_SGS_%03d"},  
     {"BaF2_Time_%03d"},  
     {"Veto_%03d"},
     {"Veto_Time_%03d"},
     {"PWO_%03d"},
     {"PWO_S_%03d"},
     {"PWO_Time_%03d"},
     {"PWO_Veto_%03d"},
     {"PWO_Veto_S_%03d"},
     {"PWO_Veto_Time_%03d"}
};


class TMCheckSpectra : public TMModule
{
    RQ_OBJECT("TMCheckSpectra")

private:
    TGCompositeFrame* fControlFrame;                        // frame for the control elements
    TGComboBox* fSpectraCombo;                              // spectra selection combo box
    TGComboBox* fElementCombo;                              // element selection combo box
    TGNumberEntry* fElementNumberEntry;                     // entry for the detector number
    TGListBox* fSpList;                                     // spectra list
     
    TGVerticalFrame* fCanvasFrame;                          // Canvas frame
    TRootEmbeddedCanvas* fEmbCanvas;                        // the embedded canvas
    TCanvas* fCanvas;                                       // the canvas

    TGCheckButton* fCheckLogy;                              // checkbox for log y-axis selection
    TGNumberEntry* fXStart;                                 // x-axis range start
    TGNumberEntry* fXEnd;                                   // x-axis range end

    Bool_t fPatternSpectrumLoaded;                          // used to check if a pattern spectrum is loaded

    void DrawSingleHistogram();
    void DrawMultipleHistograms();
    
public:
    TMCheckSpectra() : TMModule() { }
    TMCheckSpectra(const Char_t* name, UInt_t id);
    virtual ~TMCheckSpectra();
    
    void DrawPatternHistogram(Int_t id);
    void SpectraClassChanged(Int_t id);
    void ElementRangeChanged(Int_t id);
    void ElementNumberChanged();
    void UpdateHistogram()
    {
        if (fPatternSpectrumLoaded) DrawPatternHistogram(fSpList->GetSelected());
        else DrawHistogram();
    }
    void DrawHistogram()
    {
        if (fElementCombo->GetSelected() == kRange_Single_Element) DrawSingleHistogram();
        else DrawMultipleHistograms();
    }
 
    virtual void Init();
    virtual void ReadConfig() { }
    virtual void Cleanup();

    ClassDef(TMCheckSpectra, 0) // Module for checking the TAPS spectra
};

#endif

