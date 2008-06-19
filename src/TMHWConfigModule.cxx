// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMHWConfigModule                                                     //
//                                                                      //
// Module for the manipulation of the TAPS hardware settings.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMHWConfigModule.h"

ClassImp(TMHWConfigModule)


//______________________________________________________________________________
TMHWConfigModule::TMHWConfigModule(const Char_t* name, UInt_t id)
    : TMModule(name, id, 0, kFALSE, kFALSE)
{
    // Constructor.
    
    Char_t aName[256];
    
    // initialize members   
    fCurrentTable[0] = '\0';
    fLEDFitFunctions = 0;
    fLEDGraphs = 0;
    fNLEDCalibSets = 0;
    fExternalCanvas = 0;


    fFrame->SetLayoutManager(new TGHorizontalLayout(fFrame));
    
    // ------------------------------ Control frame ------------------------------
    fControlFrame = new TGCompositeFrame(fFrame, 50, 50);
    fControlFrame->SetLayoutManager(new TGTableLayout(fControlFrame, 6, 2));
    
    // DB URL
    TGLabel* l = new TGLabel(fControlFrame, "DB URL:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    fDBURLEntry = new TGTextEntry(fControlFrame, gTAPS_DB_URL);
    fControlFrame->AddFrame(fDBURLEntry, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));
    
    // DB User
    l = new TGLabel(fControlFrame, "DB User:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    fDBUserEntry = new TGTextEntry(fControlFrame, gTAPS_DB_User);
    fControlFrame->AddFrame(fDBUserEntry, new TGTableLayoutHints(1, 2, 1, 2, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));

    // DB password
    l = new TGLabel(fControlFrame, "DB Password:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    fDBPasswdEntry = new TGTextEntry(fControlFrame, gTAPS_DB_Passwd);
    fDBPasswdEntry->SetEchoMode(TGTextEntry::kPassword);
    fControlFrame->AddFrame(fDBPasswdEntry, new TGTableLayoutHints(1, 2, 2, 3, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));

    // DB table
    l = new TGLabel(fControlFrame, "DB table:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 3, 4, kLHintsFillX | kLHintsLeft, 5, 5, 5, 5));
    
    // combo box
    fTableCombo = new TGComboBox(fControlFrame);
    fTableCombo->Connect("Selected(Int_t)", "TMHWConfigModule", this, "ReadTable(Int_t)");
    fTableCombo->Resize(200, 22);
    fTableCombo->AddEntry("Select TAPS table here", EDB_Table_Empty);
    fTableCombo->AddEntry("BaF2 High Voltage", EDB_Table_BaF2_HV);
    fTableCombo->AddEntry("BaF2 CFD Threshold", EDB_Table_BaF2_CFD);
    fTableCombo->AddEntry("BaF2 LED1 Threshold", EDB_Table_BaF2_LED1);
    fTableCombo->AddEntry("BaF2 LED2 Threshold", EDB_Table_BaF2_LED2);
    fTableCombo->AddEntry("Veto LED Threshold", EDB_Table_Veto_LED);
    fControlFrame->AddFrame(fTableCombo, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));
    fTableCombo->Select(EDB_Table_Empty, kFALSE);


    // ------------------------------ Begin Tab ------------------------------
    fSettingsTab = new TGTab(fControlFrame, 50, 50);   

    // ------------------------------ Range manipulation frame ------------------------------
    fRangeManipFrame = fSettingsTab->AddTab("Set Values");
    fRangeManipFrame->SetLayoutManager(new TGHorizontalLayout(fRangeManipFrame));
    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "Set all"), new TGLayoutHints(kLHintsLeft, 15, 5, 35, 5));
    
    fRangeManipCombo = new TGComboBox(fRangeManipFrame);
    fRangeManipCombo->Resize(150, 22);
    fRangeManipCombo->AddEntry("elements", ERange_All_Elements);
    fRangeManipCombo->AddEntry("Block A elements", ERange_Block_A);
    fRangeManipCombo->AddEntry("Block B elements", ERange_Block_B);
    fRangeManipCombo->AddEntry("Block C elements", ERange_Block_C);
    fRangeManipCombo->AddEntry("Block D elements", ERange_Block_D);
    fRangeManipCombo->AddEntry("Block E elements", ERange_Block_E);
    fRangeManipCombo->AddEntry("Block F elements", ERange_Block_F);
    fRangeManipCombo->AddEntry("Ring 1 elements", ERange_Ring_1);
    fRangeManipCombo->AddEntry("Ring 2 elements", ERange_Ring_2);
    fRangeManipCombo->AddEntry("Ring 3 elements", ERange_Ring_3);
    fRangeManipCombo->AddEntry("Ring 4 elements", ERange_Ring_4);
    fRangeManipCombo->AddEntry("Ring 5 elements", ERange_Ring_5);
    fRangeManipCombo->AddEntry("Ring 6 elements", ERange_Ring_6);
    fRangeManipCombo->AddEntry("Ring 7 elements", ERange_Ring_7);
    fRangeManipCombo->AddEntry("Ring 8 elements", ERange_Ring_8);
    fRangeManipCombo->AddEntry("Ring 9 elements", ERange_Ring_9);
    fRangeManipCombo->AddEntry("Ring 10 elements", ERange_Ring_10);
    fRangeManipCombo->AddEntry("Ring 11 elements", ERange_Ring_11);
    fRangeManipFrame->AddFrame(fRangeManipCombo, new TGLayoutHints(kLHintsLeft, 7, 5, 32, 2));
    fRangeManipCombo->Select(ERange_All_Elements, kFALSE);

    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "to"), new TGLayoutHints(kLHintsLeft, 7, 5, 35, 5));
    
    fRangeManipEntry = new TGNumberEntry(fRangeManipFrame, 0, 8);
    fRangeManipFrame->AddFrame(fRangeManipEntry, new TGLayoutHints(kLHintsLeft, 7, 5, 32, 2));

    fRangeManipButton = new TGTextButton(fRangeManipFrame, "    Set    ");
    fRangeManipButton->Connect("Clicked()", "TMHWConfigModule", this, "DoRangeManipulation()");
    fRangeManipFrame->AddFrame(fRangeManipButton, new TGLayoutHints(kLHintsLeft, 7, 15, 32, 2));

    
    // ------------------------------ Import frame ------------------------------
    fImportFrame = fSettingsTab->AddTab("Import");
    fImportFrame->SetLayoutManager(new TGTableLayout(fImportFrame, 4, 4));
   
    l = new TGLabel(fImportFrame, "Input file:");
    l->SetTextJustify(kTextLeft);
    fImportFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5)); 

    fImportFileEntry = new TGTextEntry(fImportFrame, "");
    fImportFileEntry->Resize(200, 22);
    fImportFrame->AddFrame(fImportFileEntry,
                           new TGTableLayoutHints(0, 3, 1, 2, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));

    fImportBrowse = new TGTextButton(fImportFrame, " Browse ");
    fImportBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectImportFile()");
    fImportFrame->AddFrame(fImportBrowse, 
                           new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

    fImportButton = new TGTextButton(fImportFrame, " Import ");
    fImportButton->Connect("Clicked()", "TMHWConfigModule", this, "ImportFile()");
    fImportFrame->AddFrame(fImportButton, 
                           new TGTableLayoutHints(3, 4, 2, 3, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

    l = new TGLabel(fImportFrame, "Import values from a file. File format should be 2 column space sep-\n"
                                  "arated ELEMENT ID and VALUE. Commented lines using # are\n"
                                  "allowed.\n");
    l->SetTextJustify(kTextLeft);
    fImportFrame->AddFrame(l, new TGTableLayoutHints(0, 4, 3, 4, kLHintsFillX | kLHintsLeft, 15, 15, 15, 5));


    // ------------------------------ Export frame ------------------------------
    fExportFrame = fSettingsTab->AddTab("Export");
    fExportFrame->SetLayoutManager(new TGTableLayout(fExportFrame, 4, 4));
    
    l = new TGLabel(fExportFrame, "Output file:");
    l->SetTextJustify(kTextLeft);
    fExportFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5)); 
    
    fExportFileEntry = new TGTextEntry(fExportFrame, "");
    fExportFileEntry->Resize(200, 22);
    fExportFrame->AddFrame(fExportFileEntry,
                           new TGTableLayoutHints(0, 3, 1, 2, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));
    
    fExportBrowse = new TGTextButton(fExportFrame, " Browse ");
    fExportBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectExportFile()");
    fExportFrame->AddFrame(fExportBrowse, 
                           new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

    fExportButton = new TGTextButton(fExportFrame, " Export ");
    fExportButton->Connect("Clicked()", "TMHWConfigModule", this, "ExportFile()");
    fExportFrame->AddFrame(fExportButton, 
                           new TGTableLayoutHints(3, 4, 2, 3, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

  l = new TGLabel(fExportFrame, "Export all current values into the specified file. The format is 2 column\n"
                                  "ELEMENT ID and VALUE space separated\n");
    l->SetTextJustify(kTextLeft);
    fExportFrame->AddFrame(l, new TGTableLayoutHints(0, 4, 3, 4, kLHintsFillX | kLHintsLeft, 15, 15, 15, 5));

    
    // ------------------------------ Gain match frame ------------------------------
    fGMFrame = fSettingsTab->AddTab("BaF2 gain match");
    fGMFrame->SetLayoutManager(new TGTableLayout(fGMFrame, 6, 4));
    
    l = new TGLabel(fGMFrame, "Calibration file:");
    l->SetTextJustify(kTextLeft);
    fGMFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
    
    fGMCalibFileEntry = new TGTextEntry(fGMFrame, "");
    fGMCalibFileEntry->Resize(200, 22);
    fGMFrame->AddFrame(fGMCalibFileEntry,
                       new TGTableLayoutHints(0, 3, 1, 2, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));
    
    fGMCalibBrowse = new TGTextButton(fGMFrame, " Browse ");
    fGMCalibBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectGMCalibFile()");
    fGMFrame->AddFrame(fGMCalibBrowse, 
                       new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    l = new TGLabel(fGMFrame, "Range (MeV):");
    l->SetTextJustify(kTextLeft);
    fGMFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
 
    fGMRangeEntry = new TGNumberEntry(fGMFrame, 1000);
    fGMRangeEntry->Resize(80, 22);
    fGMFrame->AddFrame(fGMRangeEntry,
                       new TGTableLayoutHints(1, 2, 2, 3, kLHintsLeft, 5, 5, 12, 2));
    
    l = new TGLabel(fGMFrame, "Resolution (Channels):");
    l->SetTextJustify(kTextLeft);
    fGMFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 3, 4, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
 
    fGMResEntry = new TGNumberEntry(fGMFrame, 4096);
    fGMResEntry->Resize(80, 22);
    fGMFrame->AddFrame(fGMResEntry,
                       new TGTableLayoutHints(1, 2, 3, 4, kLHintsLeft, 5, 5, 12, 2));

    
    fGMButton = new TGTextButton(fGMFrame, " Calculate new HV ");
    fGMButton->Connect("Clicked()", "TMHWConfigModule", this, "DoGainMatch()");
    fGMFrame->AddFrame(fGMButton, 
                       new TGTableLayoutHints(3, 4, 4, 5, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));




    l = new TGLabel(fGMFrame, "Open a cosmics calibration file created by TAPSMaintain and enter\n"
                              "the desired range and the available resolution to calculate the new\n"
                              "HV values. The old HV values are directly read form the database.\n"
                              "Afterwards you can write them to the DB or to the hardware crate.\n");
    l->SetTextJustify(kTextLeft);
    fGMFrame->AddFrame(l, new TGTableLayoutHints(0, 4, 5, 6, kLHintsFillX | kLHintsLeft, 15, 15, 15, 5));

    // ------------------------------ LED settings frame ------------------------------
    fLEDFrame = fSettingsTab->AddTab("LED");
    fLEDFrame->SetLayoutManager(new TGTableLayout(fLEDFrame, 5, 4));
    
    l = new TGLabel(fLEDFrame, "LED calibration file:");
    l->SetTextJustify(kTextLeft);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
    
    fLEDCalibFileEntry = new TGTextEntry(fLEDFrame, "");
    fLEDCalibFileEntry->Resize(200, 22);
    fLEDFrame->AddFrame(fLEDCalibFileEntry,
                       new TGTableLayoutHints(0, 3, 1, 2, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));
    
    fLEDCalibBrowse = new TGTextButton(fLEDFrame, " Browse ");
    fLEDCalibBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectLEDCalibFile()");
    fLEDFrame->AddFrame(fLEDCalibBrowse, 
                       new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    fLEDAddFileButton = new TGTextButton(fLEDFrame, " Add ");
    fLEDAddFileButton->Connect("Clicked()", "TMHWConfigModule", this, "AddFileToLEDCalibration()");
    fLEDFrame->AddFrame(fLEDAddFileButton, 
                       new TGTableLayoutHints(3, 4, 2, 3, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    l = new TGLabel(fLEDFrame, "Choose channel to show:");
    l->SetTextJustify(kTextRight);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsLeft, 5, 15, 8, 2));
    
    fLEDShowChannelEntry = new TGNumberEntry(fLEDFrame, 1, 4);
    fLEDFrame->AddFrame(fLEDShowChannelEntry,
                       new TGTableLayoutHints(2, 3, 3, 4, kLHintsLeft, 5, 15, 5, 5));

    fLEDShowChannelButton = new TGTextButton(fLEDFrame, " Show ");
    fLEDShowChannelButton->Connect("Clicked()", "TMHWConfigModule", this, "ShowLEDCalibChannel()");
    fLEDFrame->AddFrame(fLEDShowChannelButton, 
                       new TGTableLayoutHints(3, 4, 3, 4, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
                                          
   
    
    l = new TGLabel(fLEDFrame, "Open several LED calibration files and add them to the LED setting\n"
                              "calculation. A linear fit is performed for every TAPS channel using\n"
                              "the points of the calibration files.");
    l->SetTextJustify(kTextLeft);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 4, 4, 5, kLHintsFillX | kLHintsLeft, 15, 15, 15, 5));
    
                       
    // ------------------------------ End Tab ------------------------------
    fControlFrame->AddFrame(fSettingsTab,  new TGTableLayoutHints(0, 2, 4, 5, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));
    fSettingsTab->SetEnabled(3, kFALSE);



    // ------------------------------ Main control buttons ------------------------------
    fButtonsFrame = new TGCompositeFrame(fControlFrame);
    fButtonsFrame->SetLayoutManager(new TGTableLayout(fButtonsFrame, 1, 3));
    
    fWriteDBButton = new TGTextButton(fButtonsFrame, "Write to DB");
    fWriteDBButton->SetTopMargin(15);
    fWriteDBButton->SetBottomMargin(15);
    fWriteDBButton->SetRightMargin(15);
    fWriteDBButton->SetLeftMargin(15);
    fWriteDBButton->Connect("Clicked()", "TMHWConfigModule", this, "WriteTable()");
    fButtonsFrame->AddFrame(fWriteDBButton,
                            new TGTableLayoutHints(0, 1, 0, 1, kLHintsExpandX | kLHintsCenterX, 5, 5, 5, 5));

    fWriteHWButton = new TGTextButton(fButtonsFrame, "Write to HW");
    fWriteHWButton->SetEnabled(kFALSE);
    fWriteHWButton->SetTopMargin(15);
    fWriteHWButton->SetBottomMargin(15);
    fWriteHWButton->SetRightMargin(15);
    fWriteHWButton->SetLeftMargin(15);
    fWriteHWButton->Connect("Clicked()", "TMHWConfigModule", this, "WriteHVToHardware()");
    fButtonsFrame->AddFrame(fWriteHWButton,
                            new TGTableLayoutHints(1, 2, 0, 1, kLHintsExpandX | kLHintsCenterX, 5, 5, 5, 5));

    fQuitButton = new TGTextButton(fButtonsFrame, "Quit Module");
    fQuitButton->SetTopMargin(15);
    fQuitButton->SetBottomMargin(15);
    fQuitButton->SetRightMargin(15);
    fQuitButton->SetLeftMargin(15);
    fQuitButton->Connect("Clicked()", "TMHWConfigModule", this, "Finished()");
    fButtonsFrame->AddFrame(fQuitButton,
                            new TGTableLayoutHints(2, 3, 0, 1, kLHintsExpandX | kLHintsCenterX, 5, 5, 5, 5));

    // add buttons frame to control frame
    fControlFrame->AddFrame(fButtonsFrame,  new TGTableLayoutHints(0, 2, 5, 6, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));


    //// Progress bar
    //fProgressBar = new TGHProgressBar(fControlFrame, TGProgressBar::kFancy, 10);
    //fProgressBar->SetBarColor("green");
    //fProgressBar->Resize(200, 25);
    //fProgressBar->SetMin(0);
    //fProgressBar->SetMax(gMaxSize);
    //fProgressBar->ShowPosition(kTRUE, kFALSE, "Writing element %.0f");
    //fControlFrame->AddFrame(fProgressBar,  new TGTableLayoutHints(0, 2, 8, 9, kLHintsFillX | kLHintsFillY | kLHintsLeft, 5, 5, 35, 5));
    

    // add control frame to main frame
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsFillX | kLHintsFillY, 10, 10, 10, 10));
    fControlFrame->Resize(fControlFrame->GetDefaultSize()); 
 

    // ------------------------------ Input frame ------------------------------
    fInputFrame = new TGVerticalFrame(fFrame, 250, 500);
    
    
    fTableTitle = new TGLabel(fInputFrame, "Table: none");
    fTableTitle->SetTextJustify(kTextLeft);
    fTableTitle->SetTextFont("-adobe-helvetica-bold-r-*-*-18-*-*-*-*-*-iso8859-1");
    fInputFrame->AddFrame(fTableTitle, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 20));

    // table scroll canvas
    fTableCanvas = new TGCanvas(fInputFrame, 250, 500, kSunkenFrame, TGCanvas::GetWhitePixel());
    fTableFrame = new TGCompositeFrame(fTableCanvas->GetViewPort(), 250, 500, kVerticalFrame | kOwnBackground);
    fTableFrame->SetLayoutManager(new TGTableLayout(fTableFrame, gMaxSize+1, 4));
    
    // mouse wheel scrolling
    fTableFrame->Connect("ProcessedEvent(Event_t*)", "TMHWConfigModule", this, "HandleMouseWheel(Event_t*)");
    gVirtualX->GrabButton(fTableFrame->GetId(), kButton4, kAnyModifier, 
                          kButtonPressMask | kButtonReleaseMask | kPointerMotionMask, kNone, kNone);
    gVirtualX->GrabButton(fTableFrame->GetId(), kButton5, kAnyModifier, 
                          kButtonPressMask | kButtonReleaseMask | kPointerMotionMask, kNone, kNone);

    // create table 
    fElementCurrentValue = new TGLabel*[gMaxSize];
    fElementNewValue = new TGNumberEntry*[gMaxSize];
    fElementValueChanged = new TGLabel*[gMaxSize];

    // add header
    l = new TGLabel(fTableFrame, "Element");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    fTableFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 10, 10, 10, 10));
    
    l = new TGLabel(fTableFrame, "Current Value");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    fTableFrame->AddFrame(l, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 10, 10, 10, 10));
    
    l = new TGLabel(fTableFrame, "New Value");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    fTableFrame->AddFrame(l, new TGTableLayoutHints(2, 3, 0, 1, kLHintsFillX | kLHintsLeft, 10, 10, 10, 10));
    
    l = new TGLabel(fTableFrame, "Change");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    fTableFrame->AddFrame(l, new TGTableLayoutHints(3, 4, 0, 1, kLHintsFillX | kLHintsLeft, 10, 10, 10, 10));
    
    // add element rows
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        sprintf(aName, "%d", i+1);

        // add element number
        l = new TGLabel(fTableFrame, aName);
        l->SetTextJustify(kTextRight);
        fTableFrame->AddFrame(l, new TGTableLayoutHints(0, 1, i+1, i+2, kLHintsFillX | kLHintsLeft, 4, 4, 8, 4));

        // add old value display
        fElementCurrentValue[i] = new TGLabel(fTableFrame, "0");
        fElementCurrentValue[i]->SetTextJustify(kTextRight);
        fTableFrame->AddFrame(fElementCurrentValue[i], 
                              new TGTableLayoutHints(1, 2, i+1, i+2, kLHintsFillX | kLHintsLeft, 4, 4, 8, 4));

        // add new value entry
        fElementNewValue[i] = new TGNumberEntry(fTableFrame, 0, 10);
        fElementNewValue[i]->Connect("ValueSet(Long_t)", "TMHWConfigModule", this, "MarkChanges()");
        fTableFrame->AddFrame(fElementNewValue[i], 
                              new TGTableLayoutHints(2, 3, i+1, i+2, kLHintsFillX | kLHintsLeft, 4, 4, 4, 4));
        
        // add value change
        fElementValueChanged[i] = new TGLabel(fTableFrame, "       ");
        fElementValueChanged[i]->SetTextJustify(kTextCenterX);
        fTableFrame->AddFrame(fElementValueChanged[i], 
                              new TGTableLayoutHints(3, 4, i+1, i+2, kLHintsFillX | kLHintsLeft, 4, 4, 8, 4));
    }
    
    // frame to canvas
    fTableCanvas->GetViewPort()->AddFrame(fTableFrame);
    fTableCanvas->SetContainer(fTableFrame);

    fInputFrame->AddFrame(fTableCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
    
    // add table frame to control frame
    fFrame->AddFrame(fInputFrame, new TGLayoutHints(kLHintsTop | kLHintsRight| kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 10));
}

