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


#ifndef TAPSCalib_TMDBModule
#define TAPSCalib_TMDBModule

#include "RQ_OBJECT.h"
#include "TGComboBox.h"

#include "TMModule.h"


enum {
    EDBModule_BaF2_HV, 
    EDBModule_BaF2_CFD,
    EDBModule_BaF2_LED1,
    EDBModule_BaF2_LED2
};


class TMDBModule : public TMModule
{
    RQ_OBJECT("TMDBModule")

private:
    TGHorizontalFrame* fControlFrame;                       // frame for the control elements
    TGComboBox* fTableCombo;                                // DB table selection combo box
    
    TGHorizontalFrame* fTableFrame;                         // frame for the value input table
    TGCanvas* fTableCanvas;                                 // table scroll canvas
    TGVerticalFrame* fTableInputFrame;                      // table input frame

public:
    TMDBModule() : TMModule() { }
    TMDBModule(const Char_t* name, UInt_t id);
    virtual ~TMDBModule();
    
    virtual void Init() { }
    virtual void ReadConfig() { }

    ClassDef(TMDBModule, 0) // TAPS database manipulation module
};

#endif

