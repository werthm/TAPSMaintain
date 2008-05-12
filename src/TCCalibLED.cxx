// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibLED                                                           //
//                                                                      //
// Module for the LED calibration.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMCalibLED.h"

ClassImp(TMCalibLED)


//______________________________________________________________________________
TMCalibLED::TMCalibLED(const Char_t* name, UInt_t id)
    : TMCalibModule(name, id, 1)
{
    // Constructor.
    
    // create an embedded canvas
    fEmbCanvas = new TRootEmbeddedCanvas("TMCalibLED_Emb_Canvas", fFrame, 800, 600);
    Int_t wID = fEmbCanvas->GetCanvasWindowId();
    fCanvas = new TCanvas("TMCalibLED_Canvas", 800, 600, wID);
    fEmbCanvas->AdoptCanvas(fCanvas);
    
    fCanvas->SetBorderMode(0);
    fEmbCanvas->MapSubwindows();
    fFrame->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));

}

//______________________________________________________________________________
TMCalibLED::~TMCalibLED()
{
    // Destructor.

}

//______________________________________________________________________________
void TMCalibLED::Start()
{
    // Start method.

}

