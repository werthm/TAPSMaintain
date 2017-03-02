/*************************************************************************
 * Author: Dominik Werthmueller, 2011-2017
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMCalibTDC                                                           //
//                                                                      //
// Module for the TDC gain calibration.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TF1.h"
#include "TH1.h"
#include "TLine.h"
#include "TGraphErrors.h"
#include "TGTableLayout.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TGLabel.h"
#include "TSystem.h"
#include "TFile.h"
#include "TMath.h"

#include "TMCalibTDC.h"

ClassImp(TMCalibTDC)

//______________________________________________________________________________
TMCalibTDC::TMCalibTDC(const Char_t* name, UInt_t id)
    : TMSeqCalibModule(name, id, 1, kFALSE, kTRUE)
{
    // Constructor.

    Char_t tmp[256];

    // set module result header
    SetResultHeader("# Results of the TDC Calibration module\n"
                    "# Format: element id, TDC gain\n");

    // create configuration dialog
    fConfigFrame->SetLayoutManager(new TGTableLayout(fConfigFrame, 5, 2));

    // create members
    fNfit = 7;
    fFiles = new TFile*[fNfit];
    fPeakFunc = new TF1*[fNfit];
    fLinFunc = new TF1("LinFunc", "pol1");
    fLinFunc->SetLineColor(kBlue);
    fPeakLine = new TLine*[fNfit];
    for (Int_t i = 0; i < fNfit; i++)
    {
        // create functions
        sprintf(tmp, "PeakFunc_%d", i);
        fPeakFunc[i] = new TF1(tmp, "gaus");
        fPeakFunc[i]->SetLineColor(kRed);

        // create lines
        fPeakLine[i] = new TLine();
        fPeakLine[i]->SetLineColor(kBlue);
        fPeakLine[i]->SetLineWidth(2);
    }

    // graph
    fGraph = new TGraphErrors(fNfit);
    fGraph->SetMarkerStyle(20);

    // Detector type
    TGLabel* l = new TGLabel(fConfigFrame, "Detector Type:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fTypeCombo = new TGComboBox(fConfigFrame);
    fTypeCombo->Connect("Selected(Int_t)", "TMCalibTDC", this, "UpdateDetectorType(Int_t)");
    fTypeCombo->Resize(200, 22);
    fTypeCombo->AddEntry("BaF2", kTDC_Calib_Type_BAF2);
    fTypeCombo->AddEntry("Veto", kTDC_Calib_Type_VETO);
    fConfigFrame->AddFrame(fTypeCombo, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));

    // ADC histogram name
    l = new TGLabel(fConfigFrame, "Histogram name:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fHNameEntry = new TGTextEntry(fConfigFrame);
    fConfigFrame->AddFrame(fHNameEntry, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsRight, 5, 5, 2, 2));

    // location
    l = new TGLabel(fConfigFrame, "Location of ROOT files:");
    l->SetTextJustify(kTextRight);
    fConfigFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    fLocation = new TGTextEntry(fConfigFrame);
    fLocation->SetText(gSystem->WorkingDirectory());
    fConfigFrame->AddFrame(fLocation, new TGTableLayoutHints(1, 2, 2, 3, kLHintsLeft | kLHintsFillX, 5, 5, 2, 2));

    // Batch mode
    fBatchMode = new TGCheckButton(fConfigFrame, "Batch mode");
    fConfigFrame->AddFrame(fBatchMode, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsRight, 5, 5, 5, 5));

    // needs to be called in every TMModule that uses a configuration dialog
    fConfigFrame->Layout();

    // select detector type
    fTypeCombo->Select(kTDC_Calib_Type_BAF2, kTRUE);

    // divide canvas
    fCanvas->Divide(4, 2);
}

//______________________________________________________________________________
TMCalibTDC::~TMCalibTDC()
{
    // Destructor.

    if (fPeakFunc)
    {
        for (Int_t i = 0; i < fNfit; i++) delete fPeakFunc[i];
        delete [] fPeakFunc;
    }
    if (fLinFunc) delete fLinFunc;
    if (fPeakLine)
    {
        for (Int_t i = 0; i < fNfit; i++) delete fPeakLine[i];
        delete [] fPeakLine;
    }
    if (fGraph) delete fGraph;
}

//______________________________________________________________________________
void TMCalibTDC::Init()
{
    // (Re-)initalize the module.

    Char_t name[256];

    // Clear results array
    ClearResults();

    // load ROOT files
    const Char_t* fname[7] = { "none.root", "0.root", "01.root", "012.root",
                               "0123.root", "01234.root", "012345.root" };
    for (Int_t i = 0; i < fNfit; i++)
    {
        // try to open ROOT files
        sprintf(name, "%s/%s", fPath, fname[i]);
        fFiles[i] = new TFile(name);

        // check state
        if (!fFiles[i])
        {
            sprintf(name, "Could not open ROOT file '%s'!", fname[i]);
            ModuleError(name);
            Finished();
            return;
        }
        if (fFiles[i]->IsZombie())
        {
            sprintf(name, "Could not open ROOT file '%s'!", fname[i]);
            ModuleError(name);
            Finished();
            return;
        }
    }

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
void TMCalibTDC::Cleanup()
{
    // Cleanup memory.

}

//______________________________________________________________________________
void TMCalibTDC::Redo()
{
    // Take the position of the line that was moved manually by the user as
    // the position of the cosmics peak.

    ModuleError("Not implemented in this module!");
}

//______________________________________________________________________________
void TMCalibTDC::Process(Int_t index, Bool_t redo)
{
    // Fit cosmics spectrum of the element 'index'.

    Char_t name[256];
    Char_t title[256];
    TH1* h[fNfit];
    const Char_t* tname[7] = { "no cable", "Cable 0", "Cables 0,1", "Cables 0,1,2",
                               "Cables 0,1,2,3", "Cables 0,1,2,3,4", "Cables 0,1,2,3,4,5" };

    // load raw spectra
    sprintf(name, fHName, index + 1);
    for (Int_t i = 0; i < fNfit; i++)
    {
        h[i] = (TH1*) fFiles[i]->Get(name);

        // check if spectra could be loaded
        if (!h[i])
        {
            sprintf(name, "Spectrum '%s' was not found in ROOT file!", fHName);
            ModuleError(name);
            Finished();
            return;
        }

        // set title
        sprintf(title, "Elem: %d: %s", index+1, tname[i]);
        h[i]->SetTitle(title);
    }

    // loop over measurements
    Double_t pos0 = 0;
    Double_t pos0_e = 0;
    for (Int_t i = 0; i < fNfit; i++)
    {
        // fit pulser
        Double_t pos = h[i]->GetXaxis()->GetBinCenter(h[i]->GetMaximumBin());
        fPeakFunc[i]->SetParameters(h[i]->GetMaximum(), pos, 0.55);
        fPeakFunc[i]->SetParLimits(2, 0.5, 5);
        fPeakFunc[i]->SetRange(pos - 20, pos + 20);
        h[i]->Fit(fPeakFunc[i], "RB0Q");
        pos = fPeakFunc[i]->GetParameter(1);
        Double_t pos_e = fPeakFunc[i]->GetParError(1);
        Double_t sigma = fPeakFunc[i]->GetParameter(2);
        if (i == 0)
        {
            pos0 = pos;
            pos0_e = pos_e;
        }

        // draw histo and function
        fCanvas->cd(i+1);
        h[i]->Draw();
        fPeakFunc[i]->Draw("same");
        h[i]->GetXaxis()->SetRangeUser(pos - 5*sigma, pos + 5*sigma);
        h[i]->GetXaxis()->SetTitle("TDC Channel");
        h[i]->GetYaxis()->SetTitle("Counts");

        // draw position line
        fPeakLine[i]->SetX1(pos);
        fPeakLine[i]->SetX2(pos);
        fPeakLine[i]->SetY1(0);
        fPeakLine[i]->SetY2(h[i]->GetMaximum());
        fPeakLine[i]->Draw("same");

        // calculate channel shift
        Double_t tdc_shift = pos - pos0;
        Double_t tdc_shift_e = TMath::Sqrt(pos_e*pos_e + pos0_e*pos0_e);

        // set graph point
        fGraph->SetPoint(i, tdc_shift, kTDC_Cable_Delay[i]);
        fGraph->SetPointError(i, tdc_shift_e, kTDC_Cable_Delay_Error[i]);
    }

    // draw graph
    fCanvas->cd(8);
    fGraph->GetXaxis()->SetTitle("TDC Channel Shift [Channels]");
    fGraph->GetYaxis()->SetTitle("Delay [ns]");
    fGraph->Draw("ap");

    // fit graph
    fLinFunc->SetRange((fGraph->GetX())[0], (fGraph->GetX())[fNfit-1]);
    fGraph->Fit(fLinFunc, "RB0Q");
    fLinFunc->Draw("same");

    // get result
    Double_t gain = fLinFunc->GetParameter(1);

    // save results: TDC gain
    SetResult(index, 0, gain);
}

//______________________________________________________________________________
void TMCalibTDC::SaveResults(const Char_t* filename)
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
        fprintf(fout, "%3d  %1.6f\n", i+1, results[i][0]);
    }

    // close the file
    fclose(fout);

    Info("SaveResults", "Saved results of module '%s' to file '%s'", GetName(), filename);
}

//______________________________________________________________________________
void TMCalibTDC::Quit()
{
    // Save cosmics calibration results and quit module.

    // save results
    Save();

    // emit Finished() signal
    Finished();
}

//______________________________________________________________________________
void TMCalibTDC::UpdateDetectorType(Int_t id)
{
    // Update the settings in the configuration dialog for the detector type the
    // user chose in the combo box

    if (id == kTDC_Calib_Type_BAF2)
    {
        fHNameEntry->SetText("BaF2_Time_%03d");
    }
    if (id == kTDC_Calib_Type_VETO)
    {
        fHNameEntry->SetText("Veto_Time_%03d");
    }
}

//______________________________________________________________________________
void TMCalibTDC::ReadConfig()
{
    // Read the configuration made by the user in the config dialog.

    // set detector id
    Int_t type = fTypeCombo->GetSelected();
    if (type == kTDC_Calib_Type_BAF2)      fDetID = kBaF2_Detector;
    else if (type == kTDC_Calib_Type_VETO) fDetID = kVeto_Detector;

    // copy histogram name
    strcpy(fHName, fHNameEntry->GetText());

    // copy file path
    strcpy(fPath, fLocation->GetText());
}