//______________________________________________________________________________
TMHWConfigModule::~TMHWConfigModule()
{
    // Destructor.
    
}

//______________________________________________________________________________
void TMHWConfigModule::Init()
{
    // (Re-)initalize the module.

    // clear table
    ClearValues();

    // scroll to table beginning
    fTableCanvas->SetVsbPosition(0);

    // clear file name entries
    fImportFileEntry->SetText("");
    fExportFileEntry->SetText("");
    fGMCalibFileEntry->SetText("");
    fLEDCalibFileEntry->SetText("");

    // select first tab
    fSettingsTab->SetTab(0, kFALSE);

    // select default entry in combos
    fTableCombo->Select(EDB_Table_Empty, kTRUE);
    fRangeManipCombo->Select(ERange_All_Elements, kFALSE);
    
    // cleanup old LED calibration memory
    if (fNLEDCalibSets)
    {
        for (UInt_t i = 0; i < gMaxSize; i++) 
        {
            delete fLEDFitFunctions[i];
            delete fLEDGraphs[i];
        }
        
        delete [] fLEDFitFunctions;
        delete [] fLEDGraphs;
        
        fNLEDCalibSets = 0;
        fLEDFitFunctions = 0;
        fLEDGraphs = 0;
    }
}

//______________________________________________________________________________
void TMHWConfigModule::HandleMouseWheel(Event_t* event)
{
    // Handle mouse wheel to scroll (taken from tutorials/gui/guitest.C).

    if (event->fType != kButtonPress && event->fType != kButtonRelease) return;

    Int_t page = 0;
    if (event->fCode == kButton4 || event->fCode == kButton5) 
    {
        if (!fTableCanvas) return;
        if (fTableCanvas->GetContainer()->GetHeight())
        page = Int_t(Float_t(fTableCanvas->GetViewPort()->GetHeight() *
                              fTableCanvas->GetViewPort()->GetHeight()) /
                              fTableCanvas->GetContainer()->GetHeight());
    }

    if (event->fCode == kButton4) 
    {
        //scroll up
        Int_t newpos = fTableCanvas->GetVsbPosition() - page;
        if (newpos < 0) newpos = 0;
        fTableCanvas->SetVsbPosition(newpos);
    }
    if (event->fCode == kButton5) 
    {
        // scroll down
        Int_t newpos = fTableCanvas->GetVsbPosition() + page;
        fTableCanvas->SetVsbPosition(newpos);
    }
}

