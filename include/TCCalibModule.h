// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMModule                                                             //
//                                                                      //
// Abstract class representing a module.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMModule
#define TAPSMaintain_TMModule

#include "TNamed.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGWidget.h"

#include "TMConfig.h"


class TMModule : public TNamed
{

protected:
    UInt_t fID;                                 // Module ID
    
    UInt_t fNresults;                           // Number of results per element
    Double_t** fResults;                        // Result array of this module
    
    TGCompositeFrame* fFrame;                   // Module GUI frame

public:
    TMModule();
    TMModule(const Char_t* name, UInt_t id, UInt_t inNresults);
    virtual ~TMModule();
    
    UInt_t GetID() { return fID; }
    UInt_t GetNresults() { return fNresults; }
    Double_t** GetResults() { return fResults; }
    
    TGCompositeFrame* GetFrame() { return fFrame; }
    
    virtual void Start() = 0;

    ClassDef(TMModule, 0) // Abstract module class
};

#endif

