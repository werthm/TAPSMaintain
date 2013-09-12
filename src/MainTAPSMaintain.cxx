// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MainTAPSMaintain                                                     //
//                                                                      //
// Contains the main method for the TAPSMaintain application.           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TApplication.h"

#include "TTMySQLManager.h"
#include "TTServerManager.h"

#include "TMTAPSMaintain.h"


//______________________________________________________________________________
int main(int argc, char* argv[])
{
    // TAPSMaintain application main method
    
    // try to load TAPSsc configuration file 
    Char_t tmp[256];
    sprintf(tmp, "%s/config/config.rootrc", gSystem->Getenv("TAPSSC"));
    if (gEnv->ReadFile(tmp, kEnvLocal))
    {
        Error("main", "Could not find configuration file - check the TAPSSC environment variable!");
        return -1;
    }
    
    // init MySQL and network server managers
    TTMySQLManager::GetManager();
    TTServerManager::GetManager();

    // init TAPSMaintain
    TApplication app("TAPSMaintain", 0, 0);
    TMTAPSMaintain tm;
    
    // try to start TAPSMaintain
    if (tm.Start()) return -1;
    
    // open ROOT file if specified
    if (argc == 2)
    {
        if (!tm.OpenInputFile(argv[1])) Error("main", "Could not open ROOT input file '%s'!", argv[1]);
    }
    
    app.Run();

    return 0;
}

