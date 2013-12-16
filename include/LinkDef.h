/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2013
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// LinkDef.h                                                            //
//                                                                      //
// TAPSMaintain dictionary header file.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifdef __CINT__ 

// turn everything off
#pragma link off all globals; 
#pragma link off all classes; 
#pragma link off all functions; 
#pragma link off all typedef; 

#pragma link C++ nestedclasses; 
#pragma link C++ nestedtypedef; 

// TAPSMaintain classes
#pragma link C++ class TMTAPSMaintain+; 
#pragma link C++ class TMModuleLoader+; 
#pragma link C++ class TMMainFrame+; 
#pragma link C++ class TMModule+; 
#pragma link C++ class TMSeqCalibModule+; 
#pragma link C++ namespace TMUtils; 

// Modules
#pragma link C++ class TMHWConfigModule+; 
#pragma link C++ class TMCalibCosmics+; 
#pragma link C++ class TMCalibLED+; 
#pragma link C++ class TMCheckSpectra+; 
#pragma link C++ class TMCalibTDC+; 

#endif

