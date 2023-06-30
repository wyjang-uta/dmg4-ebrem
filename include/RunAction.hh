#ifndef BremRunAction_h
#define BremRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Version.hh"

// version dependent header files
#if G4VERSION_NUMBER != 1100
#include "g4root.hh"  // G4_10
#else
#include "G4AnalysisManager.hh" // G4_11
#endif

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;


  private:

};

#endif

