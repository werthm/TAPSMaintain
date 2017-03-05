/*************************************************************************
 * Author: Dominik Werthmueller, 2017
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CalibOverview.C                                                      //
//                                                                      //
// Create overview plots from a cosmics calibration file.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
Int_t ReadFile(const Char_t* file, Double_t* ped, Double_t* peak, Double_t* gain)
{
    // Read values from a file exported by TAPSMaintain.

    // try to open the file
    FILE* f = fopen(file, "r");
    if (!f)
    {
        Error("ReadFile", "File %s could not be opened!", file);
        return 0;
    }

    // read file
    Char_t* line[256];
    Int_t id;
    Double_t val[3];
    Int_t n = 0;
    while (fgets(line, 256, f))
    {
        // read id and values
        if (sscanf(line, "%d%lf%lf%lf", &id, val, val+1, val+2) == 4 && n == id-1)
        {
            ped[n] = val[0];
            peak[n] = val[1];
            gain[n] = val[2];
            n++;
        }
    }

    return n;
}

//______________________________________________________________________________
void CalibOverview(const Char_t* file)
{
    // Main method.

    Int_t n = 0;
    Double_t ped[384];
    Double_t peak[384];
    Double_t gain[384];

    // read data
    Int_t n = ReadFile(file, ped, peak, gain);
    if (!n) gSystem->Exit(0);

    // create histograms
    gStyle->SetOptStat(0);
    TH1* hPed = new TH1F("Pedestals", "Pedestals", n, 0.5, n+0.5);
    TH1* hPeak = new TH1F("Peaks", "Peaks", n, 0.5, n+0.5);
    TH1* hGain = new TH1F("Gains", "Gains", n, 0.5, n+0.5);
    gStyle->SetOptStat(111111);
    TH1* hPedD = new TH1F("Pedestals_Dist", "Pedestal distribution", 160, 80, 120);
    TH1* hPeakD = new TH1F("Peaks_Dist", "Peak distribution", 300, 100, 400);
    TH1* hGainD = new TH1F("Gains_Dist", "Gain distribution", 250, 0, 0.5);

    // format histograms
    hPed->SetMarkerStyle(20);
    hPeak->SetMarkerStyle(20);
    hGain->SetMarkerStyle(20);
    hPed->SetMarkerSize(0.5);
    hPeak->SetMarkerSize(0.5);
    hGain->SetMarkerSize(0.5);
    hPedD->SetLineColor(kBlack);
    hPeakD->SetLineColor(kBlack);
    hGainD->SetLineColor(kBlack);
    hPed->GetXaxis()->SetTitle("Element");
    hPeak->GetXaxis()->SetTitle("Element");
    hGain->GetXaxis()->SetTitle("Element");
    hPed->GetYaxis()->SetTitle("Pedestal [Ch]");
    hPeak->GetYaxis()->SetTitle("Peak [Ch]");
    hGain->GetYaxis()->SetTitle("Gain [MeV/Ch]");
    hPedD->GetXaxis()->SetTitle("Pedestal [Ch]");
    hPeakD->GetXaxis()->SetTitle("Peak [Ch]");
    hGainD->GetXaxis()->SetTitle("Gain [MeV/Ch]");
    hPedD->GetYaxis()->SetTitle("Counts");
    hPeakD->GetYaxis()->SetTitle("Counts");
    hGainD->GetYaxis()->SetTitle("Counts");
    hPed->GetYaxis()->SetRangeUser(80, 120);
    hPeak->GetYaxis()->SetRangeUser(100, 400);
    hGain->GetYaxis()->SetRangeUser(0, 0.5);

    // loop over elements
    for (Int_t i = 0; i < n; i++)
    {
        // fill histograms
        hPed->SetBinContent(i+1, ped[i]);
        hPeak->SetBinContent(i+1, peak[i]);
        hGain->SetBinContent(i+1, gain[i]);
        hPedD->Fill(ped[i]);
        hPeakD->Fill(peak[i]);
        hGainD->Fill(gain[i]);
    }

    // create canvas
    TCanvas* c = new TCanvas("Calib_Overview", "Calibration Overview");
    c->Divide(3, 2, 0.001, 0.001);
    c->cd(1); hPed->Draw("p");
    c->cd(2); hPeak->Draw("p");
    c->cd(3); hGain->Draw("p");
    c->cd(4); hPedD->Draw();
    c->cd(5); hPeakD->Draw();
    c->cd(6); hGainD->Draw();
    c->Draw();

    // save histograms
    TFile* fout = new TFile("calib_overview.root", "create");
    if (!fout || fout->IsZombie())
    {
        Error("CalibOverview", "Could not open output file 'calib_overview.root'!");
        gSystem->Exit(1);
    }
    c->Write();
    gROOT->GetList()->Write();
    delete fout;

    gSystem->Exit(0);
}

