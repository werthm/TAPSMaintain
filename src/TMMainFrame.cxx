// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMMainFrame                                                          //
//                                                                      //
// Main window of the TAPSMaintain application.                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMMainFrame.h"

ClassImp(TMMainFrame)


//______________________________________________________________________________
TMMainFrame::TMMainFrame()
    : TGMainFrame()
{
    // Default constructor.
    
    fModuleFrame  = 0;
    fWelcomeFrame = 0;
    fToolBar      = 0;
    fTAPSLogo     = 0;
    fTAPSIcon     = 0;
    
    fMenuBar      = 0;
    fFileMenu     = 0;
    fModulesMenu  = 0;
    fHelpMenu     = 0;
    fLayout       = 0;
}

//______________________________________________________________________________
TMMainFrame::TMMainFrame(const TGWindow* p = 0, void* menuHandler = 0)
    : TGMainFrame(p, 0, 0)
{
    // Constructor.
    
    // create the main frame
    Connect("CloseWindow()", "TMTAPSMaintain", menuHandler, "Quit()");
    DontCallClose();
    SetWindowName("TAPSMaintain");
    
    
    // ------------------------------ TAPS Logo and icon ------------------------------
    fTAPSLogo = new TASImage(kTAPSLogo_width, kTAPSLogo_height);

    Char_t pix[3];
    Char_t pixel[256];
    
    // loop over pixel data
    for (UInt_t i = 0; i < kTAPSLogo_height; i++)
    {
        for (UInt_t j = 0; j < kTAPSLogo_width; j++)
        {
            // read pixel data
            HEADER_PIXEL(kTAPSLogo_data, pix);
            
            // convert to hex format
            sprintf(pixel, "#%02x%02x%02x", (UChar_t)pix[0], (UChar_t)pix[1], (UChar_t)pix[2]);
            
            // put pixel
            fTAPSLogo->PutPixel(j, i, pixel);
        }
    }
    
    
    // ------------------------------ Menu bar ------------------------------    
    
    fMenuBar = new TGMenuBar(this, 100, 20, kHorizontalFrame);
    
    // create the "File" menu
    fFileMenu = new TGPopupMenu(gClient->GetRoot());
    fFileMenu->AddEntry("&Open ROOT file", kACTION_OPEN_ROOT_FILE);
    fFileMenu->AddSeparator();
    fFileMenu->AddEntry("&Quit", kACTION_FILE_QUIT);
    fFileMenu->Connect("Activated(Int_t)", "TMTAPSMaintain", menuHandler, "HandleMenu(Int_t)");

    // create the "Modules" menu
    fModulesMenu = new TGPopupMenu(gClient->GetRoot());
    fModulesMenu->Connect("Activated(Int_t)", "TMTAPSMaintain", menuHandler, "HandleMenu(Int_t)");

    // create the "Help" menu
    fHelpMenu = new TGPopupMenu(gClient->GetRoot());
    fHelpMenu->AddEntry("About", kACTION_HELP_ABOUT);
    fHelpMenu->Connect("Activated(Int_t)", "TMTAPSMaintain", menuHandler, "HandleMenu(Int_t)");

    // create the layout and add all menus
    fLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
    fMenuBar->AddPopup("&File", fFileMenu, fLayout);
    fMenuBar->AddPopup("&Modules", fModulesMenu, fLayout);
    fMenuBar->AddPopup("H&elp", fHelpMenu, fLayout);
    
    AddFrame(fMenuBar, new TGLayoutHints(kLHintsExpandX, 0, 0, 0, 0));
    
    
    // ------------------------------ Tool bar ------------------------------
        
    fToolBar = new TGToolBar(this, 50, 50);
    fToolBar->Connect("Clicked(Int_t)", "TMTAPSMaintain", menuHandler, "HandleMenu(Int_t)");
    
    ToolBarData_t toolBarData;
    toolBarData.fButton = 0;        
    toolBarData.fStayDown = kFALSE;  
    
    toolBarData.fPixmap = "quit.xpm"; 
    toolBarData.fTipText = "Quit TAPSMaintain"; 
    toolBarData.fId = kACTION_FILE_QUIT;          
    fToolBar->AddButton(this, &toolBarData, 0);
    
    toolBarData.fPixmap = "open.xpm"; 
    toolBarData.fTipText = "Open ROOT file"; 
    toolBarData.fId = kACTION_OPEN_ROOT_FILE;          
    fToolBar->AddButton(this, &toolBarData, 10);
    
    toolBarData.fPixmap = "stop_t.xpm"; 
    toolBarData.fTipText = "Stop active module"; 
    toolBarData.fId = kACTION_MODULE_STOP;          
    fToolBar->AddButton(this, &toolBarData, 10);

    AddFrame(new TGHorizontal3DLine(this), new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    AddFrame(new TGHorizontal3DLine(this), new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 5));
   
    
    // ------------------------- Module frame with placeholder -------------------------
    fModuleFrame = new TGCompositeFrame(this, 800, 600, kVerticalFrame);
    AddFrame(fModuleFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    
    // create new welcome frame
    fWelcomeFrame = new TGCompositeFrame(fModuleFrame, 800, 600, kVerticalFrame);
    fWelcomeFrame->ChangeBackground(TColor::RGB2Pixel(255, 255, 255));

    sprintf(pixel, "TAPSMaintain v%s", kTAPSMaintainVersion);
    TGLabel* l = new TGLabel(fWelcomeFrame, pixel);
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-34-*-*-*-*-*-iso8859-1");
    l->ChangeBackground(TColor::RGB2Pixel(255, 255, 255));
    fWelcomeFrame->AddFrame(l, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 10, 10, 50, 10));

    // create new icon
    fTAPSIcon = new TGIcon(fWelcomeFrame);
    fTAPSIcon->SetImage(fTAPSLogo);

    // add TAPS icon to module frame
    fWelcomeFrame->AddFrame(fTAPSIcon, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 400, 400, 200, 200));
    fWelcomeFrame->Layout();
    
    // add welcome frame to main frame
    fModuleFrame->AddFrame(fWelcomeFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    fModuleFrame->MapSubwindows();
    fModuleFrame->Layout();
    
    // ------------------------------ Status bar ------------------------------
    Int_t parts[] = {80, 20};
    fStatusBar = new TGStatusBar(this, 50, 10, kHorizontalFrame);
    fStatusBar->SetParts(parts, 2);
    AddFrame(fStatusBar, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));
}

