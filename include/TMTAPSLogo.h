/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMTAPSLogo                                                           //
//                                                                      //
// TAPS logo in the GIMP header image file format (RGB).                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMTAPSLogo
#define TAPSMaintain_TMTAPSLogo

#include "Rtypes.h"

/*  Call this macro repeatedly.  After each use, the pixel data can be extracted  */

#define HEADER_PIXEL(data,pixel) {\
  pixel[0] = (((data[0] - 33) << 2) | ((data[1] - 33) >> 4)); \
  pixel[1] = ((((data[1] - 33) & 0xF) << 4) | ((data[2] - 33) >> 2)); \
  pixel[2] = ((((data[2] - 33) & 0x3) << 6) | ((data[3] - 33))); \
  data += 4; \
}

#ifndef __CINT__


extern const UInt_t kTAPSLogo_width;
extern const UInt_t kTAPSLogo_height;
extern const Char_t* kTAPSLogo_data; 

#endif
#endif

