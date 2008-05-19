// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMModuleLoader                                                       //
//                                                                      //
// This class loads and holds the available modules.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMModuleLoader
#define TAPSMaintain_TMModuleLoader

#include "TList.h"

#include "TMCalibCosmics.h"
#include "TMCalibLED.h"


class TMModuleLoader : public TObject
{

private:
    TList* fModules;                            // list containing the modules

public:
    TMModuleLoader();
    TMModuleLoader(UInt_t startID);
    virtual ~TMModuleLoader();
    
    TList* GetModules() const { return fModules; }
    TMModule* GetModuleWithID(UInt_t id) const;

    ClassDef(TMModuleLoader, 0) // Module loader class
};

#endif

