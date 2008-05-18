// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMTAPSMaintain                                                       //
//                                                                      //
// TAPSMaintain main class containing the main window and the module    //
// loader.                                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMTAPSMaintain.h"

ClassImp(TMTAPSMaintain)


//______________________________________________________________________________
TMTAPSMaintain::TMTAPSMaintain()
    : TObject()
{
    // Default constructor.

    fModuleLoader = 0;
    fMainWindow = 0;
    fActiveModule = 0;
    fFile = 0;
    fCurrentDir[0] = '\0';
}

//______________________________________________________________________________
TMTAPSMaintain::~TMTAPSMaintain()
{
    // Destructor.
    
    if (fMainWindow) delete fMainWindow;
    if (fModuleLoader) delete fModuleLoader;
    if (fFile) delete fFile;
}

//______________________________________________________________________________
void TMTAPSMaintain::Start()
{
    // Start the application.
    
    printf("\nStarting up TAPSMaintain Version %s ...\n\n", gTAPSMaintainVersion);

    // set current directory
    strcpy(fCurrentDir, ".");
    
    // Load the GUI
    if (!fMainWindow) fMainWindow = new TMMainFrame(gClient->GetRoot(), this);
    printf("GUI loaded.\n");
    
    // Load the module loader
    if (!fModuleLoader) fModuleLoader = new TMModuleLoader(1000);
    printf("ModuleLoader loaded.\n");
    
    // connect the modules to the GUI
    TList* modules = fModuleLoader->GetModules();
    Char_t name[256];
    UInt_t id;
    for (Int_t i = 0; i < modules->GetSize(); i++)
    {
        // load module
        TMModule* m = (TMModule*) modules->At(i);
        strcpy(name, m->GetName());
        id = m->GetID();
        
        // create menu entry
        fMainWindow->GetModulesMenu()->AddEntry(name, id);
        
        // connect Finished() signal
        m->Connect("Finished()", "TMTAPSMaintain", this, "StopModule(=kTRUE)");
        
        // connect Save() signal
        m->Connect("Save()", "TMTAPSMaintain", this, "SaveModuleResults()");
        
        // connect ModuleError() signal
        m->Connect("ModuleError(const Char_t*)", "TMTAPSMaintain", this, "ShowModuleError(const Char_t*)");

        // create configuration dialog if necessary
        if (m->NeedsConfig())
        {
            // create config dialog
            m->CreateConfigDialog(fMainWindow);

            // make connection to start the module after dialog unloading
            m->GetConfigDialogCloseButton()->Connect("Clicked()", "TMTAPSMaintain", this, "SetConfigAndStartModule()");
        }
        
        printf("Module '%s' (ID %d) loaded.\n", name, id);
    }
    
    fMainWindow->Show();
    printf("\nReady!\n");
}

//______________________________________________________________________________ 
void TMTAPSMaintain::Quit()
{
    // Quit the TAPSMaintain application.
    
    // module still active: ask if user really want to quit
    if (fActiveModule)
    {
        Int_t retval;
        new TGMsgBox(gClient->GetRoot(), fMainWindow, "Module active", "Do you really want to quit and stop this module? "
                     "The results of this module will NOT be saved!", kMBIconQuestion, 
                     kMBYes | kMBNo, &retval, kFitWidth | kFitHeight, kTextLeft);
    
        // abort quiting
        if (retval == 2) return;
    }
    
    if (fMainWindow) delete fMainWindow;
    if (fModuleLoader) delete fModuleLoader;
    
    gApplication->Terminate(0);
}

//______________________________________________________________________________ 
void TMTAPSMaintain::StartModule(TMModule* mod)
{
    // Start a module.
    
    // check if an other module is active
    if (fActiveModule)
    {
         printf("ERROR: Could not start module '%s' because module '%s' is still running!\n", 
                mod->GetName(), fActiveModule->GetName());
    }
    // check if needed ROOT was opened
    else if (mod->NeedsRootInputFile() && !fFile)
    {
        Int_t retval;
        new TGMsgBox(gClient->GetRoot(), fMainWindow, "Open ROOT input file", "This module needs a ROOT input file. "
                     "Please open one and try again.", kMBIconStop, kMBOk, &retval, kFitWidth | kFitHeight, kTextLeft);
    }
    else
    {
        printf("Starting module '%s'\n", mod->GetName());
        
        // set ROOT file if necessary
        if (mod->NeedsRootInputFile()) mod->SetRootInputFile(fFile);
       
        // start module configuration frame (if any)
        // or start module directly without configuration
        if (mod->NeedsConfig())
        {
            // connect frame and display it
            TGTransientFrame* configDialog = mod->GetConfigDialog();
            configDialog->ReparentWindow(gClient->GetRoot());
            configDialog->SetSize(configDialog->GetDefaultSize());
            configDialog->MapSubwindows();
            configDialog->MapRaised();
            
            // set active modules
            fActiveModule = mod;
        }
        else
        {
            // set active modules
            fActiveModule = mod;
            
            // display module frame
            fMainWindow->LoadModuleFrame(fActiveModule->GetFrame());
                    
            // diplay module name
            fMainWindow->SetStatusBar(fActiveModule->GetName(), 1);
                               
            // start module
            fActiveModule->Init();
        }
    }
}

//______________________________________________________________________________ 
void TMTAPSMaintain::SetConfigAndStartModule()
{
    // Unload the configuration dialog of a module and start it.

    // unload the configuration dialog
    fActiveModule->GetConfigDialog()->UnmapWindow();
    fActiveModule->GetConfigDialog()->ReparentWindow(0);

    // read the module configuration
    fActiveModule->ReadConfig();

    // display module frame
    fMainWindow->LoadModuleFrame(fActiveModule->GetFrame());
            
    // diplay module name
    fMainWindow->SetStatusBar(fActiveModule->GetName(), 1);
                       
    // start module
    fActiveModule->Init();
}