//______________________________________________________________________________
void TMHWConfigModule::SelectImportFile()
{
    // Let the user select a file to import the values from.

    Char_t filename[256];

    // Let TAPSMaintain open the "Open file" dialog
    ShowFileDialog(kFDOpen);

    // copy file name
    GetAndDeleteMiscFileName(filename);
  
    // abort if file name is empty
    if (!strcmp(filename, "")) return;

    // Check if it's a directory
    Long_t id, flags, modtime;
    Long64_t size;
    gSystem->GetPathInfo(filename, &id, &size, &flags, &modtime);
    
    // Write the selected file name into the text entry
    if (flags == 0 || flags == 1) fImportFileEntry->SetText(filename);
}

//______________________________________________________________________________
void TMHWConfigModule::SelectGMCalibFile()
{
    // Let the user select a calibration file for the gain match
    // function.

    Char_t filename[256];

    // Let TAPSMaintain open the "Open file" dialog
    ShowFileDialog(kFDOpen);

    // copy file name
    GetAndDeleteMiscFileName(filename);
  
    // abort if file name is empty
    if (!strcmp(filename, "")) return;

    // Check if it's a directory
    Long_t id, flags, modtime;
    Long64_t size;
    gSystem->GetPathInfo(filename, &id, &size, &flags, &modtime);
    
    // Write the selected file name into the text entry
    if (flags == 0 || flags == 1) fGMCalibFileEntry->SetText(filename);
}

