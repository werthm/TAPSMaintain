// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibCosmics                                                       //
//                                                                      //
// Module for the cosmics energy calibration.                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMCalibCosmics.h"

ClassImp(TMCalibCosmics)


//______________________________________________________________________________
TMCalibCosmics::TMCalibCosmics(const Char_t* name, UInt_t id)
    : TMSeqCalibModule(name, id, 3, kTRUE, kTRUE)
{
    // Constructor.
    
    // set module result header
    SetResultHeader("Results of the Cosmics Calibration module\nFormat: element id, pedestal position, muon position, gain\n");
    
    // create configuration dialog
    fConfigFrame->SetLayoutManager(new TGTableLayout(fConfigFrame, 4, 2));
    
    // create members
    fHClone = 0;
    fPedFunc = new TF1("PedFunc", "gaus", 0 , 200);
    fPeakFunc = new TF1("PeakFunc", "gaus", 0, 1500);
    fBgFunc = new TF1("BgFunc", "expo", 0, 1500);
    fTotalFunc = new TF1("TotalFunc", "gaus(0)+expo(3)", 0, 1500);
    fPedFunc->SetLineColor(kRed);
    fTotalFunc->SetLineColor(kRed);
    
    // Detector type
    TGLabel* l = new TGLabel(fConfigFrame, "Detector Type:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fTypeCombo = new TGComboBox(fConfigFrame);
    fTypeCombo->Connect("Selected(Int_t)", "TMCalibCosmics", this, "UpdateDetectorType(Int_t)");
    fTypeCombo->Resize(200, 22);
    fTypeCombo->AddEntry("BaF2 LG", ECosmics_Calib_Type_BAF2_LG);
    fTypeCombo->AddEntry("BaF2 SG", ECosmics_Calib_Type_BAF2_SG);
    fTypeCombo->AddEntry("PbWO4 LG", ECosmics_Calib_Type_PBWO4_LG);
    fTypeCombo->AddEntry("Veto", ECosmics_Calib_Type_VETO);
    fTypeCombo->AddEntry("Manual", ECosmics_Calib_Type_MANUAL);
    fConfigFrame->AddFrame(fTypeCombo, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));
    

    // ADC histogram name
    l = new TGLabel(fConfigFrame, "Histogram name:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fHNameEntry = new TGTextEntry(fConfigFrame);
    fConfigFrame->AddFrame(fHNameEntry, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 2, 2));
    
    
    // x-Axis Range
    l = new TGLabel(fConfigFrame, "Fit Range:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fRangeFrame = new TGHorizontalFrame(fConfigFrame);
    fCStartEntry = new TGTextEntry(fRangeFrame, "0");
    fCEndEntry = new TGTextEntry(fRangeFrame, "600");
    fCStartEntry->Resize(40, 22);
    fCEndEntry->Resize(40, 22);
    fRangeFrame->AddFrame(fCStartEntry, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
    fRangeFrame->AddFrame(new TGLabel(fRangeFrame, "to"), new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fRangeFrame->AddFrame(fCEndEntry, new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fConfigFrame->AddFrame(fRangeFrame, new TGTableLayoutHints(1, 2, 2, 3, kLHintsLeft, 0, 0, 0, 0));
    
    
    // Batch mode
    fBatchMode = new TGCheckButton(fConfigFrame, "Batch mode");
    fConfigFrame->AddFrame(fBatchMode, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    // select detector type
    fTypeCombo->Select(ECosmics_Calib_Type_BAF2_LG, kTRUE);

    
    // divide canvas
    fCanvas->Divide(2, 1);
}

//______________________________________________________________________________
TMCalibCosmics::~TMCalibCosmics()
{
    // Destructor.

    if (fHClone) delete fHClone;
}

//______________________________________________________________________________
void TMCalibCosmics::Init()
{
    // (Re-)initalize the module.
    
    // Clear results array
    ClearResults();
    
    // check if batch mode was selected
    if (fBatchMode->IsOn())
    {
        for (UInt_t i = 0; i < gTAPSSize; i++) ProcessElement(i);
    }
    else
    {
        // Start processing first element
        ProcessElement(0);
    }
}

//______________________________________________________________________________
void TMCalibCosmics::Redo()
{
    // Redo the current elements but change x-Axis range to 
    // look for the threshold limit in.

    ProcessElement(fCurrentElement);

}

//______________________________________________________________________________
void TMCalibCosmics::Process(Int_t index)
{
    // Fit cosmics spectrum of the element 'index'.
    
    Char_t name[256];
    TH1F* h1;
    
    // load raw spectrum
    sprintf(name, fHName, index + 1);
    h1 = (TH1F*) fFile->Get(name);

    // check if spectra could be loaded
    if (!h1)
    {
        sprintf(name, "Spectrum '%s' was not found in ROOT file!", fHName);
        ModuleError(name);
        Finished();
        return;
    }
    
    
    // ----------------------------- pedestal fitting -----------------------------
    // fit pedestal
    Double_t pedPos = h1->GetXaxis()->GetBinCenter(h1->GetMaximumBin());
    fPedFunc->SetParameters(h1->GetMaximum(), pedPos, 0.1);
    fPedFunc->SetRange(pedPos - 4, pedPos + 4);
    h1->Fit("PedFunc", "RBLL0Q");
    pedPos = fPedFunc->GetParameter(1);
    
    fCanvas->cd(1)->SetLogy();
    h1->Draw();
    fPedFunc->Draw("same");
    h1->GetXaxis()->SetRangeUser(pedPos - 7, pedPos + 7);
    h1->GetXaxis()->SetTitle("ADC Channel");
    h1->GetYaxis()->SetTitle("Counts");
    
    
    // ----------------------------- cosmic peak fitting -----------------------------
    // clone spectrum
    if (fHClone) delete fHClone;
    fHClone = (TH1F*) h1->Clone();
    
    // fit bg
    fBgFunc->SetRange(pedPos + 20, 1000);
    //fBgFunc->SetParameters(4.54372, -5.46387e-03);
    fHClone->Fit("BgFunc", "RB0Q");
    
    // fit peak
    fHClone->GetXaxis()->SetRangeUser(pedPos + 50, 1000);
    Double_t peakPos = fHClone->GetXaxis()->GetBinCenter(fHClone->GetMaximumBin());
    Double_t peakHeight = fHClone->GetBinContent(fHClone->GetMaximumBin());
    fHClone->GetXaxis()->SetRangeUser(pedPos, 1000);
    
    fPeakFunc->SetRange(peakPos - 100, peakPos + 200);
    fPeakFunc->SetParameters(peakHeight, peakPos, 10);
    fHClone->Fit("PeakFunc", "RB0Q");
    
    // fit both
    fTotalFunc->SetParameters(fPeakFunc->GetParameter(0), fPeakFunc->GetParameter(1), fPeakFunc->GetParameter(2),
                              fBgFunc->GetParameter(0), fBgFunc->GetParameter(1));
    fTotalFunc->SetRange(pedPos + 20, fCEnd);
    fHClone->Fit("TotalFunc", "RB0");                          
    
    // cosmics peak pad
    fCanvas->cd(2)->SetLogy();
    fHClone->Draw();
    fTotalFunc->Draw("same");
    fHClone->GetXaxis()->SetRangeUser(fCStart, fCEnd);
    fHClone->GetXaxis()->SetTitle("ADC Channel");
    fHClone->GetYaxis()->SetTitle("Counts");
    
    
    
    
    
    
    

    // clone spectrum
    //if (fHClone) delete fHClone;
    //fHClone = (TH1F*) h2->Clone();
    
    // save result
    //SetResult(index, 0, threshold);
    
    // draw spectrum
    /*
    fHClone->GetXaxis()->SetRangeUser(fXStart, fXEnd);
    fHClone->GetYaxis()->SetRangeUser(0, 1.1);
    fHClone->GetXaxis()->SetTitle("ADC Channel");
    fHClone->GetYaxis()->SetTitle("LED divided by raw");
    
    // draw text
    TText aText;
    aText.SetNDC(kTRUE);
    sprintf(name, "Thr.: %5.2f", threshold);
    aText.DrawText(0.1, 0.91, name);

    // draw level line
    TLine aLine;
    aLine.SetLineStyle(2);
    aLine.SetLineWidth(1);
    aLine.SetLineColor(kBlack);
    aLine.DrawLine(fXStart, fLevel, fXEnd, fLevel);

    // mark threshold position
    aLine.SetLineStyle(1);
    aLine.SetLineWidth(2);
    aLine.SetLineColor(kRed);
    aLine.DrawLine(threshold, 0, threshold, 1);

    */

}

//______________________________________________________________________________
void TMCalibCosmics::Quit()
{
    // Save cosmics calibration results and quit module.
    
    // dump results
    DumpResults("%6.2f");
    
    // save results
    //Save();
        
    // emit Finished() signal
    Finished();
}

//______________________________________________________________________________
void TMCalibCosmics::UpdateDetectorType(Int_t id)
{
    // Update the settings in the configuration dialog for the detector type the
    // user chose in the combo box
    
    if (id == ECosmics_Calib_Type_BAF2_LG)
    {
        fHNameEntry->SetText("LG/baf2-LG-%03d");
        fHNameEntry->SetEnabled(kFALSE);
    }
    if (id == ECosmics_Calib_Type_BAF2_SG)
    {
        fHNameEntry->SetText("SG/baf2-SG-%03d");
        fHNameEntry->SetEnabled(kFALSE);
    }
    if (id == ECosmics_Calib_Type_PBWO4_LG)
    {
        fHNameEntry->SetText("PWO-LG/pwo-LG-%03d");
        fHNameEntry->SetEnabled(kFALSE);
    }
    if (id == ECosmics_Calib_Type_VETO)
    {
        fHNameEntry->SetText("Veto-Q/veto-Q-%03d");
        fHNameEntry->SetEnabled(kFALSE);
    }
    else if (id == ECosmics_Calib_Type_MANUAL)
    {
        fHNameEntry->SetText("");
        fHNameEntry->SetEnabled(kTRUE);
    }
}

//______________________________________________________________________________
void TMCalibCosmics::ReadConfig()
{
    // Read the configuration made by the user in the config dialog.
    
    // copy histogram name
    strcpy(fHName, fHNameEntry->GetText());
    
    // copy x-Axis range values
    fCStart = atof(fCStartEntry->GetText());
    fCEnd = atof(fCEndEntry->GetText());
}

