/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2014
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
    
    // init members
    fCStart = 0;
    fCEnd = 0;
    fPeakRange = 0;
    fPeakLimMin = 0;
    fPeakLimMax = 0;
    fPeakWidthFac = 0;

    // set module result header
    SetResultHeader("# Results of the Cosmics Calibration module\n"
                    "# Format: element id, pedestal position, cosmic peak position, gain\n");
    
    // create configuration dialog
    fConfigFrame->SetLayoutManager(new TGTableLayout(fConfigFrame, 4, 2));
    
    // create members
    fHClone = 0;
    fPedFunc = new TF1("PedFunc", "gaus", 0 , 200);
    fPeakFunc = new TF1("PeakFunc", "gaus", 0, 4000);
    fBgFunc = new TF1("BgFunc", "expo", 0, 4000);
    fTotalFunc = new TF1("TotalFunc", "gaus(0)+expo(3)", 0, 4000);
    fPedFunc->SetLineColor(kRed);
    fBgFunc->SetLineColor(kGreen);
    fTotalFunc->SetLineColor(kRed);

    // Detector type
    TGLabel* l = new TGLabel(fConfigFrame, "Detector Type:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fTypeCombo = new TGComboBox(fConfigFrame);
    fTypeCombo->Connect("Selected(Int_t)", "TMCalibCosmics", this, "UpdateDetectorType(Int_t)");
    fTypeCombo->Resize(200, 22);
    fTypeCombo->AddEntry("BaF2 LG",    kCosmics_Calib_Type_BAF2_LG);
    fTypeCombo->AddEntry("BaF2 LGS",   kCosmics_Calib_Type_BAF2_LGS);
    fTypeCombo->AddEntry("BaF2 SG",    kCosmics_Calib_Type_BAF2_SG);
    fTypeCombo->AddEntry("BaF2 SGS",   kCosmics_Calib_Type_BAF2_SGS);
    fTypeCombo->AddEntry("Veto",       kCosmics_Calib_Type_VETO);
    fTypeCombo->AddEntry("PWO",        kCosmics_Calib_Type_PWO);
    fTypeCombo->AddEntry("PWO S",      kCosmics_Calib_Type_PWO_S);
    fTypeCombo->AddEntry("PWO Veto",   kCosmics_Calib_Type_PWO_VETO);
    fTypeCombo->AddEntry("PWO Veto S", kCosmics_Calib_Type_PWO_VETO_S);
    fConfigFrame->AddFrame(fTypeCombo, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));

    // ADC histogram name
    l = new TGLabel(fConfigFrame, "Histogram name:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fHNameEntry = new TGTextEntry(fConfigFrame);
    fConfigFrame->AddFrame(fHNameEntry, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 2, 2));
    
    
    // Fit Range
    l = new TGLabel(fConfigFrame, "Fit Range:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    fRangeFrame = new TGHorizontalFrame(fConfigFrame);
    fCStartEntry = new TGTextEntry(fRangeFrame, "30");
    fCEndEntry = new TGTextEntry(fRangeFrame, "1000");
    fCStartEntry->Resize(40, 22);
    fCEndEntry->Resize(40, 22);
    fRangeFrame->AddFrame(new TGLabel(fRangeFrame, "Ped. + "), new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fRangeFrame->AddFrame(fCStartEntry, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5, 2, 2));
    fRangeFrame->AddFrame(new TGLabel(fRangeFrame, "to"), new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fRangeFrame->AddFrame(fCEndEntry, new TGLayoutHints(kLHintsExpandY, 5, 5, 2, 2));
    fConfigFrame->AddFrame(fRangeFrame, new TGTableLayoutHints(1, 2, 2, 3, kLHintsLeft, 0, 0, 0, 0));
    
    
    // Batch mode
    fBatchMode = new TGCheckButton(fConfigFrame, "Batch mode");
    fConfigFrame->AddFrame(fBatchMode, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));
    
    // needs to be called in every TMModule that uses a configuration dialog
    fConfigFrame->Layout();
    
    // select detector type
    fTypeCombo->Select(kCosmics_Calib_Type_BAF2_LG, kTRUE);

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
        for (Int_t i = 0; i < GetCurrentDetectorSize(); i++) ProcessElement(i);
    }
    else
    {
        // Start processing first element
        ProcessElement(0);
    }
}

//______________________________________________________________________________
void TMCalibCosmics::Cleanup()
{
    // Cleanup memory.
    
    if (fHClone) 
    {
        delete fHClone;
        fHClone = 0;
    }
}

