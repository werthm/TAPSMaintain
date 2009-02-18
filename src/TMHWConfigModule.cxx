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
    fControlFrame->SetLayoutManager(new TGTableLayout(fControlFrame, 7, 2));
    
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
    fRangeManipEntry->SetLimits(TGNumberFormat::kNELLimitMinMax);
    fRangeManipEntry->SetLimitValues(0., 0.);
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
    fImportBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectFile(=1)");
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
    fExportBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectFile(=2)");
    fExportFrame->AddFrame(fExportBrowse, 
                           new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

    fExportColumn = new TGCheckButton(fExportFrame, "Use new values instead of current ones");
    fExportFrame->AddFrame(fExportColumn, 
                           new TGTableLayoutHints(0, 3, 2, 3, kLHintsFillX | kLHintsLeft, 15, 5, 8, 2));

    fExportButton = new TGTextButton(fExportFrame, " Export ");
    fExportButton->Connect("Clicked()", "TMHWConfigModule", this, "ExportFile()");
    fExportFrame->AddFrame(fExportButton, 
                           new TGTableLayoutHints(3, 4, 2, 3, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));

    l = new TGLabel(fExportFrame, "Export all current or new values into the specified file. The format is 2  \n"
                                  "column ELEMENT ID and VALUE space separated\n");
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
    fGMCalibBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectFile(=3)");
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
    fLEDFrame->SetLayoutManager(new TGTableLayout(fLEDFrame, 9, 4));
    
    l = new TGLabel(fLEDFrame, "Energy calibration file:");
    l->SetTextJustify(kTextLeft);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
    
    fLEDEnergyCalibFileEntry = new TGTextEntry(fLEDFrame, "");
    fLEDEnergyCalibFileEntry->Resize(200, 22);
    fLEDFrame->AddFrame(fLEDEnergyCalibFileEntry,
                       new TGTableLayoutHints(0, 3, 1, 2, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));
    
    fLEDEnergyCalibBrowse = new TGTextButton(fLEDFrame, " Browse ");
    fLEDEnergyCalibBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectFile(=4)");
    fLEDFrame->AddFrame(fLEDEnergyCalibBrowse, 
                       new TGTableLayoutHints(3, 4, 1, 2, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    l = new TGLabel(fLEDFrame, "LED calibration file:");
    l->SetTextJustify(kTextLeft);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX | kLHintsLeft, 15, 5, 15, 5));
    
    fLEDCalibFileEntry = new TGTextEntry(fLEDFrame, "");
    fLEDCalibFileEntry->Resize(200, 22);
    fLEDFrame->AddFrame(fLEDCalibFileEntry,
                       new TGTableLayoutHints(0, 3, 3, 4, kLHintsFillX | kLHintsLeft, 15, 5, 5, 5));
    
    fLEDCalibBrowse = new TGTextButton(fLEDFrame, " Browse ");
    fLEDCalibBrowse->Connect("Clicked()", "TMHWConfigModule", this, "SelectFile(=5)");
    fLEDFrame->AddFrame(fLEDCalibBrowse, 
                       new TGTableLayoutHints(3, 4, 3, 4, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    fLEDAddFileButton = new TGTextButton(fLEDFrame, " Add ");
    fLEDAddFileButton->Connect("Clicked()", "TMHWConfigModule", this, "AddFileToLEDCalibration()");
    fLEDFrame->AddFrame(fLEDAddFileButton, 
                       new TGTableLayoutHints(3, 4, 4, 5, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    
    fLEDFrame->AddFrame(new TGHorizontal3DLine(fLEDFrame), 
                        new TGTableLayoutHints(0, 4, 5, 6, kLHintsFillX, 15, 15, 15, 15));
                        
    l = new TGLabel(fLEDFrame, "Choose");
    l->SetTextJustify(kTextRight);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 6, 7, kLHintsFillX | kLHintsLeft, 5, 15, 8, 2));
    
    fLEDRangeCombo = new TGComboBox(fLEDFrame);
    fLEDRangeCombo->Resize(150, 22);
    fLEDRangeCombo->Connect("Selected(Int_t)", "TMHWConfigModule", this, "LEDRangeChanged(Int_t)");
    fLEDRangeCombo->AddEntry("Single element", ERange_Single_Element);
    fLEDRangeCombo->AddEntry("All elements", ERange_All_Elements);
    fLEDRangeCombo->AddEntry("Block A elements", ERange_Block_A);
    fLEDRangeCombo->AddEntry("Block B elements", ERange_Block_B);
    fLEDRangeCombo->AddEntry("Block C elements", ERange_Block_C);
    fLEDRangeCombo->AddEntry("Block D elements", ERange_Block_D);
    fLEDRangeCombo->AddEntry("Block E elements", ERange_Block_E);
    fLEDRangeCombo->AddEntry("Block F elements", ERange_Block_F);
    fLEDRangeCombo->AddEntry("Ring 1 elements", ERange_Ring_1);
    fLEDRangeCombo->AddEntry("Ring 2 elements", ERange_Ring_2);
    fLEDRangeCombo->AddEntry("Ring 3 elements", ERange_Ring_3);
    fLEDRangeCombo->AddEntry("Ring 4 elements", ERange_Ring_4);
    fLEDRangeCombo->AddEntry("Ring 5 elements", ERange_Ring_5);
    fLEDRangeCombo->AddEntry("Ring 6 elements", ERange_Ring_6);
    fLEDRangeCombo->AddEntry("Ring 7 elements", ERange_Ring_7);
    fLEDRangeCombo->AddEntry("Ring 8 elements", ERange_Ring_8);
    fLEDRangeCombo->AddEntry("Ring 9 elements", ERange_Ring_9);
    fLEDRangeCombo->AddEntry("Ring 10 elements", ERange_Ring_10);
    fLEDRangeCombo->AddEntry("Ring 11 elements", ERange_Ring_11);
    fLEDFrame->AddFrame(fLEDRangeCombo, new TGTableLayoutHints(1, 2, 6, 7, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    fLEDRangeCombo->Select(ERange_Single_Element, kFALSE);
    
    
    fLEDChannelEntry = new TGNumberEntry(fLEDFrame, 1, 4);
    fLEDFrame->AddFrame(fLEDChannelEntry,
                        new TGTableLayoutHints(2, 3, 6, 7, kLHintsLeft, 5, 15, 5, 5));

    fLEDShowChannelButton = new TGTextButton(fLEDFrame, " Show ");
    fLEDShowChannelButton->Connect("Clicked()", "TMHWConfigModule", this, "ShowLEDCalibration()");
    fLEDFrame->AddFrame(fLEDShowChannelButton, 
                        new TGTableLayoutHints(3, 4, 6, 7, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
              
    l = new TGLabel(fLEDFrame, "Set threshold of range to [MeV]:");
    l->SetTextJustify(kTextRight);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 2, 7, 8, kLHintsFillX | kLHintsLeft, 5, 15, 8, 2));                           
                                            
    fLEDThrEntry = new TGNumberEntry(fLEDFrame, 30, 4);
    fLEDFrame->AddFrame(fLEDThrEntry,
                        new TGTableLayoutHints(2, 3, 7, 8, kLHintsLeft, 5, 15, 5, 5));    
                        
    fLEDSetThrButton = new TGTextButton(fLEDFrame, " Set ");
    fLEDSetThrButton->Connect("Clicked()", "TMHWConfigModule", this, "SetLEDThresholds()");
    fLEDFrame->AddFrame(fLEDSetThrButton, 
                        new TGTableLayoutHints(3, 4, 7, 8, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));                                                                                 
                                                                                                                                                                                                                
    
    l = new TGLabel(fLEDFrame, "Open several LED calibration files and add them to the LED setting\n"
                               "calculation. A linear fit is performed for every TAPS channel using\n"
                               "the points of the calibration files.");
    l->SetTextJustify(kTextLeft);
    fLEDFrame->AddFrame(l, new TGTableLayoutHints(0, 4, 8, 9, kLHintsFillX | kLHintsLeft, 15, 15, 15, 5));
    
                       
    // ------------------------------ End Tab ------------------------------
    fControlFrame->AddFrame(fSettingsTab,  new TGTableLayoutHints(0, 2, 4, 5, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));
    fSettingsTab->SetEnabled(3, kFALSE);
    fSettingsTab->SetEnabled(4, kFALSE);



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
    fProgressBar = new TGHProgressBar(fControlFrame, TGProgressBar::kFancy, 10);
    fProgressBar->SetBarColor("green");
    fProgressBar->Resize(200, 25);
    fProgressBar->SetMin(0);
    fProgressBar->SetMax(gMaxSize);
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Nothing to do");
    fControlFrame->AddFrame(fProgressBar,  new TGTableLayoutHints(0, 2, 6, 7, kLHintsFillX | kLHintsFillY | kLHintsLeft, 5, 5, 35, 5));

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
        fElementNewValue[i]->SetLimits(TGNumberFormat::kNELLimitMinMax);
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
void TMHWConfigModule::Cleanup()
{
    // Cleanup memory.
    
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
    fLEDEnergyCalibFileEntry->SetText("");
    
    // enter default values in entries
    fLEDThrEntry->SetNumber(30);
    fLEDChannelEntry->SetNumber(1);

    // select first tab
    fSettingsTab->SetTab(0, kFALSE);

    // select default entry in combos
    fTableCombo->Select(EDB_Table_Empty, kTRUE);
    fRangeManipCombo->Select(ERange_All_Elements, kFALSE);
    fLEDRangeCombo->Select(ERange_Single_Element, kFALSE);
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
void TMHWConfigModule::LEDRangeChanged(Int_t id)
{
    // Update the GUI after the user has changed the range of the LED 
    // elements in the combo box.
    
    if (id == ERange_Single_Element) fLEDChannelEntry->SetState(kTRUE);
    else fLEDChannelEntry->SetState(kFALSE);
}

//______________________________________________________________________________
void TMHWConfigModule::SelectFile(Int_t ftype)
{
    // Let the user select a file. The file name will be used depending on
    // the ftype argument.
    
    Char_t filename[256];

    // Let TAPSMaintain open the "Save file" or "Open file" dialog
    if (ftype == EFILE_SELECT_EXPORT) ShowFileDialog(kFDSave);
    else ShowFileDialog(kFDOpen);

    // copy file name
    GetAndDeleteMiscFileName(filename);
  
    // abort if file name is empty
    if (!strcmp(filename, "")) return;
    
    // Check if it's a directory
    Long_t id, flags, modtime;
    Long64_t size;
    gSystem->GetPathInfo(filename, &id, &size, &flags, &modtime);
    
    // Use the selected file name
    if (ftype == EFILE_SELECT_EXPORT)
    {
        fExportFileEntry->SetText(filename);
    }
    else if (ftype == EFILE_SELECT_IMPORT)
    {
        if (flags == 0 || flags == 1) fImportFileEntry->SetText(filename);
    }
    else if (ftype == EFILE_SELECT_GM_ENERGY_CALIB)
    {
        if (flags == 0 || flags == 1) fGMCalibFileEntry->SetText(filename);
    }
    else if (ftype == EFILE_SELECT_LED_CALIB)
    {
        if (flags == 0 || flags == 1) fLEDCalibFileEntry->SetText(filename);
    }
    else if (ftype == EFILE_SELECT_LED_ENERGY_CALIB)
    {
        if (flags == 0 || flags == 1) fLEDEnergyCalibFileEntry->SetText(filename);
    }
    else return;
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

    // get the selected table
    Int_t table = fTableCombo->GetSelected();
    
    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;

    // leave if no table is selected
    if (table == EDB_Table_Empty) return;

    // open the file
    FILE* fin;
    fin = fopen(filename, "r");
 
    // read file and set values
    while (!feof(fin))
    {
        if (!fgets(line, 256, fin) && !feof(fin)) Error("ImportFile", "File read error!");

        // check if line is a comment
        if (TMUtils::IsComment(line)) continue;

        if (sscanf(line, "%d%f", &id, &value) == 2)
        {
            if (CheckValueLimits((EDB_TAPS_Table)table, value)) fElementNewValue[id-1]->SetNumber(value);
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
        if (!fgets(line, 256, fin) && !feof(fin)) Error("DoGainMatch", "File read error!");

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

            // check limits and set new HV value
            if (hv_new < gDB_BaF2_HV_Min) fElementNewValue[id-1]->SetNumber((Int_t)gDB_BaF2_HV_Min);
            else if (hv_new > gDB_BaF2_HV_Max) fElementNewValue[id-1]->SetNumber((Int_t)gDB_BaF2_HV_Max);
            else fElementNewValue[id-1]->SetNumber((Int_t)hv_new);
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
    if (fExportColumn->IsOn()) fprintf(fout, "# TAPSMaintain value export\n");
    else
    {
        fprintf(fout, "# TAPSMaintain database table export\n");
        fprintf(fout, "# Table: %s\n", fCurrentTable);
    }
    fprintf(fout, "\n");
    fprintf(fout, "# ID    Value\n");
    
    // write current values to file
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        if (fExportColumn->IsOn()) fprintf(fout, "%3d    %d\n", i+1, (Int_t)fElementNewValue[i]->GetNumber());
        else fprintf(fout, "%3d    %s\n", i+1, fElementCurrentValue[i]->GetText()->Data());
    }

    // close the file
    fclose(fout);

    // show info message
    Char_t msg[256];
    if (fExportColumn->IsOn()) sprintf(msg, "New values were saved to '%s' .", filename);
    else sprintf(msg, "Current values of table '%s' were saved to '%s' .", fCurrentTable, filename);
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
    Double_t energyCalibPed[gMaxSize];
    Double_t energyCalibGain[gMaxSize];
    FILE* fin;

    // get the selected file name of the LED calib file
    const Char_t* filename = fLEDCalibFileEntry->GetText();

    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;
     
    // check for energy calibration
    const Char_t* ecalfile = fLEDEnergyCalibFileEntry->GetText();
    if (!strcmp(ecalfile, ""))
    {
        ModuleError("Please load an energy calibration file before adding LED calibration files!");
        return;
    }
    else
    {
        Int_t id;
        Float_t ped, peak, gain;
        
        // open the energy calibration file
        fin = fopen(ecalfile, "r");
    
        // read voltages and threshold from file
        while (!feof(fin))
        {
            if (!fgets(line, 256, fin) && !feof(fin)) Error("AddFileToLEDCalibration", "File read error!");

            // check if line is a comment
            if (TMUtils::IsComment(line)) continue;
        
            // read id, pedestal pos., cosmic peak pos. and gain
            if (sscanf(line, "%d%f%f%f", &id, &ped, &peak, &gain) == 4)
            {
                energyCalibPed[id-1] = ped;
                energyCalibGain[id-1] = gain;
            }
        }
        fclose(fin);
    }
    
    // clear file input entry
    fLEDCalibFileEntry->SetText("");
    
    // init arrays
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        thresholds[i] = 0;
        voltages[i] = 0;
    }
   
    // open the LED calibration file
    fin = fopen(filename, "r");
    
    // read voltages and threshold from file
    while (!feof(fin))
    {
        if (!fgets(line, 256, fin) && !feof(fin)) Error("AddFileToLEDCalibration", "File read error!");

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
            fLEDFitFunctions[i] = new TF1(line, "pol1", 0, 2000);
            fLEDFitFunctions[i]->SetLineColor(kRed);
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
            fLEDGraphs[i]->SetPoint(0, energyCalibGain[i] * (thresholds[i] - energyCalibPed[i]), voltages[i]);
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
            
            // add new point
            fLEDGraphs[i]->SetPoint(fNLEDCalibSets, energyCalibGain[i] * (thresholds[i] - energyCalibPed[i]), voltages[i]);
            
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
void TMHWConfigModule::SetLEDThresholds()
{
    // Set the LED thresholds of the specified channels to the requested value
    // using the loaded calibration.
    
    // check if calibration was loaded
    if (!fNLEDCalibSets) return;
    
    // get selected LED range
    Int_t selectedLEDRange = fLEDRangeCombo->GetSelected();
    
    // get requested value
    Double_t reqValue = fLEDThrEntry->GetNumber();

    if (selectedLEDRange == ERange_Single_Element)
    {
        // selected channel
        Int_t id = (UInt_t) fLEDChannelEntry->GetNumber();
        
        // check limits and set value 
        Int_t resValue = (Int_t)fLEDFitFunctions[id-1]->Eval(reqValue);
        if (resValue < gDB_BaF2_LED_Min) fElementNewValue[id-1]->SetNumber(gDB_BaF2_LED_Min);
        else if (resValue > gDB_BaF2_LED_Max) fElementNewValue[id-1]->SetNumber(gDB_BaF2_LED_Max);
        else fElementNewValue[id-1]->SetNumber(resValue);
    }
    else if (selectedLEDRange == ERange_All_Elements)
    {
        // loop over all elements
        for (UInt_t i = 0; i < gMaxSize; i++)
        {
            Int_t resValue = (Int_t)fLEDFitFunctions[i]->Eval(reqValue);
            if (resValue < gDB_BaF2_LED_Min) fElementNewValue[i]->SetNumber(gDB_BaF2_LED_Min);
            else if (resValue > gDB_BaF2_LED_Max) fElementNewValue[i]->SetNumber(gDB_BaF2_LED_Max);
            else fElementNewValue[i]->SetNumber(resValue);
        }
    }
    else
    {
        // set dummy ring number
        UInt_t ring = 99;
        
        // set ring numbers if ring range was selected
        if (selectedLEDRange == ERange_Ring_1) ring = 1;
        if (selectedLEDRange == ERange_Ring_2) ring = 2;
        if (selectedLEDRange == ERange_Ring_3) ring = 3;
        if (selectedLEDRange == ERange_Ring_4) ring = 4;
        if (selectedLEDRange == ERange_Ring_5) ring = 5;
        if (selectedLEDRange == ERange_Ring_6) ring = 6;
        if (selectedLEDRange == ERange_Ring_7) ring = 7;
        if (selectedLEDRange == ERange_Ring_8) ring = 8;
        if (selectedLEDRange == ERange_Ring_9) ring = 9;
        if (selectedLEDRange == ERange_Ring_10) ring = 10;
        if (selectedLEDRange == ERange_Ring_11) ring = 11;
        
        // leave if no ring range was selected
        if (ring == 99) return;
        
        // loop over all elements and update members of selected ring
        for (UInt_t i = 0; i < gMaxSize; i++)
        {
            // element belongs to specified ring -> change value
            if (ring == TMUtils::GetRingNumber(i))
            {
                Int_t resValue = (Int_t)fLEDFitFunctions[i]->Eval(reqValue);
                if (resValue < gDB_BaF2_LED_Min) fElementNewValue[i]->SetNumber(gDB_BaF2_LED_Min);
                else if (resValue > gDB_BaF2_LED_Max) fElementNewValue[i]->SetNumber(gDB_BaF2_LED_Max);
                else fElementNewValue[i]->SetNumber(resValue);
            }
        }
    }
    
    MarkChanges();
}

//______________________________________________________________________________
void TMHWConfigModule::ShowLEDCalibration()
{
    // Show the LED calibration graphs including the fit function of a specific
    // TAPS channel.
    
    // check if calibration was loaded
    if (!fNLEDCalibSets) return;
    
    // get selected LED range
    Int_t selectedLEDRange = fLEDRangeCombo->GetSelected();
    
    if (selectedLEDRange == ERange_Single_Element)
    {
        // selected channel
        UInt_t id = (UInt_t) fLEDChannelEntry->GetNumber();
    
        if (id > 0 && id <= gMaxSize)
        {
            TText aText;
            Char_t text[256];
            aText.SetTextSize(0.05);
            aText.SetNDC();
        
            CreateExternalCanvas(1);
            fLEDGraphs[id-1]->SetMarkerStyle(20);
            fLEDGraphs[id-1]->Draw("ap");
            sprintf(text, "LED Calibration: Channel %d", id);
            fLEDGraphs[id-1]->SetTitle(text);
            fLEDGraphs[id-1]->GetXaxis()->SetTitle("Threshold Energy [Mev]");
            fLEDGraphs[id-1]->GetYaxis()->SetTitle("Applied Voltage [mV]");
        
            sprintf(text, "constant: %f", fLEDFitFunctions[id-1]->GetParameter(0));
            aText.DrawText(0.5, 0.8, text);
            sprintf(text, "slope: %f", fLEDFitFunctions[id-1]->GetParameter(1));
            aText.DrawText(0.5, 0.75, text);
    
            //fExternalCanvas->Update();
        }
    }
    else
    {
        Char_t text[256];
        
        // set dummy start value
        Int_t start = -99;
        
        // set start value for blocks
        if (selectedLEDRange == ERange_Block_A) start = 0;
        if (selectedLEDRange == ERange_Block_B) start = 64;
        if (selectedLEDRange == ERange_Block_C) start = 128;
        if (selectedLEDRange == ERange_Block_D) start = 192;
        if (selectedLEDRange == ERange_Block_E) start = 256;
        if (selectedLEDRange == ERange_Block_F) start = 320;
        
        // leave if no block was selected
        if (start == -99) return;
        
        // create block canvas
        CreateExternalCanvas(64);
        
        // draw block elements
        for (Int_t i = start; i < start + 64; i++)
        {
            TVirtualPad* aPad = fExternalCanvas->cd(i-start+1);
            
            aPad->SetLeftMargin(0.11);
            aPad->SetRightMargin(0.08);
            aPad->SetTopMargin(0.08);
            aPad->SetBottomMargin(0.1);
            
            fLEDGraphs[i]->SetMarkerStyle(20);
            fLEDGraphs[i]->Draw("ap"); 
            sprintf(text, "LED Calibration: Channel %d", i+1);
            fLEDGraphs[i]->SetTitle(text);
        }
        
        //fExternalCanvas->Update();
    }
}

//______________________________________________________________________________
void TMHWConfigModule::CreateExternalCanvas(Int_t n)
{
    // Create the external canvas if necessary. Divide for n elements.
    
    // destroy old canvas
    if (fExternalCanvas) delete fExternalCanvas;
    
    // create new canvas
    fExternalCanvas = new TCanvas();
    fExternalCanvas->Connect("Closed()", "TMHWConfigModule", this, "DestroyExternalCanvas()");
    
    // divide canvas
    if (n == 1) fExternalCanvas->Divide(1, 1);
    else if (n == 64) fExternalCanvas->Divide(8, 8, 0.001, 0.001);
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
    
    // get the selected table
    Int_t table = fTableCombo->GetSelected();
 
    // Set values
    for (UInt_t i = blockRange[block-1][0]; i < blockRange[block-1][1]; i++) 
    {
        if (CheckValueLimits((EDB_TAPS_Table)table, value)) fElementNewValue[i]->SetNumber(value);
    }
}

//______________________________________________________________________________
void TMHWConfigModule::SetRingValues(UInt_t ring, Double_t value)
{
    // Set all elements of the ring 'ring' to the value 'value' (inner ring = 1,
    // 2nd ring = 2, ...)

    // check bounds
    if (ring > 11) return;
    
    // get the selected table
    Int_t table = fTableCombo->GetSelected();
 
    // loop over all elements and change value if element belongs to the
    // specified ring
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        // element belongs to specified ring -> change value
        if (ring == TMUtils::GetRingNumber(i))
        {
            if (CheckValueLimits((EDB_TAPS_Table)table, value)) fElementNewValue[i]->SetNumber(value);
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
        fElementNewValue[i]->SetNumber(0.);
        fElementNewValue[i]->SetLimitValues(0., 0.);
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
    
    // get the selected table
    Int_t table = fTableCombo->GetSelected();
 
    // leave if no table is selected
    if (table == EDB_Table_Empty) return;

    if (range == ERange_All_Elements) 
    {
        // check limits
        if (CheckValueLimits((EDB_TAPS_Table)table, value)) 
            for (UInt_t i = 0; i < gMaxSize; i++) fElementNewValue[i]->SetNumber(value);
    }
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
Bool_t TMHWConfigModule::CheckValueLimits(EDB_TAPS_Table table, Double_t value)
{
    // Check if the value 'value' lies within the limits of the accepted values
    // for the table 'table'.

    if (table == EDB_Table_BaF2_HV)
    {
        if (value >= gDB_BaF2_HV_Min && value <= gDB_BaF2_HV_Max) return kTRUE;
        else return kFALSE;
    }
    if (table == EDB_Table_BaF2_CFD)
    {
        if (value >= gDB_BaF2_CFD_Min && value <= gDB_BaF2_CFD_Max) return kTRUE;
        else return kFALSE;
    }
    if (table == EDB_Table_BaF2_LED1 || EDB_Table_BaF2_LED2)
    {
        if (value >= gDB_BaF2_LED_Min && value <= gDB_BaF2_LED_Max) return kTRUE;
        else return kFALSE;
    }
    if (table == EDB_Table_Veto_LED)
    {
        if (value >= gDB_Veto_LED_Min && value <= gDB_Veto_LED_Max) return kTRUE;
        else return kFALSE;
    }

    return kFALSE;
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
    
    // de-/activate GUI elements depending on the loaded table 
    if (table == EDB_Table_BaF2_HV) 
    {
        fWriteHWButton->SetEnabled(kTRUE);
        fSettingsTab->SetEnabled(3, kTRUE);
        fSettingsTab->SetEnabled(4, kFALSE);
    }
    else if (table == EDB_Table_BaF2_LED1 || table == EDB_Table_BaF2_LED2)
    {
        fSettingsTab->SetEnabled(3, kFALSE);
        fSettingsTab->SetEnabled(4, kTRUE);
    }
    else 
    {
        fWriteHWButton->SetEnabled(kFALSE);
        fSettingsTab->SetEnabled(3, kFALSE);
        fSettingsTab->SetEnabled(4, kFALSE);
    }

    // set standard tab
    fSettingsTab->SetTab(0, kFALSE);
    
    // set range manipulation number entry limits
    if (table == EDB_Table_Empty) fRangeManipEntry->SetLimitValues(0., 0.);
    else if (table == EDB_Table_BaF2_HV) fRangeManipEntry->SetLimitValues(gDB_BaF2_HV_Min, gDB_BaF2_HV_Max);
    else if (table == EDB_Table_BaF2_CFD) fRangeManipEntry->SetLimitValues(gDB_BaF2_CFD_Min, gDB_BaF2_CFD_Max);
    else if (table == EDB_Table_BaF2_LED1) fRangeManipEntry->SetLimitValues(gDB_BaF2_LED_Min, gDB_BaF2_LED_Max);
    else if (table == EDB_Table_BaF2_LED2) fRangeManipEntry->SetLimitValues(gDB_BaF2_LED_Min, gDB_BaF2_LED_Max);
    else if (table == EDB_Table_Veto_LED) fRangeManipEntry->SetLimitValues(gDB_Veto_LED_Min, gDB_Veto_LED_Max);

    
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
        
        // set limits of new value entries
        if (table == EDB_Table_BaF2_HV) fElementNewValue[i]->SetLimitValues(gDB_BaF2_HV_Min, gDB_BaF2_HV_Max);
        else if (table == EDB_Table_BaF2_CFD) fElementNewValue[i]->SetLimitValues(gDB_BaF2_CFD_Min, gDB_BaF2_CFD_Max);
        else if (table == EDB_Table_BaF2_LED1) fElementNewValue[i]->SetLimitValues(gDB_BaF2_LED_Min, gDB_BaF2_LED_Max);
        else if (table == EDB_Table_BaF2_LED2) fElementNewValue[i]->SetLimitValues(gDB_BaF2_LED_Min, gDB_BaF2_LED_Max);
        else if (table == EDB_Table_Veto_LED) fElementNewValue[i]->SetLimitValues(gDB_Veto_LED_Min, gDB_Veto_LED_Max);

        delete res;
    }

    // deconnect from server
    delete tapsDB;

    // mark changes in case values from DB are out of the limits
    // of the number entries
    MarkChanges();

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

    // ask user to start HVServer
    ModuleInfo("Please check that '/taps/HVServer' is running on vme-9! Otherwise the values will not be uploaded!");

    // ask user for confirmation
    ModuleQuestion("Are you sure you want to write the current values\nsaved in the database to the hardware?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // ask user for confirmation a second time
    ModuleQuestion("Are you REALLY sure?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // format progress bar
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Uploading channel %.0f");

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
 
    // OLD CODE:
    // NOT USED AT THE MOMENT BECAUSE OF BAD STATUS MONITORING
    // call BaF2 init method
    //Init_BAF2_HV((char*)gTAPS_Server);
    //ModuleInfo("The command for the initialization of the BaF2 HV was sent to the\n"
    //           "TAPS hardware server. Please wait until all 544 elements were\n"
    //           "initialized. This could take some time!");
    
    // re-format progress bar
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Nothing to do");

    ModuleInfo("Upload completed!");
}

