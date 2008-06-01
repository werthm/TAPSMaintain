// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMDBModule                                                           //
//                                                                      //
// Module for the manipulation of the TAPS MySQL database.              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMDBModule.h"

ClassImp(TMDBModule)


//______________________________________________________________________________
TMDBModule::TMDBModule(const Char_t* name, UInt_t id)
    : TMModule(name, id, 0, kFALSE, kFALSE)
{
    // Constructor.
    
    Char_t aName[256];
    
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
    fTableCombo->Connect("Selected(Int_t)", "TMDBModule", this, "ReadTable(Int_t)");
    fTableCombo->Resize(200, 22);
    fTableCombo->AddEntry("Select TAPS table here", EDB_Table_Empty);
    fTableCombo->AddEntry("BaF2 High Voltage", EDB_Table_BaF2_HV);
    fTableCombo->AddEntry("BaF2 CFD Threshold", EDB_Table_BaF2_CFD);
    fTableCombo->AddEntry("BaF2 LED1 Threshold", EDB_Table_BaF2_LED1);
    fTableCombo->AddEntry("BaF2 LED2 Threshold", EDB_Table_BaF2_LED2);
    fControlFrame->AddFrame(fTableCombo, new TGTableLayoutHints(1, 2, 3, 4, kLHintsFillX | kLHintsLeft, 5, 5, 2, 2));
    fTableCombo->Select(EDB_Table_Empty, kFALSE);


    // ------------------------------ Range manipulation frame ------------------------------
    fRangeManipFrame = new TGGroupFrame(fControlFrame, "Element range manipulation", kHorizontalFrame);
    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "Set all"), new TGLayoutHints(kLHintsLeft, 5, 5, 15, 5));
    
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
    fRangeManipFrame->AddFrame(fRangeManipCombo, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));
    fRangeManipCombo->Select(ERange_All_Elements, kFALSE);

    fRangeManipFrame->AddFrame(new TGLabel(fRangeManipFrame, "to"), new TGLayoutHints(kLHintsLeft, 5, 5, 15, 5));
    
    fRangeManipEntry = new TGNumberEntry(fRangeManipFrame, 0, 8);
    fRangeManipFrame->AddFrame(fRangeManipEntry, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));

    fRangeManipButton = new TGTextButton(fRangeManipFrame, "  Set  ");
    fRangeManipButton->Connect("Clicked()", "TMDBModule", this, "DoRangeManipulation()");
    fRangeManipFrame->AddFrame(fRangeManipButton, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));

    // add range manipulation frame to control frame
    fControlFrame->AddFrame(fRangeManipFrame,  new TGTableLayoutHints(0, 2, 4, 5, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));

    
    // ------------------------------ Import frame ------------------------------
    fImportFrame = new TGGroupFrame(fControlFrame, 
                                    "Import values from file (2 numbers per line: ELEMENT_ID  VALUE)", kHorizontalFrame);
    fImportFrame->AddFrame(new TGLabel(fImportFrame, "File:"), new TGLayoutHints(kLHintsLeft, 5, 5, 15, 5));
    
    fImportFileEntry = new TGTextEntry(fImportFrame, "");
    fImportFileEntry->Resize(200, 22);
    fImportFrame->AddFrame(fImportFileEntry, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));
    
    fImportBrowse = new TGTextButton(fImportFrame, " Browse ");
    fImportBrowse->Connect("Clicked()", "TMDBModule", this, "OpenImportFile()");
    fImportFrame->AddFrame(fImportBrowse, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));

    fImportButton = new TGTextButton(fImportFrame, " Import ");
    fImportButton->Connect("Clicked()", "TMDBModule", this, "ImportFile()");
    fImportFrame->AddFrame(fImportButton, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));


    // add range manipulation frame to control frame
    fControlFrame->AddFrame(fImportFrame,  new TGTableLayoutHints(0, 2, 5, 6, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));


    // ------------------------------ Export frame ------------------------------
    fExportFrame = new TGGroupFrame(fControlFrame, "Export values", kHorizontalFrame);
    fExportFrame->AddFrame(new TGLabel(fExportFrame, "File:"), new TGLayoutHints(kLHintsLeft, 5, 5, 15, 5));
    
    fExportFileEntry = new TGTextEntry(fExportFrame, "");
    fExportFileEntry->Resize(200, 22);
    fExportFrame->AddFrame(fExportFileEntry, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));
    
    fExportBrowse = new TGTextButton(fExportFrame, " Browse ");
    fExportBrowse->Connect("Clicked()", "TMDBModule", this, "SelectExportFile()");
    fExportFrame->AddFrame(fExportBrowse, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));

    fExportButton = new TGTextButton(fExportFrame, " Export ");
    //fExportButton->Connect("Clicked()", "TMDBModule", this, "ExportFile()");
    fExportFrame->AddFrame(fExportButton, new TGLayoutHints(kLHintsLeft, 5, 5, 12, 2));


    // add range manipulation frame to control frame
    fControlFrame->AddFrame(fExportFrame,  new TGTableLayoutHints(0, 2, 6, 7, kLHintsFillX | kLHintsLeft, 5, 5, 15, 5));










    // add control frame to main frame
    fFrame->AddFrame(fControlFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsFillX | kLHintsFillY, 10, 10, 10, 10));
    
 

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
    fTableFrame->Connect("ProcessedEvent(Event_t*)", "TMDBModule", this, "HandleMouseWheel(Event_t*)");
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
        fElementNewValue[i]->Connect("ValueSet(Long_t)", "TMDBModule", this, "MarkChanges()");
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
TMDBModule::~TMDBModule()
{
    // Destructor.
    
}

