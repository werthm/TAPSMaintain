// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
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
static const Char_t gTAPSMaintainVersion[] = "0.1.0";

// default TAPS database settings
static const Char_t gTAPS_DB_URL[] = "mysql://localhost/taps";
static const Char_t gTAPS_DB_User[] = "root";
static const Char_t gTAPS_DB_Passwd[] = "";

// default TAPS hardware server
static const Char_t gTAPS_Server[] = "taps01";

// number of TAPS detector elements
const UInt_t gMaxSize   = 384;
const UInt_t gBaF2Size  = 384;
const UInt_t gPbWO4Size = 24;
const UInt_t gVetoSize  = 384;

// energy loss of minimal ionizing particles in the TAPS detectors [MeV]
const Double_t gTAPS_MIP_Loss_BaF2  = 37.7;
const Double_t gTAPS_MIP_Loss_PbWO4 = 22.6;   // ?
const Double_t gTAPS_MIP_Loss_Veto  = 5;      // ?

// BaF2 HV calibration gain slope
const Double_t gBaF2_Gain_Slope = 153;

// define TAPS detectors
enum ETAPSDetector {
    kNo_Detector,
    kBaF2_Detector,
    kPbWO4_Detector,
    kVeto_Detector    
};
typedef ETAPSDetector TAPSDetector_t;

#endif

