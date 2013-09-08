// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2010
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMConfig                                                             //
//                                                                      //
// TAPSMaintain configuration.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMConfig
#define TAPSMaintain_TMConfig

#include "Rtypes.h"


// TAPSMaintain version
extern const Char_t kTAPSMaintainVersion[];

// number of TAPS detector elements
extern const UInt_t kMaxSize;
extern const UInt_t kBaF2Size;
extern const UInt_t kPbWO4Size;
extern const UInt_t kVetoSize;
extern const UInt_t kPWOVetoSize;

// energy loss of minimal ionizing particles in the TAPS detectors [MeV]
extern const Double_t kTAPS_MIP_Loss_BaF2;
extern const Double_t kTAPS_MIP_Loss_PbWO4;
extern const Double_t kTAPS_MIP_Loss_Veto;

// BaF2 HV calibration gain slope
extern const Double_t kBaF2_Gain_Slope;

// TDC cable delay measurements
extern const Double_t kTDC_Cable_Delay[];
extern const Double_t kTDC_Cable_Delay_Error[];

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

