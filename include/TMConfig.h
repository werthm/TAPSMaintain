// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMConfig                                                             //
//                                                                      //
// TAPSMaintain configuration header file.                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMConfig
#define TAPSMaintain_TMConfig


// TAPSMaintain version
static const Char_t gTAPSMaintainVersion[] = "0.1.4";

// default TAPS database settings
static const Char_t gTAPS_DB_URL[] = "mysql://localhost/taps";
static const Char_t gTAPS_DB_User[] = "taps";
static const Char_t gTAPS_DB_Passwd[] = "taps2003";

// default TAPS hardware server
static const Char_t gTAPS_Server[] = "localhost";

// number of TAPS detector elements
const UInt_t gMaxSize     = 384;
const UInt_t gBaF2Size    = 384;
const UInt_t gPbWO4Size   = 72;
const UInt_t gVetoSize    = 384;
const UInt_t gPWOVetoSize = 18;

// energy loss of minimal ionizing particles in the TAPS detectors [MeV]
const Double_t gTAPS_MIP_Loss_BaF2  = 37.7;
const Double_t gTAPS_MIP_Loss_PbWO4 = 29.5;
const Double_t gTAPS_MIP_Loss_Veto  = 5;      // ?

// BaF2 HV calibration gain slope
const Double_t gBaF2_Gain_Slope = 153;

// TAPS DB limits
const Double_t gDB_BaF2_HV_Min = 0.;
const Double_t gDB_BaF2_HV_Max = 2000.;
const Double_t gDB_BaF2_CFD_Min = 1.;
const Double_t gDB_BaF2_CFD_Max = 10000.;
const Double_t gDB_BaF2_LED_Min = -999.;
const Double_t gDB_BaF2_LED_Max = -1.;
const Double_t gDB_Veto_LED_Min = -999.;
const Double_t gDB_Veto_LED_Max = -1.;
const Double_t gDB_Veto_HV_Min = 0.;
const Double_t gDB_Veto_HV_Max = 1000.;

// define TAPS detectors
enum ETAPSDetector 
{
    kNo_Detector,
    kBaF2_Detector,
    kPbWO4_Detector,
    kVeto_Detector    
};
typedef ETAPSDetector TAPSDetector_t;

#endif

