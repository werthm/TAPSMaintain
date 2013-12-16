/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// htmldoc.C                                                            //
//                                                                      //
// ROOT macro for the creation of the HTML documentation.               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
void htmldoc()
{
    gROOT->Reset();
    gSystem->Load("lib/libTAPSMaintain");

    THtml h;
    h.SetSourceDir(".");
    h.SetOutputDir("htmldoc");
    h.SetAuthorTag("* Author:");

    cout << "Sources: " << h.GetSourceDir() << endl;
    h.MakeAll();
}

