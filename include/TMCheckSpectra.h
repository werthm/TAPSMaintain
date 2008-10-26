// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
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
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGNumberEntry.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"

#include "TMModule.h"


enum {
    ESpec_Empty,
    ESpec_BaF2_LG,
    ESpec_BaF2_LG_LED1,
    ESpec_BaF2_LG_LED2,
    ESpec_BaF2_LGbed,
    ESpec_BaF2_LGnoP,
    ESpec_BaF2_LGS,
    ESpec_BaF2_SG,
    ESpec_BaF2_SGS,  
    ESpec_BaF2_TIME, 
    ESpec_BaF2_T,
    ESpec_BaF2_TDIFF,
    ESpec_Veto_Q,
    ESpec_Veto_QLED,
    ESpec_Veto_T,
    ESpec_T_DIFF_BAFVETO_single,
    ESpec_PWO_LG,
    ESpec_PWO_LG_TIME,
    ESpec_PWO_LGS,
    ESpec_PWO_TIME,
    ESpec_PWO_TIME_MULT
};


static const Char_t specNames[21][64] = 
    {{"nothing"},
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
     {"PWO-TIME/pwo-TIME-MULT-%03d"}};


class TMCheckSpectra : public TMModule
{
    RQ_OBJECT("TMCheckSpectra")

private:
    TGCompositeFrame* fControlFrame;                        // frame for the control elements
    TGComboBox* fSpectraCombo;                              // spectra selection combo box
    TGNumberEntry* fElementNumberEntry;                     // entry for the detector number
    
    TGVerticalFrame* fCanvasFrame;                          // Canvas frame
    TRootEmbeddedCanvas* fEmbCanvas;                        // the embedded canvas
    TCanvas* fCanvas;                                       // the canvas
 
public:
    TMCheckSpectra() : TMModule() { }
    TMCheckSpectra(const Char_t* name, UInt_t id);
    virtual ~TMCheckSpectra();
    
    void DrawHistogram();
    void SpectraSelectionChanged(Int_t id);

    virtual void Init();
    virtual void ReadConfig() { }
    virtual void Cleanup();

    ClassDef(TMCheckSpectra, 0) // Module for checking the TAPS spectra
};

#endif

