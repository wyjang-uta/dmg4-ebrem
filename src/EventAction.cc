#include "globals.hh"

#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "SteppingActionDMG4.hh"

#include "DarkMatter.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

EventAction::EventAction(
  RunAction* runAction,
  DetectorConstruction* detectorConstruction,
  DarkMatter* darkMatter
) : fRunAction(runAction), fDetectorConstruction(detectorConstruction), fDarkMatter(darkMatter), NEmissions(0)
{
}


EventAction::~EventAction()
{
  G4cout << "Total number of DM emissions = " << NEmissions << G4endl;
  ofstream outFile("Report.txt");
  if(NEmissions >= 3) outFile << "Total number of DM emissions = " << NEmissions << G4endl;
  outFile.close();
}


void EventAction::BeginOfEventAction(const G4Event* event)
{
  fSteppingActionDMG4->Reset();

  fDetectorConstruction->SetAEmission(0);
}


void EventAction::EndOfEventAction(const G4Event* evt)
{
  fSteppingActionDMG4->Finalize();

  G4cout << ">>> End of event " << evt->GetEventID() << endl;  
}