//______________________________________________________________________________
void TMDBModule::Init()
{
    // (Re-)initalize the module.

    // clear table
    ClearValues();

    // scroll to table beginning
    fTableCanvas->SetVsbPosition(0);

    // clear import file name
    fImportFileEntry->SetText("");

    // select default entry in combos
    fTableCombo->Select(EDB_Table_Empty, kTRUE);
    fRangeManipCombo->Select(ERange_All_Elements, kFALSE);
}

//______________________________________________________________________________
void TMDBModule::HandleMouseWheel(Event_t* event)
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
void TMDBModule::OpenImportFile()
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
void TMDBModule::SelectExportFile()
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
void TMDBModule::ImportFile()
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
void TMDBModule::SetBlockValues(UInt_t block, Double_t value)
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
void TMDBModule::SetRingValues(UInt_t ring, Double_t value)
{
    // Set all elements of the ring 'ring' to the value 'value' (inner ring = 1,
    // 2nd ring = 2, ...)

    UInt_t ringRange[11][2] = {{0, 0}, {1, 2}, {3, 5}, {6, 9}, {10, 14}, {15, 20},
                              {21, 27}, {28, 35}, {36, 44}, {45, 54}, {55, 63}};

    // check bounds
    if (ring > 11) return;

    // loop over all elements and change value if element belongs to the
    // specified ring
    UInt_t mid, mid_ring;
    for (UInt_t i = 0; i < gMaxSize; i++)
    {
        // map element to first block
        mid = i % 64;

        if (mid >= ringRange[0][0] && mid <= ringRange[0][1]) mid_ring = 1;
        if (mid >= ringRange[1][0] && mid <= ringRange[1][1]) mid_ring = 2;
        if (mid >= ringRange[2][0] && mid <= ringRange[2][1]) mid_ring = 3;
        if (mid >= ringRange[3][0] && mid <= ringRange[3][1]) mid_ring = 4;
        if (mid >= ringRange[4][0] && mid <= ringRange[4][1]) mid_ring = 5;
        if (mid >= ringRange[5][0] && mid <= ringRange[5][1]) mid_ring = 6;
        if (mid >= ringRange[6][0] && mid <= ringRange[6][1]) mid_ring = 7;
        if (mid >= ringRange[7][0] && mid <= ringRange[7][1]) mid_ring = 8;
        if (mid >= ringRange[8][0] && mid <= ringRange[8][1]) mid_ring = 9;
        if (mid >= ringRange[9][0] && mid <= ringRange[9][1]) mid_ring = 10;
        if (mid >= ringRange[10][0] && mid <= ringRange[10][1]) mid_ring = 11;
        
        // element belongs to specified ring -> change value
        if (mid_ring == ring)
        {
            printf("Updating %d\n", i);
            fElementNewValue[i]->SetNumber(value);
        }
    }
}

//______________________________________________________________________________
void TMDBModule::ClearValues()
{
    // Set all values in the element array displays and entries to 0.
    
    // clear title
    fTableTitle->SetText("Table: none");

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
void TMDBModule::MarkChanges()
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
void TMDBModule::DoRangeManipulation()
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
void TMDBModule::ReadTable(Int_t table)
{
    // Read table from database and display the content.

    Char_t tableName[256];
    Char_t query[256];
    Char_t name[256];

    // Clear current displayed values
    ClearValues();
    
    // Leave if the dummy entry in the combo was selected
    if (table == EDB_Table_Empty) return;

    // try to open connection to MySQL server
    TSQLServer* tapsDB = TSQLServer::Connect(fDBURLEntry->GetText(), 
                                             fDBUserEntry->GetText(), 
                                             fDBPasswdEntry->GetText());
    // exit if connection to DB failed
    if (!tapsDB)
    {
        ModuleError("Could not connect to the database server. Please check your settings!");
        return;
    }
    
    // Choose table and set ID and value data column
    Int_t col_ID, col_VAL;
    if (table == EDB_Table_BaF2_HV) 
    {
        strcpy(tableName, "hvbaf_par");
        col_ID = 0;
        col_VAL = 1;
    }
    else if (table == EDB_Table_BaF2_CFD)
    {
        strcpy(tableName, "cfd_par");
        col_ID = 0;
        col_VAL = 2;
    }
    else if (table == EDB_Table_BaF2_LED1)
    {
        strcpy(tableName, "ledl_par");
        col_ID = 0;
        col_VAL = 2;
    }
    else if (table == EDB_Table_BaF2_LED2)
    {
        strcpy(tableName, "ledh_par");
        col_ID = 0;
        col_VAL = 2;
    }
    else return;


    // display table info
    printf("Reading values from table %s ...\n", tableName);
    sprintf(name, "Table: %s", tableName);
    fTableTitle->SetText(name);
    
    // read values
    sprintf(query, "SELECT * FROM %s", tableName);
    TSQLResult* res = tapsDB->Query(query);
    TSQLRow* row;
    Int_t id, val;
    while ((row = res->Next()))
    {
        id  = atoi(row->GetField(col_ID));
        val = atoi(row->GetField(col_VAL));
        if (id > (Int_t)gBaF2Size) break;

        // set current and new value
        fElementCurrentValue[id-1]->SetText(val);
        fElementNewValue[id-1]->SetNumber(val);
}
    delete res;

    // deconnect from server
    delete tapsDB;
}

