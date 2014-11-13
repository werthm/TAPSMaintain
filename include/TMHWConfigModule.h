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


#ifndef TAPSMaintain_TMHWConfigModule
#define TAPSMaintain_TMHWConfigModule

#include "RQ_OBJECT.h"

#include "TMModule.h"

class TGTab;
class TGNumberEntry;
class TGLabel;
class TGCanvas;
class TGHProgressBar;
class TCanvas;
class TF1;
class TGraph;

// Hardcoded values used for QT signal connections
enum EFILE_BROWSE_SELECT {
    kFILE_SELECT_IMPORT             = 1,
    kFILE_SELECT_EXPORT             = 2,
    kFILE_SELECT_GM_ENERGY_CALIB    = 3,
    kFILE_SELECT_LED_ENERGY_CALIB   = 4,
    kFILE_SELECT_LED_CALIB          = 5
};

class TMHWConfigModule : public TMModule
{
    RQ_OBJECT("TMHWConfigModule")

private:
    TGCompositeFrame* fControlFrame;                        // frame for the control elements
    TGComboBox* fTableCombo;                                // DB table selection combo box
    TList* fParTypes;                                       // list of parameter data types

    TGTab* fSettingsTab;                                    // tab for different settings

    TGCompositeFrame* fSetValueFrame;                       // frame for value setting controls
    TGCheckButton* fHexFormat;                              // display in hex format
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
    TGCheckButton* fExportColumn;                           // checkbox to select current or new values to export
    TGTextButton* fExportButton;                            // do export button
    
    TGCompositeFrame* fGMFrame;                             // gain match frame
    TGTextEntry* fGMCalibFileEntry;                         // calibration file path entry
    TGTextButton* fGMCalibBrowse;                           // calibration file "Browse" button
    TGNumberEntry* fGMRangeEntry;                           // range entry
    TGNumberEntry* fGMResEntry;                             // resolution entry
    TGTextButton* fGMButton;                                // perform gain match
    
    TGCompositeFrame* fLEDFrame;                            // LED settings frame
    TGTextEntry* fLEDEnergyCalibFileEntry;                  // LED energy calibration file path entry
    TGTextButton* fLEDEnergyCalibBrowse;                    // LED energy calibration file "Browse" button
    TGTextEntry* fLEDCalibFileEntry;                        // LED calibration file path entry
    TGTextButton* fLEDCalibBrowse;                          // LED calibration file "Browse" button
    TGTextButton* fLEDAddFileButton;                        // 'add file to LED calibration' button
    TGComboBox* fLEDRangeCombo;                             // combo box for LED range selection
    TGNumberEntry* fLEDChannelEntry;                        // number of the channel to show the fit of
    TGTextButton* fLEDShowChannelButton;                    // show the fit of a specific channel
    TGNumberEntry* fLEDThrEntry;                            // threshold value entry
    TGTextButton* fLEDSetThrButton;                         // set thresholds to specified value
    
    TGVerticalFrame* fInputFrame;                           // input frame
    TGLabel* fTableTitle;                                   // displays the currently loaded table
    TGCanvas* fTableCanvas;                                 // table scroll canvas
    TGCompositeFrame* fTableFrame;                          // frame for the table input elements
    TGCompositeFrame** fElementFrame;                       // array of frames for the table rows
    TGLabel** fElementCurrentValue;                         // current value display array of all elements
    TGNumberEntry** fElementNewValue;                       // new value entry array of all elements
    TGLabel** fElementValueChanged;                         // value change status array of all elements

    TGCompositeFrame* fButtonsFrame;                        // horizontal frame for the main buttons
    TGTextButton* fWriteDBButton;                           // will write the values to the DB
    TGTextButton* fWriteADButton;                           // will write AcquDAQ configuration files
    TGTextButton* fQuitButton;                              // quit module button

    TGCompositeFrame* fHVCtrlFrame;                         // frame for the HV control buttons
    TGTextButton* fHVReadButton;                            // will read status of HV mainframes
    TGTextButton* fHVToggleButton[2];                       // will change status of HV mainframe 1/2
    TGTextButton* fWriteHWButton;                           // will write the values to the hardware
    TGTextButton* fReadHWButton;                            // will read the values from the hardware
     
    TGHProgressBar* fProgressBar;                           // progress bar
    
    TCanvas* fExternalCanvas;                               // external canvas

    TF1** fLEDFitFunctions;                                 // fit functions for the LED setting
    TGraph** fLEDGraphs;                                    // graph for the LED setting
    UInt_t fNLEDCalibSets;                                  // number of LED calibration values per channel

    void SetBlockValues(UInt_t block, Double_t value);
    void SetRingValues(UInt_t ring, Double_t value);
    void CreateExternalCanvas(UInt_t n);
    Bool_t CheckValueLimits(Int_t tableIndex, Double_t value);    
    void ClearValues();
    void GainMatchBaF2(const Char_t* filename, Double_t min, Double_t max);
    void GainMatchPWO(const Char_t* filename, Double_t min, Double_t max);

public:
    TMHWConfigModule() : TMModule() { }
    TMHWConfigModule(const Char_t* name, UInt_t id);
    virtual ~TMHWConfigModule();
    
    void DoRangeManipulation();
    void ReadTable(Int_t table);
    void LEDRangeChanged(Int_t id);
    void WriteTable();
    void ReadHVStatus();
    void ChangeHVStatus(Int_t id);
    void WriteHVToHardware();
    void ReadHVFromHardware();
    void MarkChanges();
    void SelectFile(Int_t ftype);
    void ShowLEDCalibration();
    void SetLEDThresholds();
    void ImportFile();
    void ExportFile();
    void DoGainMatch();
    void AddFileToLEDCalibration();
    void HandleMouseWheel(Event_t* event);
    void ToggleHexFormat();
    void WriteADConfigs();
    void DestroyExternalCanvas();

    virtual void Init();
    virtual void ReadConfig() { }
    virtual void Cleanup();

    ClassDef(TMHWConfigModule, 0) // Module for setting the TAPS hardware parameters
};

#endif

