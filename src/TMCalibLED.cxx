/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2017
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibLED                                                           //
//                                                                      //
// Module for the LED calibration.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include <cstdlib>

#include "TGTableLayout.h"
#include "TGLabel.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TText.h"
#include "TLine.h"

#include "TMCalibLED.h"
#include "TMUtils.h"

ClassImp(TMCalibLED)

//______________________________________________________________________________
TMCalibLED::TMCalibLED(const Char_t* name, UInt_t id)
    : TMSeqCalibModule(name, id, 1, kTRUE, kTRUE)
{
    // Constructor.

    // set module result header
    SetResultHeader("# Results of the LED Calibration module\n"
                    "# Format: element id, set LED threshold voltage, LED threshold channel\n");

    fThreshold = 0;
    fHClone = 0;
    fHDeriv = 0;
    fFunc = 0;

    // marker line
    fLine = new TLine();
    fLine->SetLineStyle(1);
    fLine->SetLineWidth(3);
    fLine->SetLineColor(kBlue);

    // create configuration dialog
    fConfigFrame->SetLayoutManager(new TGTableLayout(fConfigFrame, 7, 2));

    // LED type
    TGLabel* l = new TGLabel(fConfigFrame, "LED Type:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fTypeCombo = new TGComboBox(fConfigFrame);
    fTypeCombo->Connect("Selected(Int_t)", "TMCalibLED", this, "UpdateLEDType(Int_t)");
    fTypeCombo->Resize(200, 22);
    fTypeCombo->AddEntry("BaF2 LG LED 1", kLED_Calib_Type_LG_LED1);
    fTypeCombo->AddEntry("BaF2 LG LED 2", kLED_Calib_Type_LG_LED2);
    fTypeCombo->AddEntry("BaF2 LGS LED 1", kLED_Calib_Type_LGS_LED1);
    fTypeCombo->AddEntry("BaF2 LGS LED 2", kLED_Calib_Type_LGS_LED2);
    fTypeCombo->AddEntry("Manual", kLED_Calib_Type_MANUAL);
    fConfigFrame->AddFrame(fTypeCombo, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));

    // Uncut histogram name
    l = new TGLabel(fConfigFrame, "Uncut histogram name:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fUncut = new TGTextEntry(fConfigFrame);
    fConfigFrame->AddFrame(fUncut, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 2, 2));


    // LED cut histogram name
    l = new TGLabel(fConfigFrame, "LED cut histogram name:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fCut = new TGTextEntry(fConfigFrame);
    fConfigFrame->AddFrame(fCut, new TGTableLayoutHints(1, 2, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 2, 2));

    // threshold voltage number entry
    l = new TGLabel(fConfigFrame, "Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 3, 4, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fVoltageEntry = new TGNumberEntry(fConfigFrame, 30, 6);
    fConfigFrame->AddFrame(fVoltageEntry, new TGTableLayoutHints(1, 2, 3, 4, kLHintsLeft, 5, 5, 2, 2));

    // x-Axis Range
    l = new TGLabel(fConfigFrame, "x-Axis Range:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 4, 5, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fAxisRangeFrame = new TGHorizontalFrame(fConfigFrame);
    fXAxisStart = new TGTextEntry(fAxisRangeFrame, "0");
    fXAxisEnd = new TGTextEntry(fAxisRangeFrame, "300");
    fXAxisStart->Resize(40, 22);
    fXAxisEnd->Resize(40, 22);
    fAxisRangeFrame->AddFrame(fXAxisStart, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
    fAxisRangeFrame->AddFrame(new TGLabel(fAxisRangeFrame, "to"), new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fAxisRangeFrame->AddFrame(fXAxisEnd, new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fConfigFrame->AddFrame(fAxisRangeFrame, new TGTableLayoutHints(1, 2, 4, 5, kLHintsLeft, 0, 0, 0, 0));


    // Rebin
    l = new TGLabel(fConfigFrame, "Rebin:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 5, 6, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fRebinEntry = new TGTextEntry(fConfigFrame, "2");
    fRebinEntry->Resize(40, 22);
    fConfigFrame->AddFrame(fRebinEntry, new TGTableLayoutHints(1, 2, 5, 6, kLHintsLeft, 5, 5, 2, 2));


    // Batch mode
    fBatchMode = new TGCheckButton(fConfigFrame, "Batch mode");
    fConfigFrame->AddFrame(fBatchMode, new TGTableLayoutHints(1, 2, 6, 7, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    // needs to be called in every TMModule that uses a configuration dialog
    fConfigFrame->Layout();

    // select LED type
    fTypeCombo->Select(kLED_Calib_Type_LG_LED1, kTRUE);

    // divide canvas
    fCanvas->Divide(2, 1);
}

//______________________________________________________________________________
TMCalibLED::~TMCalibLED()
{
    // Destructor.

    if (fHClone) delete fHClone;
    if (fHDeriv) delete fHDeriv;
    if (fFunc) delete fFunc;
    if (fLine) delete fLine;
}

//______________________________________________________________________________
void TMCalibLED::Init()
{
    // (Re-)initalize the module.

    // Clear results array
    ClearResults();

    // check if batch mode was selected
    if (fBatchMode->IsOn())
    {
        for (Int_t i = 0; i < GetCurrentDetectorSize(); i++) ProcessElement(i);
    }
    else
    {
        // Start processing first element
        ProcessElement(0);
    }
}

//______________________________________________________________________________
void TMCalibLED::Process(Int_t index, Bool_t redo)
{
    // Fit LED spectrum of the element 'index'.

    Char_t name[256];
    TH1* h1;
    TH1* h2;

    // check if previous element was corrected
    if (!redo && fPreviousElement != -1)
    {
        if (fLine->GetX1() != fThreshold)
        {
            SetResult(fPreviousElement, 0, fLine->GetX1());
            Info("Process", "Corrected threshold of element %d to %f", fPreviousElement+1, fLine->GetX1());
        }
    }

    // clear canvas
    fCanvas->cd(1)->Clear();
    fCanvas->cd(2)->Clear();

    // load raw spectrum
    sprintf(name, fUncutHName, index + 1);
    h1 = (TH1*) fFile->Get(name);

    // load LED spectrum
    sprintf(name, fCutHName, index + 1);
    h2 = (TH1*) fFile->Get(name);

    // check if spectra could be loaded
    if (!h1)
    {
        sprintf(name, "Spectrum '%s' was not found in ROOT file!", fUncutHName);
        ModuleError(name);
        Finished();
        return;
    }

    if (!h2)
    {
        sprintf(name, "Spectrum '%s' was not found in ROOT file!", fCutHName);
        ModuleError(name);
        Finished();
        return;
    }

    // clone spectrum
    if (fHClone) delete fHClone;
    fHClone = (TH1*) h2->Clone();

    // divide LED spectrum
    fHClone->Divide(h1);
    fHClone->Rebin(fRebin);
    fHClone->Scale(1./(Double_t)fRebin);

    // prepare histogram
    for (Int_t i = 1; i <= fHClone->GetNbinsX(); i++)
    {
        if (fHClone->GetBinCenter(i) < fXStart ||
            fHClone->GetBinCenter(i) > fXEnd)
        {
            fHClone->SetBinContent(i, 0);
            fHClone->SetBinError(i, 0);
        }
    }

    //
    // find LED threshold position
    //

    // check histo
    if (fHClone->Integral() < 10)
    {
        fThreshold = 0;
        if (fHDeriv) delete fHDeriv;
        fHDeriv = 0;
    }
    else
    {
        // derive histogram
        if (fHDeriv) delete fHDeriv;
        fHDeriv = TMUtils::DeriveHistogram(fHClone);
        TMUtils::ZeroBins(fHDeriv);

        // get maximum value
        fThreshold = redo ? fLine->GetX1() : EstimateThreshold(fHClone, fHDeriv);

        // fit
        if (fFunc) delete fFunc;
        fFunc = new TF1("func", "gaus", fThreshold-20, fThreshold+20);
        fFunc->SetLineColor(kRed);
        fFunc->SetParameters(1, fThreshold, 1);
        fHDeriv->Fit(fFunc, "RQ0");
        fThreshold = fFunc->GetParameter(1);
    }

    // correct bad values
    if (fThreshold != 0 && (fThreshold < fXStart || fThreshold > fXEnd))
        fThreshold = (fXStart + fXEnd) / 2.;

    // save result
    SetResult(index, 0, fThreshold);

    // draw spectrum
    fCanvas->cd(1);
    fHClone->Draw("hist");
    fHClone->GetXaxis()->SetRangeUser(fXStart, fXEnd);
    fHClone->GetYaxis()->SetRangeUser(0, 1.1);
    fHClone->GetXaxis()->SetTitle("ADC Channel");
    fHClone->GetYaxis()->SetTitle("LED divided by raw");

    // draw text
    TText aText;
    aText.SetNDC(kTRUE);
    sprintf(name, "Thr.: %5.2f", fThreshold);
    aText.DrawText(0.1, 0.91, name);

    // mark threshold position
    fLine->SetX1(fThreshold);
    fLine->SetX2(fThreshold);
    fLine->SetY1(0);
    fLine->SetY2(1);
    fLine->Draw();

    // draw derivative
    if (fHDeriv)
    {
        fCanvas->cd(2);
        fHDeriv->Draw("hist");
        fFunc->Draw("same");
        fHDeriv->GetXaxis()->SetRangeUser(fThreshold-50, fThreshold+50);
        fHDeriv->GetXaxis()->SetTitle("ADC Channel");
        fHDeriv->GetYaxis()->SetTitle("Derivative");
        fLine->Draw();
    }

    // update the canvas
    fCanvas->Update();
}

//______________________________________________________________________________
Double_t TMCalibLED::EstimateThreshold(TH1* h, TH1* hDeriv)
{
    // Estimate the threshold in the histogram 'h'.

    Double_t thr = 0;

    // loop over bins
    for (Int_t i = 1; i <= h->GetNbinsX(); i++)
    {
        // position
        Double_t pos = h->GetBinCenter(i);

        // calculate integral range
        Int_t binLow = h->GetXaxis()->FindBin(pos-15);
        Int_t binHigh = h->GetXaxis()->FindBin(pos+15);

        // calculate integrals on both sides
        Double_t intLow = h->Integral(binLow, i-1);
        Double_t intHigh = (binHigh-i) - h->Integral(i+1, binHigh);

        // leave if difference crosses zero
        if (intHigh - intLow < 0)
        {
            thr = pos;
            break;
        }
    }

    // check result of search
    if (thr == 0)
        return hDeriv->GetBinCenter(hDeriv->GetMaximumBin());
    else
        return thr;
}

//______________________________________________________________________________
void TMCalibLED::SaveResults(const Char_t* filename)
{
    // Save the results array to the file 'filename'. Use numberFormat to format the array
    // content. Overwritten method of class TMModule to customize number output format

    // open output file
    FILE* fout;
    fout = fopen(filename, "w");

    // save module result header
    fprintf(fout, "%s", GetResultHeader());
    fprintf(fout, "\n");

    Double_t** results = GetResults();

    // print array content
    for (Int_t i = 0; i < GetCurrentDetectorSize(); i++)
    {
        // get element threshold voltage
        Int_t thresVol = (Int_t)fVoltage;

        fprintf(fout, "%3d  %5d  %7.3f\n", i+1, thresVol, results[i][0]);
    }

    fclose(fout);

    Info("SaveResults", "Saved results of module '%s' to file '%s'", GetName(), filename);
}

//______________________________________________________________________________
void TMCalibLED::Quit()
{
    // Save LED calibration results and quit module.

    // dump results
    //DumpResults("%6.2f");

    // save results
    Save();

    // emit Finished() signal
    Finished();
}

//______________________________________________________________________________
void TMCalibLED::UpdateLEDType(Int_t id)
{
    // Update the settings in the configuration dialog for the LED type the
    // user chose in the combo box

    if (id == kLED_Calib_Type_LG_LED1)
    {
        fUncut->SetText("BaF2_LG_%03d");
        fCut->SetText("BaF2_LG_LED1_%03d");
        fUncut->SetEnabled(kFALSE);
        fCut->SetEnabled(kFALSE);
    }
    else if (id == kLED_Calib_Type_LG_LED2)
    {
        fUncut->SetText("BaF2_LG_%03d");
        fCut->SetText("BaF2_LG_LED2_%03d");
        fUncut->SetEnabled(kFALSE);
        fCut->SetEnabled(kFALSE);
    }
    else if (id == kLED_Calib_Type_LGS_LED1)
    {
        fUncut->SetText("BaF2_LGS_%03d");
        fCut->SetText("BaF2_LGS_LED1_%03d");
        fUncut->SetEnabled(kFALSE);
        fCut->SetEnabled(kFALSE);
    }
    else if (id == kLED_Calib_Type_LGS_LED2)
    {
        fUncut->SetText("BaF2_LGS_%03d");
        fCut->SetText("BaF2_LGS_LED2_%03d");
        fUncut->SetEnabled(kFALSE);
        fCut->SetEnabled(kFALSE);
    }
    else if (id == kLED_Calib_Type_MANUAL)
    {
        fUncut->SetText("");
        fCut->SetText("");
        fUncut->SetEnabled(kTRUE);
        fCut->SetEnabled(kTRUE);
    }
}

//______________________________________________________________________________
void TMCalibLED::ReadConfig()
{
    // Read the configuration made by the user in the config dialog.

    // set detector id
    Int_t type = fTypeCombo->GetSelected();
    if (type == kLED_Calib_Type_LG_LED1)         fDetID = kBaF2_Detector;
    else if (type == kLED_Calib_Type_LG_LED2)    fDetID = kBaF2_Detector;
    else if (type == kLED_Calib_Type_LGS_LED1)   fDetID = kBaF2_Detector;
    else if (type == kLED_Calib_Type_LGS_LED2)   fDetID = kBaF2_Detector;
    else if (type == kLED_Calib_Type_MANUAL)     fDetID = kBaF2_Detector;

    // copy histogram names
    strcpy(fUncutHName, fUncut->GetText());
    strcpy(fCutHName, fCut->GetText());

    // copy x-Axis range values
    fXStart = atof(fXAxisStart->GetText());
    fXEnd = atof(fXAxisEnd->GetText());

    // copy rebin
    fRebin = atoi(fRebinEntry->GetText());

    // read ring threshold voltages
    fVoltage = fVoltageEntry->GetNumber();
}

