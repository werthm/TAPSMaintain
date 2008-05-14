// SVN Info: $Id$

/*************************************************************************
 * Author: Dominik Werthmueller, 2008
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TMModule                                                             //
//                                                                      //
// Abstract class representing a module.                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TAPSMaintain_TMModule
#define TAPSMaintain_TMModule

#include "TNamed.h"
#include "TGFrame.h"
#include "TGLabel.h"
#include "TGWidget.h"
#include "TFile.h"

#include "TMConfig.h"


class TMModule : public TNamed, public TQObject
{

private:
    UInt_t fID;                                 // Module ID
    Bool_t fNeedsTFile;                         // Flag if module needs ROOT input file or not
    UInt_t fNresults;                           // Number of results per element
    Double_t** fResults;                        // Result array of this module

protected:    
    TFile* fFile;                               // ROOT input file
    TGCompositeFrame* fFrame;                   // Module GUI frame

public:
    TMModule();
    TMModule(const Char_t* name, UInt_t id, UInt_t inNresults, Bool_t needsROOTInputFile);
    virtual ~TMModule();
    
    UInt_t GetID() { return fID; }
    Bool_t NeedsRootInputFile() { return fNeedsTFile; }
    UInt_t GetNresults() { return fNresults; }
    Double_t** GetResults() { return fResults; }
    TGCompositeFrame* GetFrame() { return fFrame; }
    
    void SetResult(UInt_t element, UInt_t resultNumber, Double_t result);
    void SetRootInputFile(TFile* f) { fFile = f; }
    
    void ClearResults();
    void DumpResults(const Char_t* numberFormat = "%9.7e");
    
    void Finished() { Emit("Finished()"); }         // *SIGNAL*  
    
    virtual void Init() = 0;                        // (Re-)initalize the module

    ClassDef(TMModule, 0) // Abstract module class
};

#endif

