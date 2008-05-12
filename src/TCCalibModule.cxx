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


#include "TMModule.h"

ClassImp(TMModule)


//______________________________________________________________________________
TMModule::TMModule()
    : TNamed()
{
    // Default constructor.
 
    fID = 0;
    fNresults = 0;
    fResults = 0;
    fFrame = 0;
}

//______________________________________________________________________________
TMModule::TMModule(const Char_t* name, UInt_t id, UInt_t inNresults)
    : TNamed(name, name)
{
    // Constructor.
    
    fID = id;
    fNresults = inNresults;

    // create result array
    fResults = new Double_t*[gTAPSSize];
    for (UInt_t i = 0; i < gTAPSSize; i++) fResults[i] = new Double_t[inNresults];
    
    // create frame
    fFrame = new TGCompositeFrame(0, 800, 600, kVerticalFrame);
    
    // add module title
    Char_t title[256];
    sprintf(title, "Module: %s", GetName());
    TGLabel* l = new TGLabel(fFrame, title);
    l->SetTextJustify(kTextLeft);
    l->SetTextFont("-*-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1");
    fFrame->AddFrame(l, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 10, 10));
}

//______________________________________________________________________________
TMModule::~TMModule()
{
    // Destructor.

    // delete results array
    if (fResults) {
        for (UInt_t i = 0; i < gTAPSSize; i++) delete [] fResults[i];
    }
    
    // delete frame
    if (fFrame) delete fFrame;
}

