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
    : TMSeqCalibModule(name, id, 1, kTRUE)
{
    // Constructor.
    
    fLevel = 0.9;
    hClone = 0;
}

//______________________________________________________________________________
TMCalibLED::~TMCalibLED()
{
    // Destructor.

    if (hClone) delete hClone;
}

//______________________________________________________________________________
void TMCalibLED::Init()
{
    // (Re-)initalize the module.
    
    // Clear results array
    ClearResults();
    
    // Start processing first element
    ProcessElement(0);
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
    sprintf(name, "LG/baf2-LG-%03d", index + 1);
    h1 = (TH1F*) fFile->Get(name);

    // load LED spectrum
    sprintf(name, "LG_LED1/baf2-LG_LED1-%03d", index + 1);
    h2 = (TH1F*) fFile->Get(name);
    
    // clone spectrum
    if (hClone) delete hClone;
    hClone = (TH1F*) h2->Clone();
    
    // divide LED spectrum
    hClone->Divide(h1);
    
    // find LED threshold position
    threshold = FindThreshold(hClone);
    
    // save result
    SetResult(index, 0, threshold);
    
    // draw spectrum
    hClone->Draw();
    hClone->GetXaxis()->SetRangeUser(0, 60);
    hClone->GetYaxis()->SetRangeUser(0, 1.1);
    hClone->GetXaxis()->SetTitle("ADC Channel");
    hClone->GetYaxis()->SetTitle("LED divided by raw");
    
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
    aLine.DrawLine(0, fLevel, 60, fLevel);

    // mark threshold position
    aLine.SetLineStyle(1);
    aLine.SetLineWidth(2);
    aLine.SetLineColor(kRed);
    aLine.DrawLine(threshold, 0, threshold, 1);
}

//______________________________________________________________________________
void TMCalibLED::Quit()
{
    // Save LED calibration results and quit module.
    
    // dump results
    DumpResults("%6.2f");
    
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
     
        if (pos < 0) continue;

        if (content > fLevel) return (pos + posUnder) / 2.;   
        else posUnder = pos;
    }
    
    // return 0 if position was not found
    return 0;
}

