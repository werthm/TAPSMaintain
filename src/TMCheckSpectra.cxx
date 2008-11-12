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
     
    fFrame->SetLayoutManager(new TGHorizontalLayout(fFrame));
    
    // ------------------------------ Control frame ------------------------------
    fControlFrame = new TGCompositeFrame(fFrame, 50, 50);
    fControlFrame->SetLayoutManager(new TGTableLayout(fControlFrame, 8, 2));
    
    TGLabel* l = new TGLabel(fControlFrame, "Spectrum class and element selection:");
    l->SetTextJustify(kTextLeft);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    
    // add element spectra combo box
    fSpectraCombo = new TGComboBox(fControlFrame);
    fSpectraCombo->Resize(150, 22);
    fSpectraCombo->Connect("Selected(Int_t)", "TMCheckSpectra", this, "SpectraClassChanged(Int_t)");
    fSpectraCombo->AddEntry("Select class"         , ESpec_Empty);           
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
    
    // add element selection type combo box
    fElementCombo = new TGComboBox(fControlFrame);
    fElementCombo->Resize(150, 22);
    fElementCombo->Connect("Selected(Int_t)", "TMCheckSpectra", this, "ElementRangeChanged(Int_t)");
    fElementCombo->AddEntry("Single element", ERange_Single_Element);
    fElementCombo->AddEntry("Block A elements", ERange_Block_A);
    fElementCombo->AddEntry("Block B elements", ERange_Block_B);
    fElementCombo->AddEntry("Block C elements", ERange_Block_C);
    fElementCombo->AddEntry("Block D elements", ERange_Block_D);
    fElementCombo->AddEntry("Block E elements", ERange_Block_E);
    fElementCombo->AddEntry("Block F elements", ERange_Block_F);
    fControlFrame->AddFrame(fElementCombo, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    fElementCombo->Select(ERange_Single_Element, kFALSE);
     
    // add element number entry
    fElementNumberEntry = new TGNumberEntry(fControlFrame, 1, 3);
    fElementNumberEntry->Connect("ValueSet(Long_t)", "TMCheckSpectra", this, "ElementNumberChanged()");
    fElementNumberEntry->SetLimits(TGNumberFormat::kNELLimitMinMax);
    fControlFrame->AddFrame(fElementNumberEntry, new TGTableLayoutHints(1, 2, 2, 3, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
 
    l = new TGLabel(fControlFrame, "Pattern spectra:");
    l->SetTextJustify(kTextLeft);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 2, 3, 4, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    
    // create spectra list 
    fSpList = new TGListBox(fControlFrame);
    fSpList->Resize(200, 400);
    fSpList->Connect("Selected(Int_t)", "TMCheckSpectra", this, "DrawPatternHistogram(Int_t)");
    fControlFrame->AddFrame(fSpList, new TGTableLayoutHints(0, 2, 4, 5, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));

    // log y-axis checkbox
    fCheckLogy = new TGCheckButton(fControlFrame, "Logarithmic y-Axis");
    fCheckLogy->Connect("Clicked()", "TMCheckSpectra", this, "DrawHistogram()");
    fCheckLogy->SetState(kButtonDown, kFALSE);
    fControlFrame->AddFrame(fCheckLogy, new TGTableLayoutHints(0, 2, 5, 6, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));
    
    // x-axis range
    l = new TGLabel(fControlFrame, "x-Axis start:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 6, 7, kLHintsFillX | kLHintsLeft, 5, 5, 7, 2));
    fXStart = new TGNumberEntry(fControlFrame, 0, 6);
    fControlFrame->AddFrame(fXStart, new TGTableLayoutHints(1, 2, 6, 7, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    l = new TGLabel(fControlFrame, "x-Axis end:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 7, 8, kLHintsFillX | kLHintsLeft, 5, 5, 7, 2));
    fXEnd = new TGNumberEntry(fControlFrame, 0, 6);
    fControlFrame->AddFrame(fXEnd, new TGTableLayoutHints(1, 2, 7, 8, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
      
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
void TMCheckSpectra::DrawSingleHistogram()
{
    // Draw a histogram of a single detector element.

    Char_t name[256];
    Char_t hName[256];

    // exit if no class was selected
    if (fSpectraCombo->GetSelected() == ESpec_Empty) return;
 
    // construct histogram name
    sprintf(hName, specNames[fSpectraCombo->GetSelected()], (Int_t)fElementNumberEntry->GetNumber());

    // load histogram
    TH1F* h = (TH1F*) fFile->Get(hName);

    // draw if histogram exists
    if (h) 
    {
        TVirtualPad* pad = fCanvas->cd();
        
        // check for log y-axis
        if (fCheckLogy->IsOn()) pad->SetLogy(kTRUE);
        else pad->SetLogy(kFALSE);

        // check for x-axis range ajustment
        if (fXStart->GetNumber() != 0 || fXEnd->GetNumber() != 0) 
            h->GetXaxis()->SetRange(fXStart->GetNumber(), fXEnd->GetNumber());
        
        // draw
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
void TMCheckSpectra::DrawMultipleHistograms()
{
    // Draw multiple histograms of a certain detector element range.

    Char_t hName[256];
    Int_t elements[gMaxSize];
    TH1F* histos[gMaxSize];
    UInt_t nelements = 0;
    UInt_t divisions[2] = {1, 1};

    // exit if no class was selected
    if (fSpectraCombo->GetSelected() == ESpec_Empty) return;
 
    // fill element array
    switch (fElementCombo->GetSelected())
    {
        // block A
        case ERange_Block_A:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+1;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
        // block B
        case ERange_Block_B:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+65;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
         // block C
         case ERange_Block_C:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+129;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
         // block D
         case ERange_Block_D:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+193;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
         // block E
         case ERange_Block_E:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+257;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
         // block F
         case ERange_Block_F:
            nelements = 64;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+321;
            divisions[0] = 8;
            divisions[1] = 8;
            break;
         // all PWO elements
         case ERange_All_PWO_Elements:
            nelements = 24;
            for (UInt_t i = 0; i < nelements; i++) elements[i] = i+1;
            divisions[0] = 4;
            divisions[1] = 6;
            break;
        default:
            break;
    }

    fCanvas->Clear();
    fCanvas->Divide(divisions[0], divisions[1], 0.001, 0.001);
    
    // load histograms
    for (UInt_t i = 0; i < nelements; i++)
    {
        sprintf(hName, specNames[fSpectraCombo->GetSelected()], elements[i]);
        histos[i] = (TH1F*) fFile->Get(hName);
        
        TVirtualPad* pad = fCanvas->cd(i+1);
        pad->SetLeftMargin(0.0000001);
        pad->SetRightMargin(0.0000001);
        pad->SetTopMargin(0.00000001);
        pad->SetBottomMargin(0.0000001);
        
        // check for log. y-axis
        if (fCheckLogy->IsOn()) pad->SetLogy(kTRUE);
        else pad->SetLogy(kFALSE);
        
        // check for x-axis range ajustment
        if (fXStart->GetNumber() != 0 || fXEnd->GetNumber() != 0) 
            histos[i]->GetXaxis()->SetRange(fXStart->GetNumber(), fXEnd->GetNumber());
   
        histos[i]->Draw();
    }
    
    fCanvas->Update();
}

//______________________________________________________________________________
void TMCheckSpectra::DrawPatternHistogram(Int_t id)
{
    // Load a pattern histogram from the ROOT input file and draw it
    // in the embedded canvas.

    // get selected entry
    TGTextLBEntry* anEntry = (TGTextLBEntry*) fSpList->GetEntry(id);

    // load histogram
    TH1F* h = (TH1F*) fFile->Get(anEntry->GetText()->GetString());

    // draw if histogram exists
    if (h) 
    {
        TVirtualPad* pad = fCanvas->cd();
        
        // check for log. y-axis
        if (fCheckLogy->IsOn()) pad->SetLogy(kTRUE);
        else pad->SetLogy(kFALSE);
        
        // check for x-axis range ajustment
        if (fXStart->GetNumber() != 0 || fXEnd->GetNumber() != 0) 
            h->GetXaxis()->SetRange(fXStart->GetNumber(), fXEnd->GetNumber());
     
        // draw
        h->Draw();
        fCanvas->Update();
    }
}

//______________________________________________________________________________
void TMCheckSpectra::ElementNumberChanged()
{
    // Called when the user changed the element number.
    
    // only drawing of single histograms possible here
    DrawSingleHistogram();
}

//______________________________________________________________________________
void TMCheckSpectra::ElementRangeChanged(Int_t id)
{
    // Called when the user changed the element range.
    
    if (id == ERange_Single_Element) fElementNumberEntry->SetState(kTRUE);
    else fElementNumberEntry->SetState(kFALSE);

    DrawHistogram();
}

//______________________________________________________________________________
void TMCheckSpectra::SpectraClassChanged(Int_t id)
{
    // Called when the user changed the spectra class.
    
    // Reset spectrum class combo box and clear canvas
    if (id == ESpec_Empty)
    {
        fElementNumberEntry->SetLimitValues(0, 0);
        fCanvas->Clear();
        fCanvas->Update();
        return;
    }

    // Set detector number entry limits
    if (id == ESpec_PWO_LG      ||
        id == ESpec_PWO_LG_TIME ||
        id == ESpec_PWO_LGS     ||
        id == ESpec_PWO_TIME    ||
        id == ESpec_PWO_TIME_MULT) 
    {
        // limit element numbers
        fElementNumberEntry->SetLimitValues(1, gPbWO4Size);
        
        // recreate element combo box
        fElementCombo->RemoveAll();
        fElementCombo->AddEntry("Single element", ERange_Single_Element);
        fElementCombo->AddEntry("All elements", ERange_All_PWO_Elements);
        fElementCombo->GetListBox()->Resize(150, 40);
        fElementCombo->Select(ERange_Single_Element, kFALSE);
    }
    else 
    {
        // limit element numbers
        fElementNumberEntry->SetLimitValues(1, gBaF2Size);
    
        // recreate element combo box
        fElementCombo->RemoveAll();
        fElementCombo->AddEntry("Single element", ERange_Single_Element);
        fElementCombo->AddEntry("Block A elements", ERange_Block_A);
        fElementCombo->AddEntry("Block B elements", ERange_Block_B);
        fElementCombo->AddEntry("Block C elements", ERange_Block_C);
        fElementCombo->AddEntry("Block D elements", ERange_Block_D);
        fElementCombo->AddEntry("Block E elements", ERange_Block_E);
        fElementCombo->AddEntry("Block F elements", ERange_Block_F);
        fElementCombo->GetListBox()->Resize(150, 120);
        fElementCombo->Select(ERange_Single_Element, kFALSE);
    }

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
    
    // Reset spectrum class combo box
    fElementNumberEntry->SetIntNumber(0);
    fElementNumberEntry->SetLimitValues(0, 0);
    fSpectraCombo->Select(ESpec_Empty, kFALSE);

    // reset axis settings
    fCheckLogy->SetState(kButtonDown, kFALSE);
    fXStart->SetNumber(0.);
    fXEnd->SetNumber(0.);
    
    // clear canvas
    fCanvas->Clear();

    // read in spectra
    fSpList->RemoveAll();
    TIter next(fFile->GetListOfKeys());
    TKey* key;
    
    // read all keys
    Int_t id = 0;
    while ((key = (TKey*)next()))
    {
        //TObject* obj = key->ReadObj();
        TString cname = key->GetClassName();
        TString name = key->GetName();

        // 1-dim. histograms
        if (TMUtils::IndexOf(cname.Data(), "TH1") == 0 ||
            TMUtils::IndexOf(cname.Data(), "TH2") == 0)
        {
            fSpList->AddEntry(StrDup(name), id++);
        }
    }

    fSpList->Layout();
}

