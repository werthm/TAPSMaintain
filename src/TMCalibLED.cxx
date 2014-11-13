/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
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
    
    fHClone = 0;

    // create configuration dialog
    fConfigFrame->SetLayoutManager(new TGTableLayout(fConfigFrame, 17, 2));
    
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
    
    // threshold voltage number entries
    // ring 1
    l = new TGLabel(fConfigFrame, "Ring 1 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 3, 4, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing1Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing1Voltage, new TGTableLayoutHints(1, 2, 3, 4, kLHintsLeft, 5, 5, 2, 2));
    
    // ring 2
    l = new TGLabel(fConfigFrame, "Ring 2 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 4, 5, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing2Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing2Voltage, new TGTableLayoutHints(1, 2, 4, 5, kLHintsLeft, 5, 5, 2, 2));

    // ring 3
    l = new TGLabel(fConfigFrame, "Ring 3 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 5, 6, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing3Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing3Voltage, new TGTableLayoutHints(1, 2, 5, 6, kLHintsLeft, 5, 5, 2, 2));

    // ring 4
    l = new TGLabel(fConfigFrame, "Ring 4 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 6, 7, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing4Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing4Voltage, new TGTableLayoutHints(1, 2, 6, 7, kLHintsLeft, 5, 5, 2, 2));

    // ring 5
    l = new TGLabel(fConfigFrame, "Ring 5 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 7, 8, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing5Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing5Voltage, new TGTableLayoutHints(1, 2, 7, 8, kLHintsLeft, 5, 5, 2, 2));

    // ring 6
    l = new TGLabel(fConfigFrame, "Ring 6 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 8, 9, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing6Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing6Voltage, new TGTableLayoutHints(1, 2, 8, 9, kLHintsLeft, 5, 5, 2, 2));

    // ring 7
    l = new TGLabel(fConfigFrame, "Ring 7 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 9, 10, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing7Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing7Voltage, new TGTableLayoutHints(1, 2, 9, 10, kLHintsLeft, 5, 5, 2, 2));

    // ring 8
    l = new TGLabel(fConfigFrame, "Ring 8 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 10, 11, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing8Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing8Voltage, new TGTableLayoutHints(1, 2, 10, 11, kLHintsLeft, 5, 5, 2, 2));

    // ring 9
    l = new TGLabel(fConfigFrame, "Ring 9 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 11, 12, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing9Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing9Voltage, new TGTableLayoutHints(1, 2, 11, 12, kLHintsLeft, 5, 5, 2, 2));

    // ring 10
    l = new TGLabel(fConfigFrame, "Ring 10 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 12, 13, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing10Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing10Voltage, new TGTableLayoutHints(1, 2, 12, 13, kLHintsLeft, 5, 5, 2, 2));

    // ring 11
    l = new TGLabel(fConfigFrame, "Ring 11 Threshold [mV]:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 13, 14, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    fRing11Voltage = new TGNumberEntry(fConfigFrame, 1100, 6);
    fConfigFrame->AddFrame(fRing11Voltage, new TGTableLayoutHints(1, 2, 13, 14, kLHintsLeft, 5, 5, 2, 2));

    
    // x-Axis Range
    l = new TGLabel(fConfigFrame, "x-Axis Range:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 14, 15, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fAxisRangeFrame = new TGHorizontalFrame(fConfigFrame);
    fXAxisStart = new TGTextEntry(fAxisRangeFrame, "0");
    fXAxisEnd = new TGTextEntry(fAxisRangeFrame, "300");
    fXAxisStart->Resize(40, 22);
    fXAxisEnd->Resize(40, 22);
    fAxisRangeFrame->AddFrame(fXAxisStart, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
    fAxisRangeFrame->AddFrame(new TGLabel(fAxisRangeFrame, "to"), new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fAxisRangeFrame->AddFrame(fXAxisEnd, new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fConfigFrame->AddFrame(fAxisRangeFrame, new TGTableLayoutHints(1, 2, 14, 15, kLHintsLeft, 0, 0, 0, 0));
    
    
    // Threshold level
    l = new TGLabel(fConfigFrame, "Threshold Level:");
    l->SetTextJustify(kTextRight); 
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 15, 16, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fThresholdLevel = new TGTextEntry(fConfigFrame, "0.9");
    fThresholdLevel->Resize(40, 22);
    fConfigFrame->AddFrame(fThresholdLevel, new TGTableLayoutHints(1, 2, 15, 16, kLHintsLeft, 5, 5, 2, 2));
    
    
    // Batch mode
    fBatchMode = new TGCheckButton(fConfigFrame, "Batch mode");
    fConfigFrame->AddFrame(fBatchMode, new TGTableLayoutHints(1, 2, 16, 17, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    // needs to be called in every TMModule that uses a configuration dialog
    fConfigFrame->Layout();
    
    // select LED type
    fTypeCombo->Select(kLED_Calib_Type_LG_LED1, kTRUE);
}

//______________________________________________________________________________
TMCalibLED::~TMCalibLED()
{
    // Destructor.

    if (fHClone) delete fHClone;
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
void TMCalibLED::Redo()
{
    // Redo the current elements but change x-Axis range to 
    // look for the threshold limit in.

    // backup old range values
    Double_t oldStart = fXStart;
    Double_t oldEnd = fXEnd;

    // get user selected range
    fXStart = fHClone->GetXaxis()->GetBinCenter(fHClone->GetXaxis()->GetFirst());
    fXEnd = fHClone->GetXaxis()->GetBinCenter(fHClone->GetXaxis()->GetLast());

    ProcessElement(fCurrentElement);

    // reset old range values
    fXStart = oldStart;
    fXEnd = oldEnd;
}

//______________________________________________________________________________
void TMCalibLED::Process(Int_t index)
{
    // Fit LED spectrum of the element 'index'.
    
    Char_t name[256];
    TH1F* h1;
    TH1F* h2;
    Double_t threshold;
    
    // load raw spectrum
    sprintf(name, fUncutHName, index + 1);
    h1 = (TH1F*) fFile->Get(name);

    // load LED spectrum
    sprintf(name, fCutHName, index + 1);
    h2 = (TH1F*) fFile->Get(name);
    
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
    fHClone = (TH1F*) h2->Clone();
    
    // divide LED spectrum
    fHClone->Divide(h1);
    fHClone->Rebin(2);
    fHClone->Scale(1./2.);

    // find LED threshold position
    threshold = FindThreshold(fHClone);
    
    // save result
    SetResult(index, 0, threshold);
    
    // draw spectrum
    fHClone->Draw();
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
        Int_t thresVol = (Int_t)fRingVoltages[TMUtils::GetRingNumber(i)-1];
        
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
Double_t TMCalibLED::FindThreshold(TH1F* h)
{
    // Find the position where the bin content of h changes from
    // < fLevel to > fLevel.

    Double_t pos;
    Double_t posUnder = 0;
    Double_t content;

    // loop over all bins
    for (Int_t i = 0; i < h->GetNbinsX(); i++)
    {
        pos = h->GetBinCenter(i);
        content = h->GetBinContent(i);
     
        if (pos < fXStart || pos > fXEnd) continue;

        if (content > fLevel) return (pos + posUnder) / 2.;   
        else posUnder = pos;
    }
    
    // return 0 if position was not found
    return 0;
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
    
    // copy threshold level
    fLevel = atof(fThresholdLevel->GetText());
    
    // read ring threshold voltages
    fRingVoltages[0] = fRing1Voltage->GetNumber();
    fRingVoltages[1] = fRing2Voltage->GetNumber();
    fRingVoltages[2] = fRing3Voltage->GetNumber();
    fRingVoltages[3] = fRing4Voltage->GetNumber();
    fRingVoltages[4] = fRing5Voltage->GetNumber();
    fRingVoltages[5] = fRing6Voltage->GetNumber();
    fRingVoltages[6] = fRing7Voltage->GetNumber();
    fRingVoltages[7] = fRing8Voltage->GetNumber();
    fRingVoltages[8] = fRing9Voltage->GetNumber();
    fRingVoltages[9] = fRing10Voltage->GetNumber();
    fRingVoltages[10] = fRing11Voltage->GetNumber();
}