//______________________________________________________________________________
void TMHWConfigModule::SelectLEDCalibFile()
{
    // Let the user select a calibration file for the LED setting.

    Char_t filename[256];

    // Let TAPSMaintain open the "Open file" dialog
    ShowFileDialog(kFDOpen);

    // copy file name
    GetAndDeleteMiscFileName(filename);
  
    // abort if file name is empty
    if (!strcmp(filename, "")) return;

    // Check if it's a directory
    Long_t id, flags, modtime;
    Long64_t size;
    gSystem->GetPathInfo(filename, &id, &size, &flags, &modtime);
    
    // Write the selected file name into the text entry
    if (flags == 0 || flags == 1) fLEDCalibFileEntry->SetText(filename);
}

//______________________________________________________________________________
void TMHWConfigModule::SelectExportFile()
{
    // Let the user select a file to export the value into.

    Char_t filename[256];

    // Let TAPSMaintain open the "Save file" dialog
    ShowFileDialog(kFDSave);
    
    // copy file name
    GetAndDeleteMiscFileName(filename);
    
    // abort if file name is empty
    if (!strcmp(filename, "")) return;

    // Check if it's a directory
    Long_t id, flags, modtime;
    Long64_t size;
    gSystem->GetPathInfo(filename, &id, &size, &flags, &modtime);
    
    // Write the selected file name into the text entry
    fExportFileEntry->SetText(filename);
}

