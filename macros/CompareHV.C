/*************************************************************************
 * Author: Dominik Werthmueller, 2017
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// CompareHV.C                                                          //
//                                                                      //
// Compare HV values.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


Char_t gTmp[256] = "";

//______________________________________________________________________________
Int_t ReadFile(const Char_t* file, Double_t* hv)
{
    // Read HV values from a file exported by TAPSMaintain.

    // try to open the file
    FILE* f = fopen(file, "r");
    if (!f)
    {
        Error("ReadFile", "File %s could not be opened!", file);
        return 0;
    }

    // read file
    Char_t* line[256];
    Int_t id, val;
    Int_t n = 0;
    while (fgets(line, 256, f))
    {
        // read id and HV value
        if (sscanf(line, "%d%d", &id, &val) == 2 && n == id-1)
        {
            hv[n] = val;
            n++;
        }
    }

    return n;
}

//______________________________________________________________________________
Char_t FormatDiff(Double_t diff)
{
    // Format the difference value (%) using terminal colors.

    // standard foreground color
    Int_t fg = 32;

    // check if the difference is larger than 1%
    if (TMath::Abs(diff) > 1) fg = 31;

    // format difference
    sprintf(gTmp, "%c[%d;%dm%4.1f%%%c[%dm", 0x1b, 0, fg, diff, 0x1b, 0);

    return gTmp;
}

//______________________________________________________________________________
void CompareHV(const Char_t* file1, const Char_t* file2)
{
    // Main method.

    Int_t nhv1, nhv2 = 0;
    Double_t hv1[384];
    Double_t hv2[384];

    // read HV values
    Int_t nhv1 = ReadFile(file1, hv1);
    Int_t nhv2 = ReadFile(file2, hv2);
    if (nhv1 != nhv2)
    {
        Error("CompareHV", "Numbers of values in the input files differ!");
        gSystem->Exit(1);
    }

    // loop over elements
    for (Int_t i = 0; i < nhv1; i++)
    {
        printf("Elem %3d  old: %4d  new: %4d  ", i+1, hv1[i], hv2[i]);
        Double_t diff = 100 * (hv2[i] - hv1[i]) / (Double_t)hv1[i];
        if (TMath::IsNaN(diff))
            printf("diff: -\n");
        else
            printf("diff: %s\n", FormatDiff(diff));
    }

    gSystem->Exit(0);
}

