/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2017
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMConfig                                                             //
//                                                                      //
// TAPSMaintain configuration.                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TMConfig.h"

// TAPSMaintain version
const Char_t kTAPSMaintainVersion[] = "0.4.0";

// number of TAPS detector elements
const UInt_t kMaxSize     = 384;
const UInt_t kBaF2Size    = 384;
const UInt_t kPbWO4Size   = 72;
const UInt_t kVetoSize    = 384;
const UInt_t kPWOVetoSize = 18;

// energy loss of minimal ionizing particles in the TAPS detectors [MeV]
const Double_t kTAPS_MIP_Loss_BaF2  = 37.7;
const Double_t kTAPS_MIP_Loss_PbWO4 = 29.5;
const Double_t kTAPS_MIP_Loss_Veto  = 5;      // ?

// BaF2 HV calibration gain slope
const Double_t kBaF2_Gain_Slope = 153;

// TDC cable delay measurements
const Double_t kTDC_Cable_Delay[] = {0.0, 4.895, 10.060, 20.355,
                                   30.915, 41.205, 50.910};
const Double_t kTDC_Cable_Delay_Error[] = {0.001, 0.005, 0.03, 0.005,
                                         0.005, 0.005, 0.01};