//______________________________________________________________________________
void TMHWConfigModule::ImportFile()
{
    // Import values from the specified file.
    
    Char_t line[256];

    Int_t id;
    Float_t value;

    // get the selected file name
    const Char_t* filename = fImportFileEntry->GetText();

    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;

    // open the file
    FILE* fin;
    fin = fopen(filename, "r");
 
    // read file and set values
    while (!feof(fin))
    {
        fgets(line, 256, fin);

        // check if line is a comment
        if (TMUtils::IsComment(line)) continue;

        if (sscanf(line, "%d%f", &id, &value) == 2)
        {
            fElementNewValue[id-1]->SetNumber(value);
        }
    }

    // close the file
    fclose(fin);

    // check changes
    MarkChanges();
}

//______________________________________________________________________________
void TMHWConfigModule::DoGainMatch()
{
    // Perform gain match: calculate the new HV values.
    
    Char_t line[256];
    
    Int_t id;
    Float_t ped, peak, cosm_gain;
    Double_t range, res;
    Double_t nom_gain, gain_bias;
    Double_t hv_old, hv_new;
   
    // get the selected file name
    const Char_t* filename = fGMCalibFileEntry->GetText();

    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;

    // read range/resolution
    range = fGMRangeEntry->GetNumber();
    res = fGMResEntry->GetNumber();

    // open the file
    FILE* fin;
    fin = fopen(filename, "r");
 
    // read file and set values
    while (!feof(fin))
    {
        fgets(line, 256, fin);

        // check if line is a comment
        if (TMUtils::IsComment(line)) continue;
        
        // read id, pedestal pos., cosmic peak pos. and gain
        if (sscanf(line, "%d%f%f%f", &id, &ped, &peak, &cosm_gain) == 4)
        {
            if (ped == 0 && peak == 0) continue;

            // read old HV value
            hv_old = atof(fElementCurrentValue[id-1]->GetText()->Data());

            // calculate new HV value
            nom_gain = range / (res - ped);
            gain_bias = hv_old - gBaF2_Gain_Slope * TMath::Log(peak - ped);
            hv_new = gBaF2_Gain_Slope * TMath::Log(gTAPS_MIP_Loss_BaF2 / nom_gain) + gain_bias;
            
            // correct bad HV value
            if (hv_new < 0) hv_new = hv_old;

            // set new HV value
            fElementNewValue[id-1]->SetNumber((Int_t)hv_new);
        }
    }

    // close the file
    fclose(fin);

    // check changes
    MarkChanges();
}

