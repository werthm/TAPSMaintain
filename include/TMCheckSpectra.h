// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2009
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
    kSpec_BaF2_LG_LED1,
    kSpec_BaF2_LG_LED2,
    kSpec_BaF2_LGbed,
    kSpec_BaF2_LGnoP,
    kSpec_BaF2_LGS,
    kSpec_BaF2_SG,
    kSpec_BaF2_SGS,  
    kSpec_BaF2_TIME, 
    kSpec_BaF2_T,
    kSpec_BaF2_TDIFF,
    kSpec_Veto_Q,
    kSpec_Veto_QLED,
    kSpec_Veto_T,
    kSpec_T_DIFF_BAFVETO_single,
    kSpec_PWO_LG,
    kSpec_PWO_LG_TIME,
    kSpec_PWO_LGS,
    kSpec_PWO_TIME,
    kSpec_PWO_TIME_MULT,
    kSpec_PWO_VETO_LG,
    kSpec_PWO_VETO_LGS,
    kSpec_PWO_VETO_TIME
};


static const Char_t specNames[24][64] = {
     {"nothing"},
     {"LG/baf2-LG-%03d"},
     {"LG_LED1/baf2-LG_LED1-%03d"},
     {"LG_LED2/baf2-LG_LED2-%03d"},
     {"LGbed/baf2-LGbed-%03d"},
     {"LGnoP/baf2-LGnoP-%03d"},
     {"LGS/baf2-LGS-%03d"},
     {"SG/baf2-SG-%03d"},
     {"SGS/baf2-SGS-%03d"},
     {"TIME/baf2-TIME-%03d"},
     {"T/baf2-T-%03d"},
     {"TDIFF/baf2-TDIFF-%03d"},
     {"Veto-Q/veto-Q-%03d"},
     {"Veto-QLED/veto-QV-%03d"},
     {"Veto-T/veto-T-%03d"},
     {"T_DIFF_BAFVETO_single/t_DIFF_BAFVETO_single-%03d"},
     {"PWO-LG/pwo-LG-%03d"},
     {"PWO-LG-TIME/pwo-LG-TIME-%03d"},
     {"PWO-LGS/pwo-LGS-%03d"},
     {"PWO-TIME/pwo-TIME-%03d"},
     {"PWO-TIME/pwo-TIME-MULT-%03d"},
     {"PWO-VETO-LG/pwo-veto-LG-%03d"},
     {"PWO-VETO-LGS/pwo-veto-LGS-%03d"},
     {"PWO-VETO-TIME/pwo-veto-TIME-%03d"}
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