//______________________________________________________________________________ 
void TMTAPSMaintain::StopModule(Bool_t forced)
{
    // Stop the active module. Don't ask the user when forced is kTRUE (used for
    // completed module unloading). Default is kFALSE.
    
    if (!fActiveModule)
    {
         printf("ERROR: Cannot stop module because no module is loaded!\n");
    }
    else
    {
        if (!forced)
        {
            // ask if user really want to stop the module
            Int_t retval;
            new TGMsgBox(gClient->GetRoot(), fMainWindow, "Stop running module?", "Do you really want to stop this module? "
                     "The results of this module will NOT be saved!", kMBIconQuestion, 
                     kMBYes | kMBNo, &retval, kFitWidth | kFitHeight, kTextLeft);
        
            // abort module stopping
            if (retval == 2) return;
        }
        
        printf("Stopping module '%s'\n", fActiveModule->GetName());
        
        // unload frame
        fMainWindow->UnloadModuleFrame(fActiveModule->GetFrame());
        
        // unload configuration frame
        if (fActiveModule->NeedsConfig())
        {
            TGTransientFrame* configDialog = fActiveModule->GetConfigDialog();
            configDialog->UnmapWindow();
            configDialog->ReparentWindow(0);
        }
 
        // clear module name in status bar
        fMainWindow->SetStatusBar("", 1);
            
        // unset active modules
        fActiveModule = 0;
    }
}

//______________________________________________________________________________ 
void TMTAPSMaintain::HandleMenu(Int_t id)
{
    // Handle events of the menu.
    
    if (id == EACTION_FILE_QUIT) Quit();
    else if (id == EACTION_OPEN_ROOT_FILE) SelectAndOpenFile();
    else if (id == EACTION_MODULE_STOP) StopModule();
    else if (id == EACTION_HELP_ABOUT)
    {
        Int_t retval;
        Char_t cAbout[256];
        
        sprintf(cAbout,
               "TAPSMaintain\n"
               "A Universal TAPS Maintenance Software\n\n"
               "Version %s\n"
               "Compiled on %s\n"
               "Linked to ROOT %s\n\n"
               "(C) 2008 by Dominik Werthmueller",
               gTAPSMaintainVersion, __DATE__, ROOT_RELEASE);
        
        new TGMsgBox(gClient->GetRoot(), fMainWindow, "About TAPSMaintain", cAbout, 
                                 kMBIconAsterisk, kMBOk, &retval, kFitWidth | kFitHeight, kTextLeft);
    }
    
    else
    {
        // look for a module with this event ID
        TMModule* mod = fModuleLoader->GetModuleWithID(id);
        if (mod) 
        {
            StartModule(mod);
        }
    }
}

//______________________________________________________________________________ 
Bool_t TMTAPSMaintain::OpenInputFile(const char* inFile)
{
    // Open a ROOT file. Return kTRUE if successful, otherwise kFALSE.
    
    // open the file
    TFile* fileTest = new TFile(inFile, "READ");
    
    // check if opening was successful
    if (fileTest->IsZombie())
    {
        delete fileTest;
        return kFALSE;
    }
    else
    {
        // close old file
        if (fFile) delete fFile;
        fFile = fileTest;
        
        fMainWindow->SetStatusBar(inFile, 0);
        printf("Opened ROOT file '%s'.\n", inFile);
        return kTRUE;
    }
}

//______________________________________________________________________________ 
void TMTAPSMaintain::SelectAndOpenFile()
{
    // Let the user select a file to open.
    
    // check if a module is active
    if (fActiveModule)
    {
        printf("ERROR: Cannot open a new ROOT file while a module is still active!\n");
        return;
    }
    
    // configure the file choose dialog
    const char* kFileExt[] = {"ROOT files", "*.root", 0, 0};
    TGFileInfo fileInfo;
    fileInfo.fFileTypes = kFileExt;
    fileInfo.fIniDir = strdup((char *)fCurrentDir);

    // show the dialog
    new TGFileDialog(gClient->GetRoot(), fMainWindow, kFDOpen, &fileInfo);

    // open the file if the user selected one
    if (fileInfo.fFilename)
    {
        if (!OpenInputFile(fileInfo.fFilename)) printf("ERROR: Could not open ROOT input file '%s'!\n", fileInfo.fFilename);
    }

    // save current directory
    strcpy(fCurrentDir, fileInfo.fIniDir);
}

//______________________________________________________________________________ 
void TMTAPSMaintain::ShowModuleError(const Char_t* msg) const
{
    // Show the error message emitted by a module.
    
    Int_t retval;
    new TGMsgBox(gClient->GetRoot(), fMainWindow, "Module Error", msg, 
                 kMBIconStop, kMBOk, &retval, kFitWidth | kFitHeight, kTextLeft);
}

//______________________________________________________________________________ 
void TMTAPSMaintain::SaveModuleResults()
{
    // Open a file save dialog and call the module's SaveResult() method.

    // check if module is active
    if (!fActiveModule) return;

    const char* kFileExt[] = {"All files", "*", 0, 0};
    TGFileInfo fileInfo;
    fileInfo.fFileTypes = kFileExt;
    fileInfo.fIniDir = strdup((char *)fCurrentDir);

    // show the dialog
    new TGFileDialog(gClient->GetRoot(), fMainWindow, kFDSave, &fileInfo);

    // save module results
    if (fileInfo.fFilename)
    {
        fActiveModule->SaveResults(fileInfo.fFilename);
    }

    // save current directory
    strcpy(fCurrentDir, fileInfo.fIniDir);
}


