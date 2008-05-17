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
// To-do-list for creating a new module based on TMModule:              //
//                                                                      //
// CONSTRUCTOR                                                          //
// Set the following parameters to the needs of your module:            //
// name              : the name of your module                          //
// id                : unique module ID                                 //
// inNresults        : number of results per TAPS element this module   //
//                     will create                                      //
// needsROOTInputFile: set to kTRUE if the module needs a loaded ROOT   //
//                     file before it can be started                    //
// needsConfig       : set to kTRUE if the module needs a configuration //
//                     dialog that is displayed before the module is    //
//                     started                                          //
//                                                                      //
//                                                                      //
// PURE VIRTUAL METHODS                                                 //
// The following methods are pure virtual and have to be implemented    //
// by any subclass of TMModule:                                         //
//                                                                      //
// void Init()       : will be called each time before the module is    //
//                     displayed                                        //
//                                                                      //
// void ReadConfig() : will be called each time after the configuration //
//                     dialog was displayed                             //
//                                                                      //
//                                                                      //
// CONFIGURATION DIALOG                                                 //
// A module can have a configuration dialog that is displayed before    //
// every execution of the module. To add such a dialog the needsConfig  //
// parameter in the constructor has to be set to kTRUE. In addition the //
// GUI content of this dialog has to be added to the frame fConfigFrame //
// in the module constructor. The configuration made by the user can    //
// then be read out in the ReadConfig() method that is called right     //
// after the dialog unloading before the module execution.              //
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
    fNeedsConfig = kFALSE;
    fFile = 0;
    fNresults = 0;
    fResults = 0;
    fFrame = 0;
    fConfigDialog = 0;
    fConfigFrame = 0;
    fOk = 0;
}

//______________________________________________________________________________
TMModule::TMModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile, Bool_t needsConfig)
    : TNamed(name, name)
{
    // Constructor.
    
    fID = id;
    fNeedsTFile  = needsROOTInputFile;
    fNeedsConfig = needsConfig;
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
   
    fConfigDialog = 0;
    fOk = 0;

    // create config frame so subclasses can add their stuff to it
    if (fNeedsConfig) fConfigFrame = new TGCompositeFrame(0, 400, 300, kVerticalFrame);
    else fConfigFrame = 0;
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

    // delete dialog
    if (fConfigDialog) delete fConfigDialog;
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
    // Dump the results array to stdout. Use numberFormat to format the array
    // content.

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

//______________________________________________________________________________
void TMModule::CreateConfigDialog(TGWindow* main)
{
    // Create the config dialog of this module and attach it to the main
    // window 'main'.

    Char_t name[256];

    // create dialog window
    if (!fConfigDialog) fConfigDialog = new TGTransientFrame(0, main, 400, 300);
    fConfigDialog->DontCallClose();
    sprintf(name, "Configuration for module '%s'", GetName());
    fConfigDialog->SetWindowName(name);

    // add individual configuration frame
    fConfigFrame->ReparentWindow(fConfigDialog);
    fConfigDialog->AddFrame(fConfigFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 10));

    // add Ok button
    if (!fOk) fOk = new TGTextButton(fConfigDialog, "    Ok    ");
    fConfigDialog->AddFrame(fOk, new TGLayoutHints(kLHintsBottom | kLHintsCenterX, 5, 5, 5, 5));
}