//______________________________________________________________________________
void TMHWConfigModule::ExportFile()
{
    // Export values into the specified file.
    
    // get the selected file name
    const Char_t* filename = fExportFileEntry->GetText();

    // leave if export file entry is empty
    if (!strcmp(filename, "")) return;

    // open the file
    FILE* fout;
    fout = fopen(filename, "w");
    
    // write header
    fprintf(fout, "# TAPSMaintain database table export\n");
    fprintf(fout, "# Table: %s\n", fCurrentTable);
    fprintf(fout, "\n");
    fprintf(fout, "# ID    Value\n");
 
    // write current values to file
    for (UInt_t i = 0; i < gMaxSize; i++)
    {   
        fprintf(fout, "%3d    %s\n", i+1, fElementCurrentValue[i]->GetText()->Data());
    }

    // close the file
    fclose(fout);

    // show info message
    Char_t msg[256];
    sprintf(msg, "Values of table '%s' were saved to '%s' .", fCurrentTable, filename);
    ModuleInfo(msg);
}

//______________________________________________________________________________
void TMHWConfigModule::AddFileToLEDCalibration()
{
    // Add an additional file to the LED calibration. Regenerate and refit the
    // calibration graphs of every channel.

    Char_t line[256];
    Int_t id, voltageLED;
    Float_t thr;
    Double_t thresholds[gMaxSize];
    Double_t voltages[gMaxSize];
    
    // get the selected file name
    const Char_t* filename = fLEDCalibFileEntry->GetText();

    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;
    
    // clear file input entry
    fLEDCalibFileEntry->SetText("");
    
    // init arrays
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        thresholds[i] = 0;
        voltages[i] = 0;
    }
    
    // open the file
    FILE* fin;
    fin = fopen(filename, "r");
    
    // read voltages and threshold from file
    while (!feof(fin))
    {
        fgets(line, 256, fin);

        // check if line is a comment
        if (TMUtils::IsComment(line)) continue;
        
        // read id, LED threshold voltage and LED threshold channel
        if (sscanf(line, "%d%d%f", &id, &voltageLED, &thr) == 3)
        {
            voltages[id-1] = (Double_t) voltageLED;
            thresholds[id-1] = (Double_t) thr;
        }
    }
    fclose(fin);
    
    // check if fit functions exist
    if (!fLEDFitFunctions)
    {
        // generate LED fit functions
        fLEDFitFunctions = new TF1*[gMaxSize];
        for (UInt_t i = 0; i < gMaxSize; i++)
        {
            sprintf(line, "LED_fit_func_%i", i+1);
            fLEDFitFunctions[i] = new TF1(line, "pol1", 0, 1000);
        }
    }
    
    // generate and fit graphs if not created yet
    if (!fLEDGraphs)
    {
        // create graphs
        fLEDGraphs = new TGraph*[gMaxSize];
        for (UInt_t i = 0; i < gMaxSize; i++) fLEDGraphs[i] = new TGraph(1);
        
        // fill values into graphs
        for (UInt_t i = 0; i < gMaxSize; i++)
        {
            fLEDGraphs[i]->SetPoint(0, thresholds[i], voltages[i]);
        }
        
        fNLEDCalibSets = 1;
    }
    // regenerate and refit graphs if already created
    else
    {
        Double_t oldVoltages[fNLEDCalibSets];
        Double_t oldThresholds[fNLEDCalibSets];
        
        for (UInt_t i = 0; i < gMaxSize; i++)
        {            
            // backup current values
            Double_t* graphX = fLEDGraphs[i]->GetX();
            Double_t* graphY = fLEDGraphs[i]->GetY();
            
            for (UInt_t j = 0; j < fNLEDCalibSets; j++)
            {
                oldThresholds[j] = graphX[j];
                oldVoltages[j] = graphY[j];
            }
            
            // destroy current graph
            delete fLEDGraphs[i];
            
            // create new graph
            fLEDGraphs[i] = new TGraph(fNLEDCalibSets + 1);
            
            // fill new graph
            for (UInt_t j = 0; j < fNLEDCalibSets; j++)
            {
                fLEDGraphs[i]->SetPoint(j, oldThresholds[j], oldVoltages[j]);
            }
            fLEDGraphs[i]->SetPoint(fNLEDCalibSets, thresholds[i], voltages[i]);
            
            // fit new graph
            fLEDFitFunctions[i]->SetParameters(0, 0);
            sprintf(line, "LED_fit_func_%d", i+1);
            fLEDGraphs[i]->Fit(line, "Q");
            
        }
        
        // increment LED data set counter
        fNLEDCalibSets++;
    }
}

//______________________________________________________________________________
void TMHWConfigModule::ShowLEDCalibChannel()
{
    // Show the LED calibration graph including the fit function of a specific
    // TAPS channel.
    
    UInt_t id = (UInt_t) fLEDShowChannelEntry->GetNumber();
    
    if (id > 0 && id <= gMaxSize && fNLEDCalibSets)
    {
        TText aText;
        Char_t text[256];
        aText.SetTextSize(0.05);
        aText.SetNDC();
        
        CreateExternalCanvas();
        fLEDGraphs[id-1]->SetMarkerStyle(20);
        fLEDGraphs[id-1]->Draw("ap");
        sprintf(text, "LED Calibration: Channel %d", id);
        fLEDGraphs[id-1]->SetTitle(text);
        fLEDGraphs[id-1]->GetXaxis()->SetTitle("Threshold Channel");
        fLEDGraphs[id-1]->GetYaxis()->SetTitle("Applied Voltage [mV]");
        
        sprintf(text, "constant: %f", fLEDFitFunctions[id-1]->GetParameter(0));
        aText.DrawText(0.2, 0.8, text);
        sprintf(text, "slope: %f", fLEDFitFunctions[id-1]->GetParameter(1));
        aText.DrawText(0.2, 0.75, text);
    
        fExternalCanvas->Update();
    }
}

