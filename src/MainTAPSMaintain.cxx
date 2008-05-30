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

#include "TMTAPSMaintain.h"


//______________________________________________________________________________
int main(int argc, char* argv[])
{
    // TAPSMaintain application main method
    
    TApplication app("TAPSMaintain", 0, 0);

    TMTAPSMaintain tm;
    
    // try to start TAPSMaintain
    if (tm.Start()) return -1;
    
    // open ROOT file if specified
    if (argc == 2)
    {
        if (!tm.OpenInputFile(argv[1])) printf("ERROR: Could not open ROOT input file '%s'!\n", argv[1]);
    }
    
    app.Run();

    return 0;
}

