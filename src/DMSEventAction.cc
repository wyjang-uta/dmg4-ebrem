//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file DMSEventAction.cc
/// \brief Implementation of the DMSEventAction class

#include "DMSEventAction.hh"
#include "DMSRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSEventAction::DMSEventAction(DMSRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSEventAction::~DMSEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DMSEventAction::BeginOfEventAction(const G4Event* evt)
{
  // Beam time structure simulation
  G4PrimaryVertex* pv = evt->GetPrimaryVertex();

  G4int nPtlsPerBunch = 10000;   // 1e+4 particles per bunch
  G4int evtId = evt->GetEventID();
  G4double newT0 = ( 0. + evtId * 1e-4 + evtId / nPtlsPerBunch * 6.2 ) * ns;

  // Set new T0
  pv->SetT0(newT0);


  // Print progress
  G4int eventID = evt->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Processing event: " << eventID << " completed." << G4endl;
    G4cout << "Beam Time Structure Formation" << G4endl;
    G4cout << "Primary vertex T0 is shifted to " << newT0 << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DMSEventAction::EndOfEventAction(const G4Event* event)
{
  // periodic printing

  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Processing event: " << eventID << " completed." << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