//______________________________________________________________________________
void TMHWConfigModule::CreateExternalCanvas()
{
    // Create the external canvas if necessary.
    
    if (!fExternalCanvas)
    {
        fExternalCanvas = new TCanvas();
        fExternalCanvas->Connect("Closed()", "TMHWConfigModule", this, "DestroyExternalCanvas()");
    }
}

//______________________________________________________________________________
void TMHWConfigModule::DestroyExternalCanvas()
{
    // Destroy the external canvas.
    
    if (fExternalCanvas)
    {
        //delete fExternalCanvas;
        fExternalCanvas = 0;
    }
}

//______________________________________________________________________________
void TMHWConfigModule::SetBlockValues(UInt_t block, Double_t value)
{
    // Set all elements of the block 'block' to the value 'value' (A = 1, B = 2, ...).

    UInt_t blockRange[6][2] = {{0, 64}, {64, 128}, {128, 192}, {192, 256}, {256, 320}, {320, 384}};

    // check bounds
    if (block > 6) return;

    // Set values
    for (UInt_t i = blockRange[block-1][0]; i < blockRange[block-1][1]; i++) 
    {
        fElementNewValue[i]->SetNumber(value);
    }
}

//______________________________________________________________________________
void TMHWConfigModule::SetRingValues(UInt_t ring, Double_t value)
{
    // Set all elements of the ring 'ring' to the value 'value' (inner ring = 1,
    // 2nd ring = 2, ...)

    // check bounds
    if (ring > 11) return;

    // loop over all elements and change value if element belongs to the
    // specified ring
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        // element belongs to specified ring -> change value
        if (ring == TMUtils::GetRingNumber(i))
        {
            fElementNewValue[i]->SetNumber(value);
        }
    }
}

//______________________________________________________________________________
void TMHWConfigModule::ClearValues()
{
    // Set all values in the element array displays and entries to 0.
    
    // clear title
    fTableTitle->SetText("Table: none");
    
    // unset table name variable
    fCurrentTable[0] = '\0';

    // clear values
    for (UInt_t i = 0; i < gMaxSize; i++)
    {   
        fElementCurrentValue[i]->SetText("0");
        fElementNewValue[i]->SetNumber(0);
        fElementValueChanged[i]->SetText("       ");
        fElementValueChanged[i]->ChangeBackground(fTableFrame->GetBackground());
    }
}

//______________________________________________________________________________
void TMHWConfigModule::MarkChanges()
{
    // Mark the elements if their new value differs from the current one.
    
    Double_t currValue;

    // loop over all elements
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        currValue = atof(fElementCurrentValue[i]->GetText()->Data());

        if (currValue != fElementNewValue[i]->GetNumber())
        {
            fElementValueChanged[i]->SetText("  YES  ");
            fElementValueChanged[i]->ChangeBackground(TColor::RGB2Pixel(255,0,0));
        }
        else 
        {
            fElementValueChanged[i]->SetText("       ");
            fElementValueChanged[i]->ChangeBackground(fTableFrame->GetBackground());
        }
    }
}

//______________________________________________________________________________
void TMHWConfigModule::DoRangeManipulation()
{
    // Change the values of the selected specific range of elements.

    // get selected range and value
    Int_t range = fRangeManipCombo->GetSelected();
    Double_t value = fRangeManipEntry->GetNumber();

    if (range == ERange_All_Elements) for (UInt_t i = 0; i < gMaxSize; i++) fElementNewValue[i]->SetNumber(value);
    else if (range == ERange_Block_A) SetBlockValues(1, value);
    else if (range == ERange_Block_B) SetBlockValues(2, value);
    else if (range == ERange_Block_C) SetBlockValues(3, value);
    else if (range == ERange_Block_D) SetBlockValues(4, value);
    else if (range == ERange_Block_E) SetBlockValues(5, value);
    else if (range == ERange_Block_F) SetBlockValues(6, value);
    else if (range == ERange_Ring_1) SetRingValues(1, value);
    else if (range == ERange_Ring_2) SetRingValues(2, value);
    else if (range == ERange_Ring_3) SetRingValues(3, value);
    else if (range == ERange_Ring_4) SetRingValues(4, value);
    else if (range == ERange_Ring_5) SetRingValues(5, value);
    else if (range == ERange_Ring_6) SetRingValues(6, value);
    else if (range == ERange_Ring_7) SetRingValues(7, value);
    else if (range == ERange_Ring_8) SetRingValues(8, value);
    else if (range == ERange_Ring_9) SetRingValues(9, value);
    else if (range == ERange_Ring_10) SetRingValues(10, value);
    else if (range == ERange_Ring_11) SetRingValues(11, value);

    MarkChanges();
}

//______________________________________________________________________________
Bool_t TMHWConfigModule::SetTableSettings(EDB_TAPS_Table table, Char_t* tableName, Char_t* columnName)
{
    // Set the table name of the TAPS table 'table' in tableName and the column
    // name of the data column in columnName.
    // Return kTRUE on success.
  
    if (table == EDB_Table_BaF2_HV) 
    {
        strcpy(tableName, "hvbaf_par");
        strcpy(columnName, "th");
        return kTRUE;
    }
    else if (table == EDB_Table_BaF2_CFD)
    {
        strcpy(tableName, "cfd_par");
        strcpy(columnName, "th");
        return kTRUE;
    }
    else if (table == EDB_Table_BaF2_LED1)
    {
        strcpy(tableName, "ledl_par");
        strcpy(columnName, "th");
        return kTRUE;
    }
    else if (table == EDB_Table_BaF2_LED2)
    {
        strcpy(tableName, "ledh_par");
        strcpy(columnName, "th");
        return kTRUE;
    }
    else if (table == EDB_Table_Veto_LED)
    {
        strcpy(tableName, "vled_par");
        strcpy(columnName, "th");
        return kTRUE;
    }
    else return kFALSE;
}

