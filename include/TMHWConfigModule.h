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


#ifndef TAPSMaintain_TMHWConfigModule
#define TAPSMaintain_TMHWConfigModule

#include "RQ_OBJECT.h"
#include "TMath.h"
#include "TGComboBox.h"
#include "TGFileDialog.h"
#include "TGNumberEntry.h"
#include "TGTableLayout.h"
#include "TGTab.h"
//#include "TGProgressBar.h"
#include "TColor.h"
#include "TSQLServer.h"
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TSystem.h"

#include "TMModule.h"
#include "TMUtils.h"


// C method provided by libtcpip to set the BaF2 high voltage
// in the hardware crate.
extern "C"
{
    void Set_BAF2_HV(char* server, int det, int value); 
    void Init_BAF2_HV(char* server); 
}

enum EDB_TAPS_Table {
    EDB_Table_Empty, 
    EDB_Table_BaF2_HV, 
    EDB_Table_BaF2_CFD,
    EDB_Table_BaF2_LED1,
    EDB_Table_BaF2_LED2,
    EDB_Table_Veto_LED
};


enum {
    ERange_All_Elements,
    ERange_Block_A,
    ERange_Block_B,
    ERange_Block_C,
    ERange_Block_D,
    ERange_Block_E,
    ERange_Block_F,
    ERange_Ring_1,
    ERange_Ring_2,
    ERange_Ring_3,
    ERange_Ring_4,
    ERange_Ring_5,
    ERange_Ring_6,
    ERange_Ring_7,
    ERange_Ring_8,
    ERange_Ring_9,
    ERange_Ring_10,
    ERange_Ring_11,
};


class TMHWConfigModule : public TMModule
{
    RQ_OBJECT("TMHWConfigModule")

private:
    TGCompositeFrame* fControlFrame;                        // frame for the control elements
    TGTextEntry* fDBURLEntry;                               // DB URL entry
    TGTextEntry* fDBUserEntry;                              // DB user entry
    TGTextEntry* fDBPasswdEntry;                            // DB password entry
    TGComboBox* fTableCombo;                                // DB table selection combo box

    TGTab* fSettingsTab;                                    // tab for different settings

    TGCompositeFrame* fRangeManipFrame;                     // frame for range manipulation elements
    TGComboBox* fRangeManipCombo;                           // combo box for range manipulation
    TGNumberEntry* fRangeManipEntry;                        // entry for range manipulation
    TGTextButton* fRangeManipButton;                        // button for range manipulation
    
    TGCompositeFrame* fImportFrame;                         // import frame
    TGTextEntry* fImportFileEntry;                          // import file path entry
    TGTextButton* fImportBrowse;                            // import "Browse" button
    TGTextButton* fImportButton;                            // do import button
    
    TGCompositeFrame* fExportFrame;                         // export frame
    TGTextEntry* fExportFileEntry;                          // export file path entry
    TGTextButton* fExportBrowse;                            // export "Browse" button
    TGTextButton* fExportButton;                            // do export button
    
    TGCompositeFrame* fGMFrame;                             // gain match frame
    TGTextEntry* fGMCalibFileEntry;                         // calibration file path entry
    TGTextButton* fGMCalibBrowse;                           // calibration file "Browse" button
    TGNumberEntry* fGMRangeEntry;                           // range entry
    TGNumberEntry* fGMResEntry;                             // resolution entry
    TGTextButton* fGMButton;                                // perform gain match
 
    TGVerticalFrame* fInputFrame;                           // input frame
    TGLabel* fTableTitle;                                   // displays the currently loaded table
    TGCanvas* fTableCanvas;                                 // table scroll canvas
    TGCompositeFrame* fTableFrame;                          // frame for the table input elements
    TGLabel** fElementCurrentValue;                         // current value display array of all elements
    TGNumberEntry** fElementNewValue;                       // new value entry array of all elements
    TGLabel** fElementValueChanged;                         // value change status array of all elements

    TGCompositeFrame* fButtonsFrame;                        // horizontal frame for the main buttons
    TGTextButton* fWriteDBButton;                           // will write the values to the DB
    TGTextButton* fWriteHWButton;                           // will write the values to the DB
    TGTextButton* fQuitButton;                              // quit module button

    //TGHProgressBar* fProgressBar;                           // progress bar

    Char_t fCurrentTable[256];                              // name of the current table

    void SetBlockValues(UInt_t block, Double_t value);
    void SetRingValues(UInt_t ring, Double_t value);
    Bool_t SetTableSettings(EDB_TAPS_Table table, Char_t* tableName, Char_t* columnName);
    
public:
    TMHWConfigModule() : TMModule() { }
    TMHWConfigModule(const Char_t* name, UInt_t id);
    virtual ~TMHWConfigModule();
    
    void DoRangeManipulation();
    void ClearValues();
    void ReadTable(Int_t table);
    void WriteTable();
    void WriteHVToHardware();
    void MarkChanges();
    void SelectImportFile();
    void SelectExportFile();
    void SelectGMCalibFile();
    void ImportFile();
    void ExportFile();
    void DoGainMatch();
    void HandleMouseWheel(Event_t* event);

    virtual void Init();
    virtual void ReadConfig() { }

    ClassDef(TMHWConfigModule, 0) // Module for setting the TAPS hardware parameters
};

#endif
