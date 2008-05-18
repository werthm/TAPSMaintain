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
//                                                                      //
// RESULT HEADER                                                        //
// Every module should have a result header that is saved along with    //
// the results to file. Set this header via the SetResultHeader() meth- //
// od.                                                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMModule
#define TAPSMaintain_TMModule

#include "TNamed.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGWidget.h"
#include "TFile.h"
#include "TGButton.h"

#include "TMConfig.h"


class TMModule : public TNamed, public TQObject
{

private:
    UInt_t fID;                                 // Module ID
    Bool_t fNeedsTFile;                         // Flag if module needs ROOT input file or not
    Bool_t fNeedsConfig;                        // Flag if module needs a configuration dialog
    UInt_t fNresults;                           // Number of results per element
    Double_t** fResults;                        // Result array of this module
    Char_t fResultHeader[256];                  // Header for the result description when saving to file

protected:    
    TFile* fFile;                               // ROOT input file
    TGCompositeFrame* fFrame;                   // Module GUI frame
    TGTransientFrame* fConfigDialog;            // Configuration dialog frame of this module
    TGCompositeFrame* fConfigFrame;             // Individual module configuration frame
    TGTextButton* fOk;                          // OK button of the configuration dialog

public:
    TMModule();
    TMModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile, Bool_t needsConfig);
    virtual ~TMModule();
    
    UInt_t GetID() const { return fID; }
    Bool_t NeedsRootInputFile() const { return fNeedsTFile; }
    Bool_t NeedsConfig() const { return fNeedsConfig; }
    UInt_t GetNresults() const { return fNresults; }
    Double_t** GetResults() const { return fResults; }
    TGCompositeFrame* GetFrame() const { return fFrame; }
    TGTransientFrame* GetConfigDialog() const { return fConfigDialog; }
    TGButton* GetConfigDialogCloseButton() const { return fOk; }

    void SetResultHeader(const Char_t* h) { strncpy(fResultHeader, h, 256); }
    void SetResult(UInt_t element, UInt_t resultNumber, Double_t result);
    void SetRootInputFile(TFile* f) { fFile = f; }
    
    void ClearResults();
    void DumpResults(const Char_t* numberFormat = "%9.7e") const;
    void SaveResults(const Char_t* filename) const;
    void CreateConfigDialog(TGWindow* main);
    
    void ModuleError(const Char_t* msg) { Emit("ModuleError(const Char_t*)", msg); }    // *SIGNAL*  
    void Save() { Emit("Save()"); }                                                     // *SIGNAL* 
    void Finished() { Emit("Finished()"); }                                             // *SIGNAL*  
    
    virtual void Init() = 0;                        // (Re-)initalize the module
    virtual void ReadConfig() = 0;                  // Read configuration made in the config dialog 

    ClassDef(TMModule, 0) // Abstract module class
};

#endif