//______________________________________________________________________________
void TMCalibCosmics::Redo()
{
    // Take the position of the line that was moved manually by the user as 
    // the position of the cosmics peak.

    Char_t msg[256];

    // get peak position from line
    Double_t peakPos = fPeakLine->GetX1();

    // get fitted pedestal position
    Double_t pedPos = GetResult(fCurrentElement, 0);
    
    // calculate gain
    Double_t energyLoss = 0;
    
    // check detector type
    if (fDetID == kBaF2_Detector)            energyLoss = kTAPS_MIP_Loss_BaF2;
    else if (fDetID == kVeto_Detector)       energyLoss = kTAPS_MIP_Loss_Veto;
    else if (fDetID == kPbWO4_Detector)      energyLoss = kTAPS_MIP_Loss_PbWO4;
    else if (fDetID == kPbWO4_Veto_Detector) energyLoss = kTAPS_MIP_Loss_Veto;
    
    Double_t gain = energyLoss / (peakPos - pedPos); 
    
    // save updated results: cosmic peak position, gain
    SetResult(fCurrentElement, 1, peakPos);
    SetResult(fCurrentElement, 2, gain);

    // inform the user
    sprintf(msg, "The cosmics peak position was manually set to %6.2f", peakPos);
    ModuleInfo(msg);
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
    
    // set title
    sprintf(name, "%s (Ring %d, Block %c)", 
            h1->GetName(), TMUtils::GetRingNumber(index), TMUtils::GetBlock(index));
    h1->SetTitle(name);
    

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
    
    // draw position line
    TLine aLine;
    aLine.SetLineStyle(2);
    aLine.SetLineWidth(2);
    aLine.SetLineColor(kBlue);
    aLine.DrawLine(pedPos, 0, pedPos, h1->GetMaximum());
    
    
    // ----------------------------- cosmic peak fitting -----------------------------
    // clone spectrum
    if (fHClone) delete fHClone;
    fHClone = (TH1F*) h1->Clone();
    
    // fit bg
    fBgFunc->SetParameters(1, -0.01);
    fBgFunc->SetRange(pedPos + 20, fCEnd);
    fHClone->GetXaxis()->SetRangeUser(pedPos, fCEnd);
    fHClone->Fit("BgFunc", "RB0Q");
    
    // fit peak
    fHClone->GetXaxis()->SetRangeUser(pedPos + 50, fCEnd);
    Double_t peakPos = fHClone->GetXaxis()->GetBinCenter(fHClone->GetMaximumBin());
    Double_t peakHeight = fHClone->GetBinContent(fHClone->GetMaximumBin());
    fHClone->GetXaxis()->SetRangeUser(pedPos, fCEnd);
    
    fPeakFunc->SetRange(peakPos - fPeakRange, peakPos + fPeakRange);
    fPeakFunc->SetParameters(peakHeight, peakPos, 10);
    fHClone->Fit("PeakFunc", "RB0Q");
    
    // fit both
    if (fDetID == kPbWO4_Detector) 
    {
        if (peakPos < fPeakLimMin) peakPos = fPeakLimMin*1.1;
        fTotalFunc->SetParameters(peakHeight, peakPos, 2.5*fPeakWidthFac, 8, -1.3e-2);
        fTotalFunc->SetParLimits(0, 0.01, peakHeight*1.1);
        fTotalFunc->SetParLimits(1, fPeakLimMin, fPeakLimMax);
        fTotalFunc->SetParLimits(2, fPeakWidthFac, 4*fPeakWidthFac);
    }
    else 
    {
        fTotalFunc->SetParameters(fPeakFunc->GetParameter(0), fPeakFunc->GetParameter(1), fPeakFunc->GetParameter(2),
                              fBgFunc->GetParameter(0), fBgFunc->GetParameter(1));
        fTotalFunc->SetParLimits(0, 10, peakHeight);
        fTotalFunc->SetParLimits(1, fPeakLimMin, fPeakLimMax);
        fTotalFunc->SetParLimits(2, fPeakWidthFac, 6*fPeakWidthFac);
    }
    
    fTotalFunc->SetRange(pedPos + fCStart, fCEnd);
    fHClone->Fit("TotalFunc", "RB0Q"); 
    peakPos = fTotalFunc->GetParameter(1);  
    fBgFunc->SetParameters(fTotalFunc->GetParameter(3), fTotalFunc->GetParameter(4));
    
    // cosmics peak pad
    fCanvas->cd(2)->SetLogy();
    fHClone->Draw();
    fTotalFunc->Draw("same");
    fBgFunc->Draw("same");
    fHClone->GetXaxis()->SetRangeUser(0, fCEnd);
    fHClone->GetXaxis()->SetTitle("ADC Channel");
    fHClone->GetYaxis()->SetTitle("Counts");
    
    // draw position line
    fPeakLine = aLine.DrawLine(peakPos, 0, peakPos, fHClone->GetMaximum());
    
    // ----------------------------- calculate and set results -----------------------------
    
    // calculate gain
    Double_t energyLoss = 0;
    
    // check detector type
    if (fDetID == kBaF2_Detector)            energyLoss = kTAPS_MIP_Loss_BaF2;
    else if (fDetID == kVeto_Detector)       energyLoss = kTAPS_MIP_Loss_Veto;
    else if (fDetID == kPbWO4_Detector)      energyLoss = kTAPS_MIP_Loss_PbWO4;
    else if (fDetID == kPbWO4_Veto_Detector) energyLoss = kTAPS_MIP_Loss_Veto;
    
    Double_t gain = energyLoss / (peakPos - pedPos); 
    
    // save results: pedestal position, cosmic peak position, gain
    SetResult(index, 0, pedPos);
    SetResult(index, 1, peakPos);
    SetResult(index, 2, gain);
}

