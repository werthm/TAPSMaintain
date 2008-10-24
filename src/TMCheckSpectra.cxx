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


#include "TMCheckSpectra.h"

ClassImp(TMCheckSpectra)


//______________________________________________________________________________
TMCheckSpectra::TMCheckSpectra(const Char_t* name, UInt_t id)
    : TMModule(name, id, 0, kTRUE, kFALSE)
{
    // Constructor.
    
    fFrame->SetLayoutManager(new TGHorizontalLayout(fFrame));
    
    // ------------------------------ Control frame ------------------------------
    fControlFrame = new TGCompositeFrame(fFrame, 50, 50);
    fControlFrame->SetLayoutManager(new TGTableLayout(fControlFrame, 2, 2));
    
    TGLabel* l = new TGLabel(fControlFrame, "Spectrum class and detector number:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    
    // add element spectra combo box
    fSpectraCombo = new TGComboBox(fControlFrame);
    fSpectraCombo->Resize(135, 22);
    fSpectraCombo->Connect("Selected(Int_t)", "TMCheckSpectra", this, "SpectraSelectionChanged(Int_t)");
    fSpectraCombo->AddEntry("BaF2 LG"              , ESpec_BaF2_LG);           
    fSpectraCombo->AddEntry("BaF2 LG_LED1"         , ESpec_BaF2_LG_LED1);
    fSpectraCombo->AddEntry("BaF2 LG_LED2"         , ESpec_BaF2_LG_LED2);
    fSpectraCombo->AddEntry("BaF2 LGbed"           , ESpec_BaF2_LGbed);
    fSpectraCombo->AddEntry("BaF2 LGnoP"           , ESpec_BaF2_LGnoP);
    fSpectraCombo->AddEntry("BaF2 LGS"             , ESpec_BaF2_LGS);
    fSpectraCombo->AddEntry("BaF2 SG"              , ESpec_BaF2_SG);
    fSpectraCombo->AddEntry("BaF2 SGS"             , ESpec_BaF2_SGS); 
    fSpectraCombo->AddEntry("BaF2 TIME"            , ESpec_BaF2_TIME);
    fSpectraCombo->AddEntry("BaF2 T"               , ESpec_BaF2_T);
    fSpectraCombo->AddEntry("BaF2 TDIFF"           , ESpec_BaF2_TDIFF);
    fSpectraCombo->AddEntry("Veto Q"               , ESpec_Veto_Q);
    fSpectraCombo->AddEntry("Veto QLED"            , ESpec_Veto_QLED);
    fSpectraCombo->AddEntry("Veto T"               , ESpec_Veto_T);
    fSpectraCombo->AddEntry("T_DIFF_BAFVETO"       , ESpec_T_DIFF_BAFVETO_single);
    fSpectraCombo->AddEntry("PWO LG"               , ESpec_PWO_LG);
    fSpectraCombo->AddEntry("PWO LG-TIME"          , ESpec_PWO_LG_TIME);
    fSpectraCombo->AddEntry("PWO LGS"              , ESpec_PWO_LGS);
    fSpectraCombo->AddEntry("PWO TIME"             , ESpec_PWO_TIME);
    fSpectraCombo->AddEntry("PWO TIME MULT"        , ESpec_PWO_TIME_MULT);
    fControlFrame->AddFrame(fSpectraCombo, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    fSpectraCombo->Select(ESpec_BaF2_LG, kFALSE);
    
    // add element number entry
    fElementNumberEntry = new TGNumberEntry(fControlFrame, 1, 3);
    fElementNumberEntry->Connect("ValueSet(Long_t)", "TMCheckSpectra", this, "DrawHistogram()");
    fElementNumberEntry->SetLimits(TGNumberFormat::kNELLimitMinMax);
    fElementNumberEntry->SetLimitValues(1, gBaF2Size);
    fControlFrame->AddFrame(fElementNumberEntry, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));

    // add control frame to main frame
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsFillX | kLHintsFillY, 10, 10, 10, 10));
    fControlFrame->Resize(fControlFrame->GetDefaultSize()); 

    // ------------------------------ Canvas frame ------------------------------
    fCanvasFrame = new TGVerticalFrame(fFrame, 800, 600);
    
    // create the canvas
    fEmbCanvas = new TRootEmbeddedCanvas("EmbeddedCanvas", fCanvasFrame, 800, 600);
    Int_t wID = fEmbCanvas->GetCanvasWindowId();
    fCanvas = new TCanvas("TAPSMaintainCanvas", 800, 600, wID);
    fEmbCanvas->AdoptCanvas(fCanvas);

    // add canvas to canvas frame
    fCanvas->SetBorderMode(0);
    fEmbCanvas->MapSubwindows();
    fCanvasFrame->AddFrame(fEmbCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));
    
    // add canvas frame to main frame
    fFrame->AddFrame(fCanvasFrame, new TGLayoutHints(kLHintsTop | kLHintsRight| kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
}

//______________________________________________________________________________
TMCheckSpectra::~TMCheckSpectra()
{
    // Destructor.
    
}

//______________________________________________________________________________
void TMCheckSpectra::DrawHistogram()
{
    // Load the histogram from the ROOT input file and draw it
    // in the embedded canvas.

    Char_t name[256];
    Char_t hName[256];
  
    // construct histogram name
    sprintf(hName, specNames[fSpectraCombo->GetSelected()], (Int_t)fElementNumberEntry->GetNumber());

    // load histogram
    TH1F* h = (TH1F*) fFile->Get(hName);

    // draw if histogram exists
    if (h) 
    {
        fCanvas->cd();
        h->Draw();
        fCanvas->Update();
    }
    else 
    {
        sprintf(name, "Spectrum '%s' was not found in ROOT file!", hName);
        //ModuleError(name);
        printf("%s\n", name);
    }
}

//______________________________________________________________________________
void TMCheckSpectra::SpectraSelectionChanged(Int_t id)
{
    // Update the GUI after the user has changed the spectra combo box.
    
    // Set detector number entry limits
    if (id == ESpec_PWO_LG      ||
        id == ESpec_PWO_LG_TIME ||
        id == ESpec_PWO_LGS     ||
        id == ESpec_PWO_TIME    ||
        id == ESpec_PWO_TIME_MULT) fElementNumberEntry->SetLimitValues(1, gPbWO4Size);
    else fElementNumberEntry->SetLimitValues(1, gBaF2Size);

    DrawHistogram();
}

//______________________________________________________________________________
void TMCheckSpectra::Cleanup()
{
    // Cleanup memory.
    
}

//______________________________________________________________________________
void TMCheckSpectra::Init()
{
    // (Re-)initalize the module.
    
    // Draw default histogram
    fElementNumberEntry->SetIntNumber(1);
    fSpectraCombo->Select(ESpec_BaF2_LG, kFALSE);
    DrawHistogram();
}