//______________________________________________________________________________
void TMHWConfigModule::ReadTable(Int_t table)
{
    // Read table from database and display the content.

    Char_t columnName[256];
    Char_t query[256];
    Char_t name[256];
    Double_t val;
    Int_t badEntries = 0;
    Char_t badEntriesID[768];
    badEntriesID[0] = '\0';


    // Clear current displayed values
    ClearValues();
    
    // activate BaF2 HV functions
    if (table == EDB_Table_BaF2_HV) 
    {
        fWriteHWButton->SetEnabled(kTRUE);
        fSettingsTab->SetEnabled(3, kTRUE);
    }
    else 
    {
        fWriteHWButton->SetEnabled(kFALSE);
        fSettingsTab->SetEnabled(3, kFALSE);
    }

    // Leave if the dummy entry in the combo was selected
    if (table == EDB_Table_Empty) return;

    // try to open connection to MySQL server
    TSQLServer* tapsDB = TSQLServer::Connect(fDBURLEntry->GetText(), 
                                             fDBUserEntry->GetText(), 
                                             fDBPasswdEntry->GetText());
    
    // exit if connection to DB failed
    if (!tapsDB)
    {
        fTableCombo->Select(EDB_Table_Empty, kTRUE);
        printf("ERROR: Could not connect to the database server. Please check your settings!\n");
        return;
    }
    
    // Choose table and set name of the column containing the
    // data values
    if (!SetTableSettings((EDB_TAPS_Table)table, fCurrentTable, columnName)) return;

    // display table info
    printf("Reading values from table %s ...\n", fCurrentTable);
    sprintf(name, "Table: %s", fCurrentTable);
    fTableTitle->SetText(name);
    
    // read values
    for (UInt_t i = 0; i < gMaxSize; i++)
    {   
        sprintf(query, "SELECT %s FROM %s WHERE id=%d", columnName, fCurrentTable, i+1);
        TSQLResult* res = tapsDB->Query(query);
        
        // check if multiple values were found
        if (res->GetRowCount() > 1) 
        {
            Char_t tmp[4];

            // increment counter and add bad ID to list
            badEntries++;
            if (badEntries == 1) sprintf(tmp, "%d", i+1);
            else sprintf(tmp, ", %d", i+1);
            strcat(badEntriesID, tmp);
        }
  
        // get the value
        val = atof(res->Next()->GetField(0));

        // set current and new value
        Char_t value[128];
        sprintf(value, "%.f", val);
        fElementCurrentValue[i]->SetText(value);
        fElementNewValue[i]->SetNumber(val);

        delete res;
    }

    // deconnect from server
    delete tapsDB;

    // create SQL error if necessary
    if (badEntries)
    {
        printf("\nWARNING: Broken SQL table!\n"
               "         For the following ID(s) more than 1 values were found in table '%s':\n"
               "         -> %s\n"
               "         Only the last value of every ID is read by TAPSMaintain!\n"
               "         ATTENTION: THESE VALUE WILL NOT BE UPDATED BY TAPSMaintain!\n\n"
               , fCurrentTable, badEntriesID);
    }
}

//______________________________________________________________________________
void TMHWConfigModule::WriteTable()
{
    // Write the values back to the table in the database. 
    
    Char_t columnName[256];
    Char_t query[256];
    Int_t table = fTableCombo->GetSelected();

    // Leave if the dummy entry in the combo was selected
    if (table == EDB_Table_Empty) return;
 
    // ask user for confirmation
    ModuleQuestion("Are you REALLY sure you want to write the new values to the DB?");
    if (GetDialogReturnValue() == kMBNo) return;

    // try to open connection to MySQL server
    TSQLServer* tapsDB = TSQLServer::Connect(fDBURLEntry->GetText(), 
                                             fDBUserEntry->GetText(), 
                                             fDBPasswdEntry->GetText());
    
    // exit if connection to DB failed
    if (!tapsDB)
    {
        printf("ERROR: Could not connect to the database server. Please check your settings!\n");
        return;
    }
    
    // Choose table and set name of the column containing the
    // data values
    if (!SetTableSettings((EDB_TAPS_Table)table, fCurrentTable, columnName)) return;

    // display table info
    printf("Updating values in table %s ...\n", fCurrentTable);
    
    // write values
    for (UInt_t i = 0; i < gMaxSize; i++)
    {   
        sprintf(query, "UPDATE %s SET %s=%f WHERE id=%d", fCurrentTable, columnName, 
                fElementNewValue[i]->GetNumber(), i+1);
        TSQLResult* res = tapsDB->Query(query);
        
        delete res;
    }

    // deconnect from server
    delete tapsDB;

    // read new table
    ReadTable(table);
}

//______________________________________________________________________________
void TMHWConfigModule::WriteHVToHardware()
{
    // Write the current values to the hardware HV crate. 
    
    // check value range 0..2000 V
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        Double_t val = atof(fElementCurrentValue[i]->GetText()->Data());
        if (val < 0 || val > 2000) return;
    }

    // ask user for confirmation
    ModuleQuestion("Are you sure you want to write the current values\nsaved in the database to the hardware?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // ask user for confirmation a second time
    ModuleQuestion("Are you REALLY sure?");
    if (GetDialogReturnValue() == kMBNo) return;

    /* Commented method of setting the HV using Set_BAF2_HV. Use Init_BAF2_HV instead
    // write all values
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        Int_t val = atoi(fElementCurrentValue[i]->GetText()->Data());
        Set_BAF2_HV((char*)gTAPS_Server, i+1, val);

        fProgressBar->SetPosition(i+1); 
        gSystem->ProcessEvents();
    }

    // reset progress bar
    fProgressBar->Reset();
    */

    // call BaF2 init method
    Init_BAF2_HV((char*)gTAPS_Server);

    ModuleInfo("The command for the initialization of the BaF2 HV was sent to the\n"
               "TAPS hardware server. Please wait until all 544 elements were\n"
               "initialized. This could take some time!");
}