//______________________________________________________________________________
TMMainFrame::~TMMainFrame()
{
    // Destructor.
    
    UnmapWindow();
    Cleanup();
}

//______________________________________________________________________________ 
void TMMainFrame::Show()
{
    // Show the interface.
    
    // Map all subwindows of main frame
    MapSubwindows();

    // Initialize the layout algorithm
    Resize(GetDefaultSize());

    // Map main frame
    MapRaised();
}

//______________________________________________________________________________ 
void TMMainFrame::LoadModuleFrame(TGCompositeFrame* mf)
{
    // Load the frame mf into the module frame.
    
    // unload welcome frame
    UnloadWelcomeFrame();
    
    // add an display module frame
    mf->MapSubwindows();
    mf->Layout();
    mf->ReparentWindow(fModuleFrame);
    fModuleFrame->AddFrame(mf, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    fModuleFrame->MapSubwindows();
    fModuleFrame->Layout();
}

//______________________________________________________________________________ 
void TMMainFrame::UnloadModuleFrame(TGCompositeFrame* mf)
{
    // Unload the frame mf from the module frame.
    
    // unload module frame
    fModuleFrame->RemoveFrame(mf);
    mf->UnmapWindow();
    mf->ReparentWindow(0);
    
    // load welcome frame
    LoadWelcomeFrame();
}

//______________________________________________________________________________ 
void TMMainFrame::LoadWelcomeFrame()
{
    // Load the welcome frame into the module frame.
    
    fWelcomeFrame->MapSubwindows();
    fWelcomeFrame->Layout();
    fWelcomeFrame->ReparentWindow(fModuleFrame);
    fModuleFrame->AddFrame(fWelcomeFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    fModuleFrame->MapSubwindows();
    fModuleFrame->Layout();
}

//______________________________________________________________________________ 
void TMMainFrame::UnloadWelcomeFrame()
{
    // Unload the welcome frame from the module frame.
    
    fModuleFrame->RemoveFrame(fWelcomeFrame);
    fWelcomeFrame->UnmapWindow();
    fWelcomeFrame->ReparentWindow(0);
}