//______________________________________________________________________________
void TMCalibCosmics::SaveResults(const Char_t* filename)
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
        fprintf(fout, "%3d  %7.3f  %7.3f  %8.6f\n", i+1, results[i][0], results[i][1], results[i][2]); 
    }
    
    // close the file
    fclose(fout);
    
    Info("SaveResults", "Saved results of module '%s' to file '%s'", GetName(), filename);
}
    
//______________________________________________________________________________
void TMCalibCosmics::Quit()
{
    // Save cosmics calibration results and quit module.
    
    // save gain distribution to ROOT file
    TFile f("Cosmics_Calibration_Results.root", "recreate");
    TH1F h("Gain distribution", "Gain distribution", 250, 0, 0.5);
    Double_t** results = GetResults();
    for (Int_t i = 0; i < GetCurrentDetectorSize(); i++) h.Fill(results[i][2]);
    h.Write(h.GetName());
    f.Close();
     
    // save results
    Save();
        
    // emit Finished() signal
    Finished();
}

//______________________________________________________________________________
void TMCalibCosmics::UpdateDetectorType(Int_t id)
{
    // Update the settings in the configuration dialog for the detector type the
    // user chose in the combo box
    
    if (id == kCosmics_Calib_Type_BAF2_LG)
    {
        fHNameEntry->SetText("BaF2_LG_%03d");
        fCEndEntry->SetText("1000");
    }
    else if (id == kCosmics_Calib_Type_BAF2_LGS)
    {
        fHNameEntry->SetText("BaF2_LGS_%03d");
        fCEndEntry->SetText("2000");
    }
    else if (id == kCosmics_Calib_Type_BAF2_SG)
    {
        fHNameEntry->SetText("BaF2_SG_%03d");
        fCEndEntry->SetText("1000");
    }
    else if (id == kCosmics_Calib_Type_BAF2_SGS)
    {
        fHNameEntry->SetText("BaF2_SGS_%03d");
        fCEndEntry->SetText("1000");
    }
    else if (id == kCosmics_Calib_Type_VETO)
    {
        fHNameEntry->SetText("Veto_%03d");
        fCEndEntry->SetText("800");
    }
    else if (id == kCosmics_Calib_Type_PWO)
    {
        fHNameEntry->SetText("PWO_%03d");
        fCEndEntry->SetText("1000");
    }
    else if (id == kCosmics_Calib_Type_PWO_S)
    {
        fHNameEntry->SetText("PWO_S_%03d");
        fCEndEntry->SetText("3500");
    }
    else if (id == kCosmics_Calib_Type_PWO_VETO)
    {
        fHNameEntry->SetText("PWO_Veto_%03d");
        fCEndEntry->SetText("800");
    }
    else if (id == kCosmics_Calib_Type_PWO_VETO_S)
    {
        fHNameEntry->SetText("PWO_Veto_S_%03d");
        fCEndEntry->SetText("1500");
    }
}

//______________________________________________________________________________
void TMCalibCosmics::ReadConfig()
{
    // Read the configuration made by the user in the config dialog.
    
    // configure fitting
    Int_t type = fTypeCombo->GetSelected();
    if (type == kCosmics_Calib_Type_BAF2_LG)
    {
        fDetID = kBaF2_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 500;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_BAF2_LGS)
    {
        fDetID = kBaF2_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 500;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_BAF2_SG)
    {
        fDetID = kBaF2_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 500;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_BAF2_SGS)
    {
        fDetID = kBaF2_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 500;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_VETO)
    {
        fDetID = kVeto_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 500;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_PWO)
    {
        fDetID = kPbWO4_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 300;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_PWO_S)
    {
        fDetID = kPbWO4_Detector;
        fPeakRange = 700;
        fPeakLimMin = 1200;
        fPeakLimMax = 1900;
        fPeakWidthFac = 100;
    }
    else if (type == kCosmics_Calib_Type_PWO_VETO)
    {
        fDetID = kPbWO4_Veto_Detector;
        fPeakRange = 200;
        fPeakLimMin = 100;
        fPeakLimMax = 300;
        fPeakWidthFac = 10;
    }
    else if (type == kCosmics_Calib_Type_PWO_VETO_S)
    {
        fDetID = kPbWO4_Veto_Detector;
        fPeakRange = 700;
        fPeakLimMin = 500;
        fPeakLimMax = 1300;
        fPeakWidthFac = 80;
    }

    // copy histogram name
    strcpy(fHName, fHNameEntry->GetText());
    
    // copy x-Axis range values
    fCStart = atof(fCStartEntry->GetText());
    fCEnd = atof(fCEndEntry->GetText());
}

