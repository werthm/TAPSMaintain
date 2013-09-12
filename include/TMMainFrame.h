// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMMainFrame                                                          //
//                                                                      //
// Main window of the TAPSMaintain application.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMMainFrame
#define TAPSMaintain_TMMainFrame

#include "TGToolBar.h"
#include "TGMenu.h"
#include "TGMsgBox.h"
#include "TG3DLine.h"
#include "TGFrame.h"
#include "TASImage.h"
#include "TGIcon.h"
#include "TColor.h"
#include "TGLabel.h"
#include "TGStatusBar.h"

#include "TMTAPSLogo.h"
#include "TMConfig.h"


enum TMMainFrameActionEvents {
    kACTION_OPEN_ROOT_FILE,
    kACTION_FILE_QUIT,
    kACTION_MODULE_STOP,
    kACTION_HELP_ABOUT
};


class TMMainFrame : public TGMainFrame
{
    
private:
    TGCompositeFrame* fModuleFrame;             // Frame for modules
    TGCompositeFrame* fWelcomeFrame;            // Placeholder welcome frame
    TGToolBar* fToolBar;                        // Tool bar
    TASImage* fTAPSLogo;                        // TAPS logo bitmap
    TGIcon* fTAPSIcon;                          // TAPS logo icon
    
    TGMenuBar* fMenuBar;                        // Menu bar
    TGPopupMenu* fFileMenu;                     // "File" menu
    TGPopupMenu* fModulesMenu;                  // "Modules" menu
    TGPopupMenu* fHelpMenu;                     // "Help" menu
    TGLayoutHints* fLayout;                     // menu layout
    TGStatusBar* fStatusBar;                    // status bar
    
public:
    TMMainFrame();
    TMMainFrame(const TGWindow* p, void* menuHandler);
    virtual ~TMMainFrame();
    
    void LoadModuleFrame(TGCompositeFrame* mf);
    void UnloadModuleFrame(TGCompositeFrame* mf);
    void LoadWelcomeFrame();
    void UnloadWelcomeFrame();

    void SetStatusBar(const Char_t* text, Int_t part) { fStatusBar->SetText(text, part); }
    TGPopupMenu* GetModulesMenu() const { return fModulesMenu; }
    void Show();
    
    ClassDef(TMMainFrame, 0) // TAPSMaintain main window
};

#endif

