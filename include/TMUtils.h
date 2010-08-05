// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
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
    ERange_Single_Element,
    ERange_All_Elements,
    ERange_All_PWO_Elements,
    ERange_All_PWO_VETO_Elements,
    ERange_Block_A,
    ERange_Block_B,
    ERange_Block_C,
    ERange_Block_D,
    ERange_Block_E,
    ERange_Block_F,
    ERange_Ring_1,
    ERange_Ring_2,
    ERange_Ring_3,
    ERange_Ring_4,
    ERange_Ring_5,
    ERange_Ring_6,
    ERange_Ring_7,
    ERange_Ring_8,
    ERange_Ring_9,
    ERange_Ring_10,
    ERange_Ring_11,
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

