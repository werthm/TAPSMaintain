// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
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
    fLEDFitFunctions = 0;
    fLEDGraphs = 0;
    fNLEDCalibSets = 0;
    fExternalCanvas = 0;


    fFrame->SetLayoutManager(new TGHorizontalLayout(fFrame));
    
    // ------------------------------ Control frame ------------------------------
    fControlFrame = new TGCompositeFrame(fFrame, 50, 50);
    fControlFrame->SetLayoutManager(new TGTableLayout(fControlFrame, 4, 2));
    
    // DB table
    TGLabel* l = new TGLabel(fControlFrame, "DB table:");
    l->SetTextJustify(kTextRight);
    fControlFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsLeft, 5, 5, 5, 5));
    
    // combo box
    fTableCombo = new TGComboBox(fControlFrame);
    fTableCombo->Connect("Selected(Int_t)", "TMHWConfigModule", this, "ReadTable(Int_t)");
    fTableCombo->Resize(200, 22);
    
    // create a list of all parameter data types and fill combo box (including first dummy entry)
    TList* lt = TTMySQLManager::GetManager()->GetDataTypes();
    fParTypes = new TList();
    Int_t npar = 0;
    fParTypes->Add(new TTDataTypePar());
    fTableCombo->AddEntry("Select TAPS table here", npar++);
    for (Int_t i = 0; i < lt->GetSize(); i++)
    {
        TTDataType* d = (TTDataType*) lt->At(i);
        if (d->GetType() == kParType)
        {
            TTDataTypePar* dp = (TTDataTypePar*) d;
            fParTypes->Add(dp);
            fTableCombo->AddEntry(dp->GetTitle(), npar++);
        }
    }
    fControlFrame->AddFrame(fTableCombo, new TGTableLayoutHints(1, 2, 0, 1, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));
    fTableCombo->Select(0, kFALSE);


    // ------------------------------ Begin Tab ------------------------------
    fSettingsTab = new TGTab(fControlFrame, 50, 50);   

    // ------------------------------ Value setting frame ------------------------------
    fSetValueFrame = fSettingsTab->AddTab("Set Values");
    fSetValueFrame->SetLayoutManager(new TGTableLayout(fSetValueFrame, 2, 1));
    
    fHexFormat = new TGCheckButton(fSetValueFrame, "Hexadecimal value input");
    fHexFormat->Connect("Clicked()", "TMHWConfigModule", this, "ToggleHexFormat()");
    fSetValueFrame->AddFrame(fHexFormat, new TGTableLayoutHints(0, 1, 0, 1, kLHintsLeft, 15, 5, 15, 5));
    
    // ------------------------------ Range manipulation frame ------------------------------
    fRangeManipFrame = new TGCompositeFrame(fSetValueFrame, 5, 5);
    fRangeManipFrame->SetLayoutManager(new TGHorizontalLayout(fRangeManipFrame));
  
    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "Set all"), new TGLayoutHints(kLHintsLeft, 15, 5, 35, 5));
    
    fRangeManipCombo = new TGComboBox(fRangeManipFrame);
    fRangeManipCombo->Resize(150, 22);
    fRangeManipCombo->AddEntry("elements", kRange_All_Elements);
    fRangeManipCombo->AddEntry("Ring 1 elements", kRange_Ring_1);
    fRangeManipCombo->AddEntry("Ring 2 elements", kRange_Ring_2);
    fRangeManipCombo->AddEntry("Ring 3 elements", kRange_Ring_3);
    fRangeManipCombo->AddEntry("Ring 4 elements", kRange_Ring_4);
    fRangeManipCombo->AddEntry("Ring 5 elements", kRange_Ring_5);
    fRangeManipCombo->AddEntry("Ring 6 elements", kRange_Ring_6);
    fRangeManipCombo->AddEntry("Ring 7 elements", kRange_Ring_7);
    fRangeManipCombo->AddEntry("Ring 8 elements", kRange_Ring_8);
    fRangeManipCombo->AddEntry("Ring 9 elements", kRange_Ring_9);
    fRangeManipCombo->AddEntry("Ring 10 elements", kRange_Ring_10);
    fRangeManipCombo->AddEntry("Ring 11 elements", kRange_Ring_11);
    fRangeManipCombo->AddEntry("Block A elements", kRange_Block_A);
    fRangeManipCombo->AddEntry("Block B elements", kRange_Block_B);
    fRangeManipCombo->AddEntry("Block C elements", kRange_Block_C);
    fRangeManipCombo->AddEntry("Block D elements", kRange_Block_D);
    fRangeManipCombo->AddEntry("Block E elements", kRange_Block_E);
    fRangeManipCombo->AddEntry("Block F elements", kRange_Block_F);
    fRangeManipFrame->AddFrame(fRangeManipCombo, new TGLayoutHints(kLHintsLeft, 7, 5, 32, 2));
    fRangeManipCombo->Select(kRange_All_Elements, kFALSE);

    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "to"), new TGLayoutHints(kLHintsLeft, 7, 5, 35, 5));
    
    fRangeManipEntry = new TGNumberEntry(fRangeManipFrame, 0, 8);
    fRangeManipEntry->SetLimits(TGNumberFormat::kNELLimitMinMax);
    fRangeManipEntry->SetLimitValues(0., 0.);
    fRangeManipFrame->AddFrame(fRangeManipEntry, new TGLayoutHints(kLHintsLeft, 7, 5, 32, 2));

    fRangeManipButton = new TGTextButton(fRangeManipFrame, "    Set    ");
    fRangeManipButton->Connect("Clicked()", "TMHWConfigModule", this, "DoRangeManipulation()");
    fRangeManipFrame->AddFrame(fRangeManipButton, new TGLayoutHints(kLHintsLeft, 7, 15, 32, 2));
    
    fSetValueFrame->AddFrame(fRangeManipFrame, new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX | kLHintsLeft, 0, 5, 5, 5));
    
    
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
    fLEDRangeCombo->AddEntry("Single element", kRange_Single_Element);
    fLEDRangeCombo->AddEntry("All elements", kRange_All_Elements);
    fLEDRangeCombo->AddEntry("Block A elements", kRange_Block_A);
    fLEDRangeCombo->AddEntry("Block B elements", kRange_Block_B);
    fLEDRangeCombo->AddEntry("Block C elements", kRange_Block_C);
    fLEDRangeCombo->AddEntry("Block D elements", kRange_Block_D);
    fLEDRangeCombo->AddEntry("Block E elements", kRange_Block_E);
    fLEDRangeCombo->AddEntry("Block F elements", kRange_Block_F);
    fLEDRangeCombo->AddEntry("Ring 1 elements", kRange_Ring_1);
    fLEDRangeCombo->AddEntry("Ring 2 elements", kRange_Ring_2);
    fLEDRangeCombo->AddEntry("Ring 3 elements", kRange_Ring_3);
    fLEDRangeCombo->AddEntry("Ring 4 elements", kRange_Ring_4);
    fLEDRangeCombo->AddEntry("Ring 5 elements", kRange_Ring_5);
    fLEDRangeCombo->AddEntry("Ring 6 elements", kRange_Ring_6);
    fLEDRangeCombo->AddEntry("Ring 7 elements", kRange_Ring_7);
    fLEDRangeCombo->AddEntry("Ring 8 elements", kRange_Ring_8);
    fLEDRangeCombo->AddEntry("Ring 9 elements", kRange_Ring_9);
    fLEDRangeCombo->AddEntry("Ring 10 elements", kRange_Ring_10);
    fLEDRangeCombo->AddEntry("Ring 11 elements", kRange_Ring_11);
    fLEDFrame->AddFrame(fLEDRangeCombo, new TGTableLayoutHints(1, 2, 6, 7, kLHintsFillX | kLHintsLeft, 5, 15, 5, 5));
    fLEDRangeCombo->Select(kRange_Single_Element, kFALSE);
    
    
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
    
    // ------------------------------ HV ctrl frame ------------------------------
    fHVCtrlFrame = fSettingsTab->AddTab("HV");
    fHVCtrlFrame->SetLayoutManager(new TGTableLayout(fHVCtrlFrame, 3, 3));
    
    fHVReadButton = new TGTextButton(fHVCtrlFrame, "Read HV status");
    fHVReadButton->SetTopMargin(15);
    fHVReadButton->SetBottomMargin(15);
    fHVReadButton->SetRightMargin(15);
    fHVReadButton->SetLeftMargin(15);
    fHVReadButton->Connect("Clicked()", "TMHWConfigModule", this, "ReadHVStatus()");
    fHVCtrlFrame->AddFrame(fHVReadButton, 
                           new TGTableLayoutHints(0, 1, 0, 1, kLHintsFillX, 20, 5, 20, 5));
    
    fHVToggleButton[0] = new TGTextButton(fHVCtrlFrame, "Turn off M1");
    fHVToggleButton[0]->SetTopMargin(15);
    fHVToggleButton[0]->SetBottomMargin(15);
    fHVToggleButton[0]->SetRightMargin(15);
    fHVToggleButton[0]->SetLeftMargin(15);
    fHVToggleButton[0]->SetText("Status unknown");
    fHVToggleButton[0]->SetState(kButtonUp);
    fHVToggleButton[0]->SetEnabled(kFALSE);
    fHVToggleButton[0]->Connect("Clicked()", "TMHWConfigModule", this, "ChangeHVStatus(=0)");
    fHVCtrlFrame->AddFrame(fHVToggleButton[0],  
                           new TGTableLayoutHints(1, 2, 0, 1, kLHintsExpandX | kLHintsFillY | kLHintsCenterX | kLHintsCenterY, 5, 5, 20, 5));

    fHVToggleButton[1] = new TGTextButton(fHVCtrlFrame, "Turn off M2");
    fHVToggleButton[1]->SetTopMargin(15);
    fHVToggleButton[1]->SetBottomMargin(15);
    fHVToggleButton[1]->SetRightMargin(15);
    fHVToggleButton[1]->SetLeftMargin(15);
    fHVToggleButton[1]->SetText("Status unknown");
    fHVToggleButton[1]->SetState(kButtonUp);
    fHVToggleButton[1]->SetEnabled(kFALSE);
    fHVToggleButton[1]->Connect("Clicked()", "TMHWConfigModule", this, "ChangeHVStatus(=1)");
    fHVCtrlFrame->AddFrame(fHVToggleButton[1], 
                           new TGTableLayoutHints(2, 3, 0, 1, kLHintsExpandX | kLHintsFillY | kLHintsCenterX | kLHintsCenterY, 5, 5, 20, 5));
    
    Pixel_t pixel;
    gClient->GetColorByName("gray", pixel);
    fHVToggleButton[0]->SetBackgroundColor(pixel);
    fHVToggleButton[1]->SetBackgroundColor(pixel);

    fWriteHWButton = new TGTextButton(fHVCtrlFrame, "Write to HW");
    fWriteHWButton->SetTopMargin(15);
    fWriteHWButton->SetBottomMargin(15);
    fWriteHWButton->SetRightMargin(15);
    fWriteHWButton->SetLeftMargin(15);
    fWriteHWButton->Connect("Clicked()", "TMHWConfigModule", this, "WriteHVToHardware()");
    fHVCtrlFrame->AddFrame(fWriteHWButton,
                            new TGTableLayoutHints(0, 1, 1, 2, kLHintsFillX, 20, 5, 25, 5));

    fReadHWButton = new TGTextButton(fHVCtrlFrame, "Read from HW");
    fReadHWButton->SetTopMargin(15);
    fReadHWButton->SetBottomMargin(15);
    fReadHWButton->SetRightMargin(15);
    fReadHWButton->SetLeftMargin(15);
    fReadHWButton->Connect("Clicked()", "TMHWConfigModule", this, "ReadHVFromHardware()");
    fHVCtrlFrame->AddFrame(fReadHWButton,
                            new TGTableLayoutHints(0, 1, 2, 3, kLHintsFillX, 20, 5, 15, 5));


    // ------------------------------ End Tab ------------------------------
    fControlFrame->AddFrame(fSettingsTab,  new TGTableLayoutHints(0, 2, 1, 2, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));
    fSettingsTab->SetEnabled(3, kFALSE);
    fSettingsTab->SetEnabled(4, kFALSE);
    fSettingsTab->SetEnabled(5, kFALSE);


    // ------------------------------ Main control buttons ------------------------------
    fButtonsFrame = new TGCompositeFrame(fControlFrame);
    fButtonsFrame->SetLayoutManager(new TGHorizontalLayout(fButtonsFrame));
    
    fQuitButton = new TGTextButton(fButtonsFrame, "Quit Module");
    fQuitButton->SetTopMargin(15);
    fQuitButton->SetBottomMargin(15);
    fQuitButton->SetRightMargin(15);
    fQuitButton->SetLeftMargin(15);
    fQuitButton->Connect("Clicked()", "TMHWConfigModule", this, "Finished()");
    fButtonsFrame->AddFrame(fQuitButton,
                            new TGLayoutHints(kLHintsRight, 5, 5, 5, 5));
    
    fWriteDBButton = new TGTextButton(fButtonsFrame, "Write to DB");
    fWriteDBButton->SetTopMargin(15);
    fWriteDBButton->SetBottomMargin(15);
    fWriteDBButton->SetRightMargin(15);
    fWriteDBButton->SetLeftMargin(15);
    fWriteDBButton->Connect("Clicked()", "TMHWConfigModule", this, "WriteTable()");
    fButtonsFrame->AddFrame(fWriteDBButton,
                            new TGLayoutHints(kLHintsRight, 5, 0, 5, 5));


    // add buttons frame to control frame
    fControlFrame->AddFrame(fButtonsFrame,  new TGTableLayoutHints(0, 2, 2, 3, kLHintsFillX | kLHintsRight, 5, 5, 15, 5));


    // Progress bar
    fProgressBar = new TGHProgressBar(fControlFrame, TGProgressBar::kFancy, 10);
    fProgressBar->SetBarColor("green");
    fProgressBar->Resize(200, 25);
    fProgressBar->SetMin(0);
    fProgressBar->SetMax(kMaxSize);
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Nothing to do");
    fControlFrame->AddFrame(fProgressBar,  new TGTableLayoutHints(0, 2, 3, 4, kLHintsFillX | kLHintsFillY | kLHintsLeft, 5, 5, 35, 5));

    // add control frame to main frame
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsFillX | kLHintsFillY, 10, 10, 10, 10));
    fControlFrame->Resize(fControlFrame->GetDefaultSize()); 
 

    // ------------------------------ Input frame ------------------------------
    fInputFrame = new TGVerticalFrame(fFrame, 250, 500);
    
    
    fTableTitle = new TGLabel(fInputFrame, "Table: none");
    fTableTitle->SetTextJustify(kTextLeft);
    fTableTitle->SetTextFont("-adobe-helvetica-bold-r-*-*-18-*-*-*-*-*-iso8859-1");
    fInputFrame->AddFrame(fTableTitle, new TGLayoutHints(kLHintsTop | kLHintsExpandX , 0, 0, 0, 10));
    
    // add header
    TGCompositeFrame* hdrFrame = new TGCompositeFrame(fInputFrame);
    hdrFrame->SetLayoutManager(new TGTableLayout(hdrFrame, 1, 4));
    
    l = new TGLabel(hdrFrame, "Element");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    hdrFrame->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsExpandX, 10, 10, 10, 10));
    
    l = new TGLabel(hdrFrame, "Current Value");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    hdrFrame->AddFrame(l, new TGTableLayoutHints(1, 2, 0, 1, kLHintsExpandX, 10, 10, 10, 10));
    
    l = new TGLabel(hdrFrame, "New Value");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    hdrFrame->AddFrame(l, new TGTableLayoutHints(2, 3, 0, 1, kLHintsExpandX, 10, 10, 10, 10));
    
    l = new TGLabel(hdrFrame, "Change");
    l->SetTextFont("-adobe-helvetica-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
    hdrFrame->AddFrame(l, new TGTableLayoutHints(3, 4, 0, 1, kLHintsExpandX, 10, 10, 10, 10));
    fInputFrame->AddFrame(hdrFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));
 
    // table scroll canvas
    fTableCanvas = new TGCanvas(fInputFrame, 250, 500, kSunkenFrame, TGCanvas::GetWhitePixel());
    fTableFrame = new TGCompositeFrame(fTableCanvas->GetViewPort(), 250, 500, kVerticalFrame | kOwnBackground);
    fTableFrame->SetLayoutManager(new TGTableLayout(fTableFrame, kMaxSize, 1));
    
    // mouse wheel scrolling
    fTableFrame->Connect("ProcessedEvent(Event_t*)", "TMHWConfigModule", this, "HandleMouseWheel(Event_t*)");
    gVirtualX->GrabButton(fTableFrame->GetId(), kButton4, kAnyModifier, 
                          kButtonPressMask | kButtonReleaseMask | kPointerMotionMask, kNone, kNone);
    gVirtualX->GrabButton(fTableFrame->GetId(), kButton5, kAnyModifier, 
                          kButtonPressMask | kButtonReleaseMask | kPointerMotionMask, kNone, kNone);

    // create table 
    fElementFrame = new TGCompositeFrame*[kMaxSize];
    fElementCurrentValue = new TGLabel*[kMaxSize];
    fElementNewValue = new TGNumberEntry*[kMaxSize];
    fElementValueChanged = new TGLabel*[kMaxSize];
   
    // add element rows
    for (UInt_t i = 0; i < kMaxSize; i++)
    {
        
        // create element frame
        fElementFrame[i] = new TGCompositeFrame(fTableFrame);
        fElementFrame[i]->SetLayoutManager(new TGTableLayout(fElementFrame[i], 1, 4));
 
        // add element number
        sprintf(aName, "   %3d   ", i+1);
        l = new TGLabel(fElementFrame[i], aName);
        l->SetTextJustify(kTextCenterX);
        l->SetTextFont("-adobe-courier-*-r-*-*-*-*-*-*-*-70-*-*");
        fElementFrame[i]->AddFrame(l, new TGTableLayoutHints(0, 1, 0, 1, kLHintsExpandX | kLHintsCenterY, 4, 4, 4, 4));

        // add old value display
        fElementCurrentValue[i] = new TGLabel(fElementFrame[i], "   0   ");
        fElementCurrentValue[i]->SetTextJustify(kTextCenterX);
        fElementCurrentValue[i]->SetTextFont("-adobe-courier-*-r-*-*-*-*-*-*-*-70-*-*");
        fElementFrame[i]->AddFrame(fElementCurrentValue[i], 
                                   new TGTableLayoutHints(1, 2, 0, 1, kLHintsExpandX | kLHintsCenterY, 80, 30, 4, 4));

        // add new value entry
        fElementNewValue[i] = new TGNumberEntry(fElementFrame[i], 0, 10);
        fElementNewValue[i]->SetLimits(TGNumberFormat::kNELLimitMinMax);
        fElementNewValue[i]->Connect("ValueSet(Long_t)", "TMHWConfigModule", this, "MarkChanges()");
        fElementFrame[i]->AddFrame(fElementNewValue[i], 
                                   new TGTableLayoutHints(2, 3, 0, 1, kLHintsExpandX | kLHintsCenterY, 60, 60, 4, 4));
        
        // add value change
        fElementValueChanged[i] = new TGLabel(fElementFrame[i], "          ");
        fElementValueChanged[i]->SetTextJustify(kTextCenterX);
        fElementFrame[i]->AddFrame(fElementValueChanged[i], 
                                   new TGTableLayoutHints(3, 4, 0, 1, kLHintsExpandX | kLHintsCenterY, 0, 0, 4, 4));
        
        // add element fram to table frame
        fTableFrame->AddFrame(fElementFrame[i], 
                              new TGTableLayoutHints(0, 1, i, i+1, kLHintsExpandX, 10, 10, 10, 10));
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
        for (UInt_t i = 0; i < kMaxSize; i++) 
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
    fTableCombo->Select(0, kTRUE);
    fRangeManipCombo->Select(kRange_All_Elements, kFALSE);
    fLEDRangeCombo->Select(kRange_Single_Element, kFALSE);
    
    // select decimal format
    fHexFormat->SetState(kButtonUp);
    ToggleHexFormat();
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
void TMHWConfigModule::ToggleHexFormat()
{
    // Change the user input fields to decimal or hexadecimal format.
    
    //Int_t val;

    // evaluate check-button
    if (fHexFormat->IsOn())
    {
        fRangeManipEntry->SetFormat(TGNumberFormat::kNESHex);
        for (UInt_t i = 0; i < kMaxSize; i++)
        {
            //sscanf(fElementCurrentValue[i]->GetText()->Data(), "%d", &val);
            //fElementCurrentValue[i]->SetText(TString::Format("%x", val));
            fElementNewValue[i]->SetFormat(TGNumberFormat::kNESHex);
        }
    }
    else
    {
        fRangeManipEntry->SetFormat(TGNumberFormat::kNESInteger);
        for (UInt_t i = 0; i < kMaxSize; i++) 
        {
            //sscanf(fElementCurrentValue[i]->GetText()->Data(), "%x", &val);
            //fElementCurrentValue[i]->SetText(TString::Format("%d", val));
            fElementNewValue[i]->SetFormat(TGNumberFormat::kNESInteger);
        }
    }
}

//______________________________________________________________________________
void TMHWConfigModule::LEDRangeChanged(Int_t id)
{
    // Update the GUI after the user has changed the range of the LED 
    // elements in the combo box.
    
    if (id == kRange_Single_Element) fLEDChannelEntry->SetState(kTRUE);
    else fLEDChannelEntry->SetState(kFALSE);
}

//______________________________________________________________________________
void TMHWConfigModule::SelectFile(Int_t ftype)
{
    // Let the user select a file. The file name will be used depending on
    // the ftype argument.
    
    Char_t filename[256];

    // Let TAPSMaintain open the "Save file" or "Open file" dialog
    if (ftype == kFILE_SELECT_EXPORT) ShowFileDialog(kFDSave);
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
    if (ftype == kFILE_SELECT_EXPORT)
    {
        fExportFileEntry->SetText(filename);
    }
    else if (ftype == kFILE_SELECT_IMPORT)
    {
        if (flags == 0 || flags == 1) fImportFileEntry->SetText(filename);
    }
    else if (ftype == kFILE_SELECT_GM_ENERGY_CALIB)
    {
        if (flags == 0 || flags == 1) fGMCalibFileEntry->SetText(filename);
    }
    else if (ftype == kFILE_SELECT_LED_CALIB)
    {
        if (flags == 0 || flags == 1) fLEDCalibFileEntry->SetText(filename);
    }
    else if (ftype == kFILE_SELECT_LED_ENERGY_CALIB)
    {
        if (flags == 0 || flags == 1) fLEDEnergyCalibFileEntry->SetText(filename);
    }
    else return;
}

//______________________________________________________________________________
void TMHWConfigModule::ImportFile()
{
    // Import values from the specified file.
    
    Char_t tmp[256];

    Int_t id;
    Float_t value;

    // get the selected file name
    const Char_t* filename = fImportFileEntry->GetText();

    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
  
    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;

    // try to open the file
    FILE* fin;
    fin = fopen(filename, "r");
    if (!fin)
    {
        sprintf(tmp, "Could not open the file '%s'!", filename);
        ModuleError(tmp);
        return;
    }

    // read file and set values
    while (!feof(fin))
    {
        if (!fgets(tmp, 256, fin) && !feof(fin)) Error("ImportFile", "File read error!");

        // check if line is a comment
        if (TMUtils::IsComment(tmp)) continue;

        if (sscanf(tmp, "%d%f", &id, &value) == 2)
        {
            if (CheckValueLimits(table, value)) fElementNewValue[id-1]->SetNumber(value);
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
    
    Char_t tmp[256];
    
    Int_t id;
    Float_t ped, peak, cosm_gain;
    Double_t range, res;
    Double_t nom_gain, gain_bias;
    Double_t hv_old, hv_new;
   
    // get the selected file name
    const Char_t* filename = fGMCalibFileEntry->GetText();

    // leave if import file entry is empty
    if (!strcmp(filename, "")) return;
    
    // get selected data type
    Int_t table = fTableCombo->GetSelected();
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
    
    // get minimum and maximum values
    Double_t min = dataType->GetMin();
    Double_t max = dataType->GetMax();

    // read range/resolution
    range = fGMRangeEntry->GetNumber();
    res = fGMResEntry->GetNumber();

    // try to open the file
    FILE* fin;
    fin = fopen(filename, "r");
    if (!fin)
    {
        sprintf(tmp, "Could not open the file '%s'!", filename);
        ModuleError(tmp);
        return;
    }
 
    // read file and set values
    while (!feof(fin))
    {
        if (!fgets(tmp, 256, fin) && !feof(fin)) Error("DoGainMatch", "File read error!");

        // check if line is a comment
        if (TMUtils::IsComment(tmp)) continue;
        
        // read id, pedestal pos., cosmic peak pos. and gain
        if (sscanf(tmp, "%d%f%f%f", &id, &ped, &peak, &cosm_gain) == 4)
        {	
            // skip bad calibration
            if (ped == 0 && peak == 0) continue;
            
            // skip empty BaF2 channels
            Int_t ring = TMUtils::GetRingNumber(id-1);
            if (ring == 1 || ring == 2) continue;

            // read old HV value
            hv_old = atof(fElementCurrentValue[id-1]->GetText()->Data());

            // calculate new HV value
            nom_gain = range / (res - ped);
            gain_bias = hv_old - kBaF2_Gain_Slope * TMath::Log(peak - ped);
            hv_new = kBaF2_Gain_Slope * TMath::Log(kTAPS_MIP_Loss_BaF2 / nom_gain) + gain_bias;
            
            // correct bad HV value
            if (hv_new < 0) hv_new = hv_old;

            // check limits and set new HV value
            if (hv_new < min) fElementNewValue[id-1]->SetNumber((Int_t)min);
            else if (hv_new > max) fElementNewValue[id-1]->SetNumber((Int_t)max);
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
    
    Char_t tmp[256];

    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
     
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
  
    // get the selected file name
    const Char_t* filename = fExportFileEntry->GetText();

    // leave if export file entry is empty
    if (!strcmp(filename, "")) return;
    
    // try to open the file
    FILE* fout;
    fout = fopen(filename, "w");
    if (!fout)
    {
        sprintf(tmp, "Could not open the file '%s'!", filename);
        ModuleError(tmp);
        return;
    }
 
    // write header
    if (fExportColumn->IsOn()) fprintf(fout, "# TAPSMaintain data export\n");
    else
    {
        fprintf(fout, "# TAPSMaintain data export\n");
        fprintf(fout, "# Data: %s\n", dataType->GetTitle());
    }
    fprintf(fout, "\n");
    fprintf(fout, "# ID    Value\n");
    
    // write current values to file
    for (Int_t i = 0; i < dataType->GetSize(); i++)
    {
        if (fExportColumn->IsOn()) fprintf(fout, "%3d    %d\n", i+1, (Int_t)fElementNewValue[i]->GetNumber());
        else fprintf(fout, "%3d    %s\n", i+1, fElementCurrentValue[i]->GetText()->Data());
    }

    // close the file
    fclose(fout);

    // show info message
    Char_t msg[256];
    if (fExportColumn->IsOn()) sprintf(msg, "New values were saved to '%s' .", filename);
    else sprintf(msg, "Current values of data '%s' were saved to '%s' .", dataType->GetTitle(), filename);
    ModuleInfo(msg);
}

//______________________________________________________________________________
void TMHWConfigModule::AddFileToLEDCalibration()
{
    // Add an additional file to the LED calibration. Regenerate and refit the
    // calibration graphs of every channel.

    Char_t tmp[256];
    Int_t id, voltageLED;
    Float_t thr;
    Double_t thresholds[kMaxSize];
    Double_t voltages[kMaxSize];
    Double_t energyCalibPed[kMaxSize];
    Double_t energyCalibGain[kMaxSize];
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
        
        // try to open the energy calibration file
        fin = fopen(ecalfile, "r");
        if (!fin)
        {
            sprintf(tmp, "Could not open the energy calibration file '%s'!", ecalfile);
            ModuleError(tmp);
            return;
        }
 
        // read voltages and threshold from file
        while (!feof(fin))
        {
            if (!fgets(tmp, 256, fin) && !feof(fin)) Error("AddFileToLEDCalibration", "File read error!");

            // check if line is a comment
            if (TMUtils::IsComment(tmp)) continue;
        
            // read id, pedestal pos., cosmic peak pos. and gain
            if (sscanf(tmp, "%d%f%f%f", &id, &ped, &peak, &gain) == 4)
            {
                energyCalibPed[id-1] = ped;
                energyCalibGain[id-1] = gain;
            }
        }
        fclose(fin);
    }
    
    // init arrays
    for (UInt_t i = 0; i < kMaxSize; i++)
    {
        thresholds[i] = 0;
        voltages[i] = 0;
    }
   
    // open the LED calibration file
    fin = fopen(filename, "r");
    if (!fin)
    {
        sprintf(tmp, "Could not open the LED calibration file '%s'!", filename);
        ModuleError(tmp);
        return;
    }
 
    // read voltages and threshold from file
    while (!feof(fin))
    {
        if (!fgets(tmp, 256, fin) && !feof(fin)) Error("AddFileToLEDCalibration", "File read error!");

        // check if line is a comment
        if (TMUtils::IsComment(tmp)) continue;
        
        // read id, LED threshold voltage and LED threshold channel
        if (sscanf(tmp, "%d%d%f", &id, &voltageLED, &thr) == 3)
        {
            voltages[id-1] = (Double_t) voltageLED;
            thresholds[id-1] = (Double_t) thr;
        }
    }
    fclose(fin);
    
    // clear file input entry
    fLEDCalibFileEntry->SetText("");
  
    // check if fit functions exist
    if (!fLEDFitFunctions)
    {
        // generate LED fit functions
        fLEDFitFunctions = new TF1*[kMaxSize];
        for (UInt_t i = 0; i < kMaxSize; i++)
        {
            sprintf(tmp, "LED_fit_func_%i", i+1);
            fLEDFitFunctions[i] = new TF1(tmp, "pol1", 0, 2000);
            fLEDFitFunctions[i]->SetLineColor(kRed);
        }
    }
    
    // generate and fit graphs if not created yet
    if (!fLEDGraphs)
    {
        // create graphs
        fLEDGraphs = new TGraph*[kMaxSize];
        for (UInt_t i = 0; i < kMaxSize; i++) fLEDGraphs[i] = new TGraph(1);
        
        // fill values into graphs
        for (UInt_t i = 0; i < kMaxSize; i++)
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
        
        for (UInt_t i = 0; i < kMaxSize; i++)
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
            sprintf(tmp, "LED_fit_func_%d", i+1);
            fLEDGraphs[i]->Fit(tmp, "Q");
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
    
    // get selected data type
    Int_t table = fTableCombo->GetSelected();
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
    
    // get minimum and maximum values
    Double_t min = dataType->GetMin();
    Double_t max = dataType->GetMax();

    if (selectedLEDRange == kRange_Single_Element)
    {
        // selected channel
        Int_t id = (UInt_t) fLEDChannelEntry->GetNumber();
        
        // check limits and set value 
        Int_t resValue = (Int_t)fLEDFitFunctions[id-1]->Eval(reqValue);
        if (resValue < min) fElementNewValue[id-1]->SetNumber(min);
        else if (resValue > max) fElementNewValue[id-1]->SetNumber(max);
        else fElementNewValue[id-1]->SetNumber(resValue);
    }
    else if (selectedLEDRange == kRange_All_Elements)
    {
        // loop over all elements
        for (UInt_t i = 0; i < kMaxSize; i++)
        {
            Int_t resValue = (Int_t)fLEDFitFunctions[i]->Eval(reqValue);
            if (resValue < min) fElementNewValue[i]->SetNumber(min);
            else if (resValue > max) fElementNewValue[i]->SetNumber(max);
            else fElementNewValue[i]->SetNumber(resValue);
        }
    }
    else
    {
        // set dummy ring number
        UInt_t ring = 99;
        
        // set ring numbers if ring range was selected
        if (selectedLEDRange == kRange_Ring_1) ring = 1;
        if (selectedLEDRange == kRange_Ring_2) ring = 2;
        if (selectedLEDRange == kRange_Ring_3) ring = 3;
        if (selectedLEDRange == kRange_Ring_4) ring = 4;
        if (selectedLEDRange == kRange_Ring_5) ring = 5;
        if (selectedLEDRange == kRange_Ring_6) ring = 6;
        if (selectedLEDRange == kRange_Ring_7) ring = 7;
        if (selectedLEDRange == kRange_Ring_8) ring = 8;
        if (selectedLEDRange == kRange_Ring_9) ring = 9;
        if (selectedLEDRange == kRange_Ring_10) ring = 10;
        if (selectedLEDRange == kRange_Ring_11) ring = 11;
        
        // leave if no ring range was selected
        if (ring == 99) return;
        
        // loop over all elements and update members of selected ring
        for (UInt_t i = 0; i < kMaxSize; i++)
        {
            // element belongs to specified ring -> change value
            if (ring == TMUtils::GetRingNumber(i))
            {
                Int_t resValue = (Int_t)fLEDFitFunctions[i]->Eval(reqValue);
                if (resValue < min) fElementNewValue[i]->SetNumber(min);
                else if (resValue > max) fElementNewValue[i]->SetNumber(max);
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
    
    if (selectedLEDRange == kRange_Single_Element)
    {
        // selected channel
        UInt_t id = (UInt_t) fLEDChannelEntry->GetNumber();
    
        if (id > 0 && id <= kMaxSize)
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
    else if (selectedLEDRange == kRange_All_Elements)
    {
        Char_t text[256];
        
        // create block canvas
        CreateExternalCanvas(kMaxSize);
        
        // draw block elements
        for (UInt_t i = 0; i < kMaxSize; i++)
        {
            TVirtualPad* aPad = fExternalCanvas->cd(i+1);
            
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
    else
    {
        Char_t text[256];
        
        // set dummy start value
        Int_t start = -99;
        
        // set start value for blocks
        if (selectedLEDRange == kRange_Block_A) start = 0;
        if (selectedLEDRange == kRange_Block_B) start = 64;
        if (selectedLEDRange == kRange_Block_C) start = 128;
        if (selectedLEDRange == kRange_Block_D) start = 192;
        if (selectedLEDRange == kRange_Block_E) start = 256;
        if (selectedLEDRange == kRange_Block_F) start = 320;
        
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
void TMHWConfigModule::CreateExternalCanvas(UInt_t n)
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
    else if (n == kMaxSize) fExternalCanvas->Divide(24, 16, 0.001, 0.001);
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
        if (CheckValueLimits(table, value)) fElementNewValue[i]->SetNumber(value);
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
    
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
  
    // loop over all elements and change value if element belongs to the
    // specified ring
    for (Int_t i = 0; i < dataType->GetSize(); i++)
    {
        // element belongs to specified ring -> change value
        if (ring == TMUtils::GetRingNumber(i))
        {
            if (CheckValueLimits(table, value)) fElementNewValue[i]->SetNumber(value);
        }
    }
}

//______________________________________________________________________________
void TMHWConfigModule::ClearValues()
{
    // Set all values in the element array displays and entries to 0.
    
    // clear title
    fTableTitle->SetText("Table: none");
    
    // clear values
    for (UInt_t i = 0; i < kMaxSize; i++)
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
    for (UInt_t i = 0; i < kMaxSize; i++)
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
    
    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
    
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
  
    // set values
    if (range == kRange_All_Elements) 
    {
        // check limits
        if (CheckValueLimits(table, value)) 
            for (Int_t i = 0; i < dataType->GetSize(); i++) fElementNewValue[i]->SetNumber(value);
    }
    else if (range == kRange_Block_A) SetBlockValues(1, value);
    else if (range == kRange_Block_B) SetBlockValues(2, value);
    else if (range == kRange_Block_C) SetBlockValues(3, value);
    else if (range == kRange_Block_D) SetBlockValues(4, value);
    else if (range == kRange_Block_E) SetBlockValues(5, value);
    else if (range == kRange_Block_F) SetBlockValues(6, value);
    else if (range == kRange_Ring_1) SetRingValues(1, value);
    else if (range == kRange_Ring_2) SetRingValues(2, value);
    else if (range == kRange_Ring_3) SetRingValues(3, value);
    else if (range == kRange_Ring_4) SetRingValues(4, value);
    else if (range == kRange_Ring_5) SetRingValues(5, value);
    else if (range == kRange_Ring_6) SetRingValues(6, value);
    else if (range == kRange_Ring_7) SetRingValues(7, value);
    else if (range == kRange_Ring_8) SetRingValues(8, value);
    else if (range == kRange_Ring_9) SetRingValues(9, value);
    else if (range == kRange_Ring_10) SetRingValues(10, value);
    else if (range == kRange_Ring_11) SetRingValues(11, value);

    MarkChanges();
}

//______________________________________________________________________________
Bool_t TMHWConfigModule::CheckValueLimits(Int_t tableIndex, Double_t value)
{
    // Check if the value 'value' lies within the limits of the accepted values
    // for the table index 'tableIndex'.
    
    // get data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(tableIndex);
    
    // check value
    if (value >= dataType->GetMin() && value <= dataType->GetMax()) return kTRUE;
    else return kFALSE;
}

//______________________________________________________________________________
void TMHWConfigModule::ReadTable(Int_t table)
{
    // Read table from database and display the content.
    
    // Clear current displayed values
    ClearValues();
     
    // set all to standard when first dummy entry was selected
    if (table == 0) 
    {
        fRangeManipEntry->SetLimitValues(0., 0.);
        fSettingsTab->SetEnabled(3, kFALSE);
        fSettingsTab->SetEnabled(4, kFALSE);
        fSettingsTab->SetEnabled(5, kFALSE);
        fSettingsTab->SetTab(0, kFALSE);
        return;
    }

    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);

    // de-/activate GUI elements depending on the loaded table 
    if (!strcmp(dataType->GetName(), "Par.BaF2.HV") || 
        !strcmp(dataType->GetName(), "Par.Veto.HV") || 
        !strcmp(dataType->GetName(), "Par.PWO.HV"))
    {
        fSettingsTab->SetEnabled(3, kTRUE);
        fSettingsTab->SetEnabled(4, kFALSE);
        fSettingsTab->SetEnabled(5, kTRUE);
    }
    else if (!strcmp(dataType->GetName(), "Par.BaF2.Thr.LED1") || 
             !strcmp(dataType->GetName(), "Par.BaF2.Thr.LED2"))
    {
        fSettingsTab->SetEnabled(3, kFALSE);
        fSettingsTab->SetEnabled(4, kTRUE);
    }
    else 
    {
        fSettingsTab->SetEnabled(3, kFALSE);
        fSettingsTab->SetEnabled(4, kFALSE);
        fSettingsTab->SetEnabled(5, kFALSE);
    }

    // set standard tab
    fSettingsTab->SetTab(0, kFALSE);
    
    // set range manipulation number entry limits
    fRangeManipEntry->SetLimitValues(dataType->GetMin(), dataType->GetMax());
    
    // try to open connection to database server
    if (!TTMySQLManager::GetManager())
    {
        fTableCombo->Select(0, kTRUE);
        Error("ReadTable", "Could not connect to the database. Please check your settings!");
        return;
    }
  
    // try to read values
    Int_t size = dataType->GetSize();
    Int_t elem[size];
    Double_t par[size];
    for (Int_t i = 0; i < size; i++) elem[i] = i;
    if (!TTMySQLManager::GetManager()->ReadParameters(dataType->GetName(), size, elem, par))
    {
        fTableCombo->Select(0, kTRUE);
        Error("ReadTable", "Could not read values from the database. Please check your settings!");
        return;
    }
    
    // display values
    for (Int_t i = 0; i < size; i++)
    {   
        // show element row
        fElementFrame[i]->MapWindow();
        
        // set current and new value
        Char_t value[128];
        sprintf(value, "%.f", par[i]);
        fElementCurrentValue[i]->SetText(value);
        fElementNewValue[i]->SetNumber(par[i]);
        
        // set limits of new value entries
        fElementNewValue[i]->SetLimitValues(dataType->GetMin(), dataType->GetMax());
    }
    
    // hide unused element rows
    for (UInt_t i = size; i < kMaxSize; i++)
    {
        fElementFrame[i]->UnmapWindow();
    }

    // mark changes in case values from DB are out of the limits of the number entries
    MarkChanges();
}

//______________________________________________________________________________
void TMHWConfigModule::WriteTable()
{
    // Write the values back to the table in the database. 
    
    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
 
    // ask user for confirmation
    ModuleQuestion("Are you REALLY sure you want to write the new values to the DB?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
    
    // try to open connection to database server
    if (!TTMySQLManager::GetManager())
    {
        fTableCombo->Select(0, kTRUE);
        Error("WriteTable", "Could not connect to the database. Please check your settings!");
        return;
    }
 
    // prepare arrays
    Int_t size = dataType->GetSize();
    Int_t elem[size];
    Double_t par[size];
    for (Int_t i = 0; i < size; i++) 
    {
        elem[i] = i;
        par[i] = fElementNewValue[i]->GetNumber();
    }
    
    // try to write values
    if (!TTMySQLManager::GetManager()->WriteParameters(dataType->GetName(), size, elem, par))
    {
        fTableCombo->Select(0, kTRUE);
        Error("WriteTable", "Could not write values to the database. Please check your settings!");
        return;
    }
 
    // read new table
    ReadTable(table);
}

//______________________________________________________________________________
void TMHWConfigModule::WriteHVToHardware()
{
    // Write the current values to the hardware HV mainframe. 
    
    Char_t tmp[256];

    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
     
    // check HV connection
    if (!TTServerManager::GetManager()->IsConnectedToHV())
    {
        ModuleError("No connection to HV server!");
        Error("WriteHVToHardware", "No connection to HV server!");
        return;
    }
    
    // ask user for confirmation
    ModuleQuestion("Are you REALLY sure you want to write the current values\nsaved in the database to the hardware?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
    
    // format progress bar
    fProgressBar->SetMax(dataType->GetSize());
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Writing element %.0f");

    // loop over elements
    for (Int_t i = 0; i < dataType->GetSize(); i++) 
    {
        // write HV to hardware
        if (!TTServerManager::GetManager()->WriteHV(dataType, i))
        {
            sprintf(tmp, "Could not write high voltage of element %d!", i);
            ModuleError(tmp);
            Error("WriteHVToHardware", "%s", tmp);
        }
        
        // update GUI
        fProgressBar->SetPosition(i+1); 
        gSystem->ProcessEvents();
    }
    
    // reset progress bar
    fProgressBar->Reset();
    
    // re-format progress bar
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Nothing to do");

    ModuleInfo("Writing completed!");
}

//______________________________________________________________________________
void TMHWConfigModule::ReadHVFromHardware()
{
    // Read the HV values from the mainframe to the current values. 
    
    Char_t tmp[256];
    Int_t val;

    // leave if the dummy entry in the combo was selected
    Int_t table = fTableCombo->GetSelected();
    if (table == 0) return;
     
    // check HV connection
    if (!TTServerManager::GetManager()->IsConnectedToHV())
    {
        ModuleError("No connection to HV server!");
        Error("ReadHVFromHardware", "No connection to HV server!");
        return;
    }
    
    // ask user for confirmation
    ModuleQuestion("Are you REALLY sure you want to overwrite the new values\nwith the values from the HV hardware?");
    if (GetDialogReturnValue() == kMBNo) return;
    
    // get selected data type
    TTDataTypePar* dataType = (TTDataTypePar*) fParTypes->At(table);
    
    // format progress bar
    fProgressBar->SetMax(dataType->GetSize());
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Reading element %.0f");

    // loop over elements
    for (Int_t i = 0; i < dataType->GetSize(); i++) 
    {
        // read HV from hardware
        if (!TTServerManager::GetManager()->ReadHV(dataType, i, &val))
        {
            sprintf(tmp, "Could not read high voltage of element %d!", i);
            ModuleError(tmp);
            Error("ReadHVFromHardware", "%s", tmp);
        }

        // set value
        fElementNewValue[i]->SetNumber((Double_t)val);
        
        // update GUI
        fProgressBar->SetPosition(i+1); 
        MarkChanges();
        gSystem->ProcessEvents();
    }
    
    // reset progress bar
    fProgressBar->Reset();
    
    // re-format progress bar
    fProgressBar->ShowPosition(kTRUE, kFALSE, "Nothing to do");

    ModuleInfo("Reading completed!");
}

//______________________________________________________________________________
void TMHWConfigModule::ReadHVStatus()
{
    // Read the status of the HV mainframes. 
    
    Char_t tmp[256];
    
    // check HV connection
    if (!TTServerManager::GetManager()->IsConnectedToHV())
    {
        ModuleError("No connection to HV server!");
        Error("ReadHVStatus", "No connection to HV server!");
        return;
    }
    
    // lock read button
    fHVReadButton->SetText("Reading...");
    fHVReadButton->SetEnabled(kFALSE);
    gSystem->ProcessEvents();
    
    // get colors
    Pixel_t pixel_gray;
    Pixel_t pixel_red;
    Pixel_t pixel_green;
    gClient->GetColorByName("gray", pixel_gray);
    gClient->GetColorByName("red", pixel_red);
    gClient->GetColorByName("green", pixel_green);
     
    // read HV status of mainframes
    Bool_t status;
    for (Int_t i = 0; i < 2; i++)
    {
        // try to read status and set-up GUI
        if (!TTServerManager::GetManager()->GetStatusHV(i+1, &status))
        {
            fHVToggleButton[i]->SetText("Status unknown");
            fHVToggleButton[i]->SetBackgroundColor(pixel_gray);
            fHVToggleButton[i]->SetEnabled(kFALSE);
            sprintf(tmp, "Could not read high voltage status of mainframe %d!", i+1);
            ModuleError(tmp);
            Error("ReadHVStatus", "%s", tmp);
        }
        else
        {
            if (status)
            {
                sprintf(tmp, "Turn off M%d", i+1);
                fHVToggleButton[i]->SetText(tmp);
                fHVToggleButton[i]->SetBackgroundColor(pixel_green);
                fHVToggleButton[i]->SetEnabled(kTRUE);
            }
            else
            {
                sprintf(tmp, "Turn on M%d", i+1);
                fHVToggleButton[i]->SetText(tmp);
                fHVToggleButton[i]->SetBackgroundColor(pixel_red);
                fHVToggleButton[i]->SetEnabled(kTRUE);
            }
        }

        // update GUI
        gSystem->ProcessEvents();
    }

    // release read button
    fHVReadButton->SetText("Read HV status");
    fHVReadButton->SetEnabled(kTRUE);
}

//______________________________________________________________________________
void TMHWConfigModule::ChangeHVStatus(Int_t id)
{
    // Change the status of the HV mainframes. 
    
    Char_t tmp[256];
    
    // check HV connection
    if (!TTServerManager::GetManager()->IsConnectedToHV())
    {
        ModuleError("No connection to HV server!");
        Error("ChangeHVStatus", "No connection to HV server!");
        return;
    }
    
    // ask user for confirmation
    ModuleQuestion("Are you REALLY sure you want to change the status of the\nhigh voltage mainframe?");
    if (GetDialogReturnValue() == kMBNo) return;
  
    // lock read button
    fHVToggleButton[id]->SetText("Changing...");
    fHVToggleButton[id]->SetEnabled(kFALSE);
    gSystem->ProcessEvents();
    
    // get colors
    Pixel_t pixel_gray;
    Pixel_t pixel_red;
    Pixel_t pixel_green;
    gClient->GetColorByName("gray", pixel_gray);
    gClient->GetColorByName("red", pixel_red);
    gClient->GetColorByName("green", pixel_green);
     
    // try to read HV status of mainframes
    Bool_t status;
    if (!TTServerManager::GetManager()->GetStatusHV(id+1, &status))
    {
        fHVToggleButton[id]->SetText("Status unknown");
        fHVToggleButton[id]->SetBackgroundColor(pixel_gray);
        fHVToggleButton[id]->SetEnabled(kFALSE);
        sprintf(tmp, "Could not read high voltage status of mainframe %d!", id+1);
        ModuleError(tmp);
        Error("ChangeHVStatus", "%s", tmp);
        return;
    }
    
    // update GUI
    gSystem->ProcessEvents();
 
    // try to change HV status of mainframes
    status = !status;
    if (!TTServerManager::GetManager()->SetStatusHV(id+1, status))
    {
        fHVToggleButton[id]->SetText("Status unknown");
        fHVToggleButton[id]->SetBackgroundColor(pixel_gray);
        fHVToggleButton[id]->SetEnabled(kFALSE);
        sprintf(tmp, "Could not set high voltage status of mainframe %d!", id+1);
        ModuleError(tmp);
        Error("ChangeHVStatus", "%s", tmp);
        return;
    }

    // format GUI
    if (status)
    {
        sprintf(tmp, "Turn off M%d", id+1);
        fHVToggleButton[id]->SetText(tmp);
        fHVToggleButton[id]->SetBackgroundColor(pixel_green);
        fHVToggleButton[id]->SetEnabled(kTRUE);
    }
    else
    {
        sprintf(tmp, "Turn on M%d", id+1);
        fHVToggleButton[id]->SetText(tmp);
        fHVToggleButton[id]->SetBackgroundColor(pixel_red);
        fHVToggleButton[id]->SetEnabled(kTRUE);
    }

    // update GUI
    gSystem->ProcessEvents();
}

