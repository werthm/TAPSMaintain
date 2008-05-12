// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
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

// Modules
#pragma link C++ class TMCalibLED+; 

#endif

