// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMDBModule                                                           //
//                                                                      //
// Module for the manipulation of the TAPS MySQL database.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMDBModule.h"

ClassImp(TMDBModule)


//______________________________________________________________________________
TMDBModule::TMDBModule(const Char_t* name, UInt_t id)
    : TMModule(name, id, 0, kFALSE, kFALSE)
{
    // Constructor.
    
    // ------------------------------ Control frame ------------------------------
    fControlFrame = new TGHorizontalFrame(fFrame, 50, 50);
    
    // combo title
    fControlFrame->AddFrame(new TGLabel(fControlFrame, "DB table:"), new TGLayoutHints(kLHintsNoHints, 5, 5, 5, 5));
    
    // combo box
    fTableCombo = new TGComboBox(fControlFrame);
    //fTableCombo->Connect("Selected(Int_t)", "TMCalibLED", this, "UpdateLEDType(Int_t)");
    fTableCombo->Resize(200, 22);
    fTableCombo->AddEntry("BaF2 High Voltage", EDBModule_BaF2_HV);
    fTableCombo->AddEntry("BaF2 CFD Threshold", EDBModule_BaF2_CFD);
    fTableCombo->AddEntry("BaF2 LED1 Threshold", EDBModule_BaF2_LED1);
    fTableCombo->AddEntry("BaF2 LED2 Threshold", EDBModule_BaF2_LED2);
    fControlFrame->AddFrame(fTableCombo, new TGLayoutHints(kLHintsNoHints, 5, 5, 2, 2));


    // add control frame to main frame
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
    
    
    // ------------------------------ Table frame ------------------------------
    fTableFrame = new TGHorizontalFrame(fFrame, 50, 50);
    
    // table scroll canvas
    fTableCanvas = new TGCanvas(fTableFrame, 250, 500, kSunkenFrame | kDoubleBorder, TGCanvas::GetWhitePixel());
    fTableInputFrame = new TGVerticalFrame(fTableCanvas, kHorizontalFrame);
    fTableFrame->AddFrame(fTableCanvas, new TGLayoutHints(kLHintsLeft | kLHintsExpandX , 0, 0, 0, 0));
    
    // add table frame to control frame
    fFrame->AddFrame(fTableFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX | kLHintsExpandY, 5, 5, 5, 5));
    
    
}

//______________________________________________________________________________
TMDBModule::~TMDBModule()
{
    // Destructor.
    
}

