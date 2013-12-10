/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
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


enum EElement_Selection {
    kRange_Single_Element,
    kRange_All_Elements,
    kRange_All_PWO_Elements,
    kRange_All_PWO_VETO_Elements,
    kRange_Block_A,
    kRange_Block_B,
    kRange_Block_C,
    kRange_Block_D,
    kRange_Block_E,
    kRange_Block_F,
    kRange_Ring_1,
    kRange_Ring_2,
    kRange_Ring_3,
    kRange_Ring_4,
    kRange_Ring_5,
    kRange_Ring_6,
    kRange_Ring_7,
    kRange_Ring_8,
    kRange_Ring_9,
    kRange_Ring_10,
    kRange_Ring_11,
};


namespace TMUtils
{
    extern Char_t* fTmpCh;

    Int_t IndexOf(const Char_t* s1, const Char_t* s2);
    Char_t* Trim(const Char_t* s);
    Bool_t IsComment(const Char_t* s);
    
    UInt_t GetRingNumber(Int_t element);
    Char_t GetBlock(Int_t element);
}

#endif

