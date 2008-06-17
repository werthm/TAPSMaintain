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


#ifndef TAPSMaintain_TMUtils
#define TAPSMaintain_TMUtils

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include <cstring>


namespace TMUtils
{
    extern Char_t* fTmpCh;

    Char_t* Trim(const Char_t* s);
    Bool_t IsComment(const Char_t* s);
    
    UInt_t GetRingNumber(Int_t element);
}

#endif

