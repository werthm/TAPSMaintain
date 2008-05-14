// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMSeqCalibModule                                                     //
//                                                                      //
// Abstract module for sequential element calibration.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSCalib_TMSeqCalibModule
#define TAPSCalib_TMSeqCalibModule

#include "RQ_OBJECT.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGButton.h"
#include "TStyle.h"

#include "TMModule.h"


class TMSeqCalibModule : public TMModule
{
    RQ_OBJECT("TMSeqCalibModule")

protected:
    Int_t fCurrentElement;                          // index of the current element
    
    TRootEmbeddedCanvas* fEmbCanvas;                // the embedded canvas
    TCanvas* fCanvas;                               // the canvas
    TGHorizontalFrame* fControlFrame;               // Control frame
    
    TGTextButton* fPrev;                            // "Previous" button
    TGTextButton* fNext;                            // "Next" button
    TGTextButton* fQuit;                            // "Quit" button

public:
    TMSeqCalibModule() : TMModule() { }
    TMSeqCalibModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile);
    virtual ~TMSeqCalibModule();

    void ProcessPreviousElement();
    void ProcessNextElement();
    void ProcessElement(Int_t index)
    {
        fCurrentElement = index;
        Process(index);
        fCanvas->Update();
    }
    
    virtual void Process(Int_t index) = 0;          // Process the element 'index'
    virtual void Quit() = 0;                        // Save results and quit

    ClassDef(TMSeqCalibModule, 0) // Sequential element calibration base module
};

#endif

