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
    fNeedsTFile = kFALSE;
    fFile = 0;
    fNresults = 0;
    fResults = 0;
    fFrame = 0;
}

//______________________________________________________________________________
TMModule::TMModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile)
    : TNamed(name, name)
{
    // Constructor.
    
    fID = id;
    fNeedsTFile = needsROOTInputFile;
    fFile = 0;
    fNresults = inNresults;

    // create result array
    fResults = new Double_t*[gTAPSSize];
    for (UInt_t i = 0; i < gTAPSSize; i++) fResults[i] = new Double_t[fNresults];
    
    // create frame
    fFrame = new TGCompositeFrame(0, 800, 600, kVerticalFrame);
    
    // add module title
    //Char_t title[256];
    //sprintf(title, "Module: %s", GetName());
    //TGLabel* l = new TGLabel(fFrame, title);
    //l->SetTextJustify(kTextLeft);
    //l->SetTextFont("-*-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1");
    //fFrame->AddFrame(l, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 0, 0));
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

//______________________________________________________________________________
void TMModule::SetResult(UInt_t element, UInt_t resultNumber, Double_t result)
{
    // Set the result #resultNumber of the element 'element' to 'result'.
    
    // check bounds
    if (element < 0 || element >= gTAPSSize) return;
    if (resultNumber < 0 || resultNumber >= fNresults) return;
    
    // set result
    fResults[element][resultNumber] = result;
}

//______________________________________________________________________________
void TMModule::ClearResults()
{
    // Clear the results array.
    
    for (UInt_t i = 0; i < gTAPSSize; i++)
    {
        for (UInt_t j = 0; j < fNresults; j++)
        {
            fResults[i][j] = 0.0;
        }
    }
}

//______________________________________________________________________________
void TMModule::DumpResults(const Char_t* numberFormat)
{
    // Dump the results array to stdout.

    Char_t format[256];
    sprintf(format, " %s ", numberFormat);
    
    printf("\nDumping result array of module '%s'\n\n", GetName());
    
    // print array content
    for (UInt_t i = 0; i < gTAPSSize; i++)
    {
        printf(" %3d ", i);
        for (UInt_t j = 0; j < fNresults; j++)
        {
            printf(numberFormat, fResults[i][j]);
        }
        printf("\n");
    }
    
    printf("\nEnd of dump.\n");
}

