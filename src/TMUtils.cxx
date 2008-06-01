// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMUtils                                                              //
//                                                                      //
// This namespace contains some utility functions used by TAPSMaintain. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMUtils.h"

#if !defined(R__ALPHA) && !defined(R__SOLARIS) && !defined(R__ACC) && !defined(R__FBSD)
NamespaceImp(TMUtils)
#endif


// Declare namespace member
Char_t* TMUtils::fTmpCh = 0;


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

