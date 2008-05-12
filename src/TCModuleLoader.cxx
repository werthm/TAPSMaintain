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


#include "TMModuleLoader.h"

ClassImp(TMModuleLoader)


//______________________________________________________________________________
TMModuleLoader::TMModuleLoader()
    : TObject()
{
    // Default constructor.
 
    fModules = 0;
}

//______________________________________________________________________________
TMModuleLoader::TMModuleLoader(UInt_t startID)
    : TObject()
{
    // Constructor.
 
    // create modules list (is owner)
    fModules = new TList();
    fModules->SetOwner(kTRUE);
    
    // ---------------------------- Load modules ----------------------------
    UInt_t numberOfModules = startID;
    
    // LED calibration module
    TMCalibLED* mod1 = new TMCalibLED("LED Calibration", numberOfModules++);
    fModules->Add(mod1);
}

//______________________________________________________________________________
TMModuleLoader::~TMModuleLoader()
{
    // Destructor.

    if (fModules) delete fModules;
}

//______________________________________________________________________________
TMModule* TMModuleLoader::GetModuleWithID(UInt_t id)
{
    // Return the module with the ID inID. If no such module exists return 0.
    
    // loop over modules
    for (Int_t i = 0; i < fModules->GetSize(); i++)
    {
        TMModule* m = (TMModule*) fModules->At(i);
        if (m->GetID() == id) return m;
    }
    
    return 0;
}

