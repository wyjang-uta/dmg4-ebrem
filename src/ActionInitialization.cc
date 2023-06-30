#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"

ActionInitialization::ActionInitialization()
{}

ActionInitialization::ActionInitialization(DetectorConstruction* detectorConstruction, DarkMatter* darkMatter)
 : fDetectorConstruction(detectorConstruction),
   fDarkMatter(darkMatter)
{
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  RunAction* runAction = new RunAction;
  SetUserAction(runAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  RunAction* runAction = new RunAction;
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction(runAction, fDetectorConstruction, fDarkMatter);
  SetUserAction(eventAction);

  SetUserAction(new SteppingActionDMG4(fDetectorConstruction, eventAction));
  
}

