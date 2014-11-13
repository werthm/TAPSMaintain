/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2014
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMTAPSMaintain                                                       //
//                                                                      //
// TAPSMaintain main class containing the main window and the module    //
// loader.                                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMTAPSMaintain
#define TAPSMaintain_TMTAPSMaintain

#include "RQ_OBJECT.h"
#include "TObject.h"
#include "TGMsgBox.h"

class TFile;
class TMModuleLoader;
class TMMainFrame;
class TMModule;

class TMTAPSMaintain : public TObject
{
    RQ_OBJECT("TMTAPSMaintain")

private:
    TMModuleLoader* fModuleLoader;                  // Module loader
    TMMainFrame* fMainWindow;                       // TAPSMaintain main window
    TMModule* fActiveModule;                        // currently active module
    TFile* fFile;                                   // ROOT input file
    Char_t fCurrentDir[256];                        // current directory on file system
    
public:
    TMTAPSMaintain();
    virtual ~TMTAPSMaintain();
    
    Int_t Start();
    void Quit();
    void HandleMenu(Int_t id);
    
    void StartModule(TMModule* mod);
    void StopModule(Bool_t forced = kFALSE);
    void ShowModuleError(const Char_t* msg) const { ShowModuleMessage(msg, kMBIconStop); }
    void ShowModuleInfo(const Char_t* msg) const { ShowModuleMessage(msg, kMBIconAsterisk); }
    void ShowModuleQuestion(const Char_t* question) const;
    void ShowModuleMessage(const Char_t* msg, EMsgBoxIcon icon = kMBIconAsterisk) const;
    void ShowFileDialogForModule(EFileDialogMode type);
    void SaveModuleResults();
    
    void SetModuleConfigAndStart();
    void AbortModuleConfig();
    Bool_t OpenInputFile(const char* inFile);
    void SelectAndOpenFile();

    ClassDef(TMTAPSMaintain, 0) // TAPSMaintain main class
};

#endif

