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


#include "TMSeqCalibModule.h"

ClassImp(TMSeqCalibModule)


//______________________________________________________________________________
TMSeqCalibModule::TMSeqCalibModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile, 
                                   Bool_t needsConfig)
    : TMModule(name, id, inNresults, needsROOTInputFile, needsConfig)
{
    // Constructor.
    
    Char_t cname[256];
    
    fCurrentElement = -1;
    fDetID = kNo_Detector;
    
    // style
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetTitleFillColor(0);
    gStyle->SetTitleBorderSize(1);
    gStyle->SetStatColor(0);
    gStyle->SetStatBorderSize(1);
    gStyle->SetLegendBorderSize(1);
    //gStyle->SetPadTopMargin(0.05);
    //gStyle->SetPadBottomMargin(0.05);
    //gStyle->SetPadLeftMargin(0.05);
    //gStyle->SetPadRightMargin(0.05);
    
    // create an embedded canvas
    sprintf(cname, "%s_Emb_Canvas", name);
    fEmbCanvas = new TRootEmbeddedCanvas(cname, fFrame, 800, 600);
    Int_t wID = fEmbCanvas->GetCanvasWindowId();
    sprintf(cname, "%s_Canvas", name);
    fCanvas = new TCanvas(cname, 800, 600, wID);
    fEmbCanvas->AdoptCanvas(fCanvas);
    
    fEmbCanvas->MapSubwindows();
    fFrame->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
    
    // connect canvas info method
    fCanvas->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "TMSeqCalibModule", this, 
                    "ShowCanvasInfo(Int_t, Int_t, Int_t, TObject*)");
    
    // create control elements
    fControlFrame = new TGHorizontalFrame(fFrame, 50, 50, kFixedWidth);
    
    fPrev = new TGTextButton(fControlFrame, "Prev");
    fPrev->Connect("Clicked()", "TMSeqCalibModule", this, "ProcessPreviousElement()");
    fControlFrame->AddFrame(fPrev, new TGLayoutHints(kLHintsTop, 8, 8, 8, 8));
    
    fNext = new TGTextButton(fControlFrame, "Next");
    fNext->Connect("Clicked()", "TMSeqCalibModule", this, "ProcessNextElement()");
    fControlFrame->AddFrame(fNext, new TGLayoutHints(kLHintsTop, 8, 8, 8, 8));
 
    fRedo = new TGTextButton(fControlFrame, "Redo/Correct");
    fRedo->Connect("Clicked()", "TMSeqCalibModule", this, "Redo()");
    fControlFrame->AddFrame(fRedo, new TGLayoutHints(kLHintsTop, 8, 8, 8, 8));
    
    fQuit = new TGTextButton(fControlFrame, "Save && Quit");
    fQuit->Connect("Clicked()", "TMSeqCalibModule", this, "Quit()");
    fControlFrame->AddFrame(fQuit, new TGLayoutHints(kLHintsTop, 8, 8, 8, 8));

    fInfo = new TGLabel(fControlFrame, "                                                                                           ");
    fInfo->SetTextJustify(kTextLeft);
    fControlFrame->AddFrame(fInfo, new TGLayoutHints(kLHintsLeft, 20, 8, 11, 8));
    
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsBottom | kLHintsExpandX));
}

//______________________________________________________________________________
TMSeqCalibModule::~TMSeqCalibModule()
{
    // Destructor.
    
}

//______________________________________________________________________________
Int_t TMSeqCalibModule::GetCurrentDetectorSize() const
{
    // Return the number of elements in the current detector (check fDetID).

    if (fDetID == kBaF2_Detector) return kBaF2Size;
    else if (fDetID == kPbWO4_Detector) return kPbWO4Size;
    else if (fDetID == kVeto_Detector) return kVetoSize;
    else return 0;
}

//______________________________________________________________________________ 
void TMSeqCalibModule::ShowCanvasInfo(Int_t event, Int_t px, Int_t py, TObject *selected)
{
    // Show canvas information in the info label.
    
    fInfo->SetText(selected->GetObjectInfo(px, py));
}

//______________________________________________________________________________
void TMSeqCalibModule::ProcessNextElement()
{
    // Process the next element.
    
    // process the next element if index is within bounds
    if (fCurrentElement < GetCurrentDetectorSize() - 1) ProcessElement(fCurrentElement + 1);
}

//______________________________________________________________________________
void TMSeqCalibModule::ProcessPreviousElement()
{
    // Process the previous element.
 
    // process the previous element if index is within bounds
    if (fCurrentElement > 0) ProcessElement(fCurrentElement - 1);
}

