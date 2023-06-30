#include "globals.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"

#include "QGSP_BERT.hh"
#include "FTFP_BERT.hh"

#include "DarkMatterPhysics.hh"
#include "DarkMatter.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "G4PhysListFactory.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4ios.hh"

#include "DarkMatterParametersFactory.hh"

int main(int argc,char** argv) {

  // Run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  G4int nThreads = G4Threading::G4GetNumberOfCores();
  runManager->SetNumberOfThreads(nThreads);
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  // UserInitialization classes
  DetectorConstruction* detectorConstruction = new DetectorConstruction;
  runManager->SetUserInitialization(detectorConstruction);

  // ___ Here the "extension" part starts ___
  G4PhysListFactory factory;
  G4VModularPhysicsList * phys = factory.GetReferencePhysList("FTFP_BERT");
  // ^^^ most of the standard physics lists are available by this interface

  //  G4PhysicsListHelper * phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();
  //  phLHelper->DumpOrdingParameterTable();

  DarkMatterPhysics* darkMatterPhysics = new DarkMatterPhysics();
  //bool config = darkMatterPhysics->DarkMatterPhysicsConfigure();
  DarkMatterParametersFactory* DMpar = DarkMatterParametersFactory::GetInstance();
  G4cout << "BiasSigmaFactor0 = " << DMpar->GetRegisteredParam("BiasSigmaFactor0") << G4endl;
  G4cout << "EThresh = " << DMpar->GetRegisteredParam("EThresh") << G4endl;
  G4cout << "DMProcessType = " << DMpar->GetRegisteredParam("DMProcessType") << G4endl;
  G4cout << "DMMass = " << DMpar->GetRegisteredParam("DMMass") << G4endl;
  G4cout << "Epsilon = " << DMpar->GetRegisteredParam("Epsilon") << G4endl;
  phys->RegisterPhysics(darkMatterPhysics);
  // ^^^ Here the "extension" part ends ^^^

  runManager->SetUserInitialization(phys);  // init phys

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer()));


#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  G4VisManager* visManager = new G4VisExecutive;

  visManager->Initialize();
#endif

  // UserAction classes
  //runManager->SetUserAction(runAction);
  /*
  runManager->SetUserAction(new PrimaryGeneratorAction(detectorConstruction));

  RunAction* runAction = new RunAction();
  EventAction* myEA = new EventAction(detectorConstruction, darkMatterPhysics->GetDarkMatterPointer());
  runManager->SetUserAction(runAction);
  runManager->SetUserAction(myEA);
  runManager->SetUserAction(new SteppingActionDMG4(detectorConstruction, myEA));
  */

  // User interactions
  G4UImanager * UI = G4UImanager::GetUIpointer();  

  if(argc==1)
  // Define (G)UI terminal for interactive mode  
  { 
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = new G4UIterminal;
    UI->ApplyCommand("/control/execute prerun.g4mac");    
    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

