/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2016
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMUtils                                                              //
//                                                                      //
// This namespace contains some utility functions used by TAPSMaintain. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TH1.h"

#include "TMUtils.h"

#if !defined(R__ALPHA) && !defined(R__SOLARIS) && !defined(R__ACC) && !defined(R__FBSD)
NamespaceImp(TMUtils)
#endif

// Declare namespace member
Char_t* TMUtils::fTmpCh = 0;

//______________________________________________________________________________
Int_t TMUtils::IndexOf(const Char_t* s1, const Char_t* s2)
{
    // Returns the position of the first occurrence of the string s2
    // in the string s1. Returns -1 if s2 was not found.

    const Char_t* pos = strstr(s1, s2);
    if (pos) return pos-s1;
    else return -1;
}

//______________________________________________________________________________
Char_t* TMUtils::Trim(const Char_t* s)
{
    // Removes the leading and trailing whitespace of the string s.

    Int_t l = strlen(s);
    Int_t posF = -1;
    Int_t posL = -1;

    // Create output string if empty
    if (!fTmpCh) fTmpCh = new Char_t[256];

    // Search first non-whitespace
    for (Int_t i = 0; i < l; i++)
    {
        // leave loop when first non-whitespace was found
        if (s[i] != ' ')
        {
            posF = i;
            break;
        }
    }

    // handle empty string
    if (posF == -1)
    {
        fTmpCh[0] = '\0';
        return fTmpCh;
    }

    // Search last non-whitespace
    for (Int_t i = l-1; i >= 0; i--)
    {
        // leave loop when last non-whitespace was found
        if (s[i] != ' ')
        {
            posL = i;
            break;
        }
    }

    // Copy trimmed string
    Int_t j = 0;
    for (Int_t i = posF; i <= posL; i++)
    {
        fTmpCh[j++] = s[i];
    }
    fTmpCh[j] = '\0';

    return fTmpCh;
}

//______________________________________________________________________________
Bool_t TMUtils::IsComment(const Char_t* s)
{
    // Returns kTRUE if the string s is a comment, i.e. starts with a '#'

    // Trim string
    Char_t* st = Trim(s);

    // First character is a '#'
    if (st[0] == '#') return kTRUE;
    else return kFALSE;
}

//______________________________________________________________________________
UInt_t TMUtils::GetRingNumber(Int_t element)
{
    // Returns the number of the ring (1 to 11 for 384 BaF2 layout) the element
    // 'elem' belongs to (element counting starts at 0).

    // element layout (first and last elements of a ring)
    UInt_t ringRange[11][2] = {{0, 0}, {1, 2}, {3, 5}, {6, 9}, {10, 14}, {15, 20},
                              {21, 27}, {28, 35}, {36, 44}, {45, 54}, {55, 63}};

    // map element to first block
    UInt_t mid = element % 64;

    if (mid >= ringRange[0][0] && mid <= ringRange[0][1]) return 1;
    else if (mid >= ringRange[1][0] && mid <= ringRange[1][1]) return 2;
    else if (mid >= ringRange[2][0] && mid <= ringRange[2][1]) return 3;
    else if (mid >= ringRange[3][0] && mid <= ringRange[3][1]) return 4;
    else if (mid >= ringRange[4][0] && mid <= ringRange[4][1]) return 5;
    else if (mid >= ringRange[5][0] && mid <= ringRange[5][1]) return 6;
    else if (mid >= ringRange[6][0] && mid <= ringRange[6][1]) return 7;
    else if (mid >= ringRange[7][0] && mid <= ringRange[7][1]) return 8;
    else if (mid >= ringRange[8][0] && mid <= ringRange[8][1]) return 9;
    else if (mid >= ringRange[9][0] && mid <= ringRange[9][1]) return 10;
    else if (mid >= ringRange[10][0] && mid <= ringRange[10][1]) return 11;
    else return 99;
}

//______________________________________________________________________________
Char_t TMUtils::GetBlock(Int_t element)
{
    // Returns the block (A-F) the element 'elem' belongs to (element counting
    // starts at 0).

    Int_t block = element / 64;
    switch (block)
    {
        case 0: return 'A';
        case 1: return 'B';
        case 2: return 'C';
        case 3: return 'D';
        case 4: return 'E';
        case 5: return 'F';
        default: return 'X';
    }
}

//______________________________________________________________________________
TH1* TMUtils::DeriveHistogram(TH1* inH)
{
    // Return the derivative of the histogram 'inH'.

    Char_t tmp[256];

    // get the number of bins
    Int_t nBins = inH->GetNbinsX();

    // create new histogram
    sprintf(tmp, "%s_Derivative", inH->GetName());
    TH1* h = new TH1D(tmp, tmp, nBins, inH->GetXaxis()->GetXmin(), inH->GetXaxis()->GetXmax());

    // loop over bins
    for (Int_t i = 1; i <= nBins-1; i++)
    {
        // x-difference
        Double_t xdiff = inH->GetBinCenter(i+1) - inH->GetBinCenter(i);

        // y-difference
        Double_t ydiff = inH->GetBinContent(i+1) - inH->GetBinContent(i);

        // fill derived histogram
        h->SetBinContent(i, ydiff / xdiff);
    }

    return h;
}

//______________________________________________________________________________
void TMUtils::ZeroBins(TH1* inH, Double_t th)
{
    // Set all bins of the histogram 'inH' that are lower than the value 'th' to zero.

    // get number of bins
    Int_t nbinsX = inH->GetNbinsX();
    Int_t nbinsY = inH->GetNbinsY();
    Int_t nbinsZ = inH->GetNbinsZ();

    // loop over bins
    // z bins
    for (Int_t i = 0; i <= nbinsZ+1; i++)
    {
        // y bins
        for (Int_t j = 0; j <= nbinsY+1; j++)
        {
            // x bins
            for (Int_t k = 0; k <= nbinsX+1; k++)
            {
                // set content to zero if negative
                if (inH->GetBinContent(k, j, i) < th) inH->SetBinContent(k, j, i, 0);
            }
        }
    }
}

