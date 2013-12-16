/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMSeqCalibModule                                                     //
//                                                                      //
// Abstract module for sequential element calibration.                  //
//                                                                      //
// To-do-list for creating a new module based on TMSeqCalibModule:      //
//                                                                      //
// PURE VIRTUAL METHODS                                                 //
// The following methods are pure virtual and have to be implemented    //
// by any subclass of TMSeqCalibModule:                                 //
//                                                                      //
// void Process(Int_t index): Element fitter method. Will be called for //
//                            each element index. This method should    //
//                            contain the main code of the module.      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMSeqCalibModule
#define TAPSMaintain_TMSeqCalibModule

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
    TAPSDetector_t fDetID;                          // TAPS detector ID
    
    TRootEmbeddedCanvas* fEmbCanvas;                // the embedded canvas
    TCanvas* fCanvas;                               // the canvas
    TGHorizontalFrame* fControlFrame;               // Control frame
    
    TGTextButton* fPrev;                            // "Previous" button
    TGTextButton* fNext;                            // "Next" button
    TGTextButton* fRedo;                            // "Redo" button
    TGTextButton* fQuit;                            // "Quit" button
    
    TGLabel* fInfo;                                 // Info label
    
    Int_t GetCurrentDetectorSize() const;

public:
    TMSeqCalibModule() : TMModule() { }
    TMSeqCalibModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile, Bool_t needsConfig);
    virtual ~TMSeqCalibModule();

    void ShowCanvasInfo(Int_t event, Int_t px, Int_t py, TObject *selected);
    
    void ProcessPreviousElement();
    void ProcessNextElement();
    void ProcessElement(Int_t index)
    {
        fCanvas->cd();
        fCurrentElement = index;
        Process(index);
        fCanvas->Update();
    }
    
    virtual void Redo() { Process(fCurrentElement); }
    virtual void Process(Int_t index) = 0;          // Process the element 'index'
    virtual void Quit() = 0;                        // Save results and quit

    ClassDef(TMSeqCalibModule, 0) // Sequential element calibration base module
};

#endif

