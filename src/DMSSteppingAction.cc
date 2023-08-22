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
/// \file DMSSteppingAction.cc
/// \brief Implementation of the DMSSteppingAction class

#include "DMSSteppingAction.hh"
#include "DMSEventAction.hh"
#include "DMSDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleTypes.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSSteppingAction::DMSSteppingAction()
: G4UserSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSSteppingAction::~DMSSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DMSSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto analysisManager = G4AnalysisManager::Instance();

  G4StepPoint* postStep = step->GetPostStepPoint();
  G4StepPoint* preStep = step->GetPreStepPoint();
  G4Track* track = step->GetTrack();
  if( !postStep ) return;
  if( !preStep  ) return;
  if( !track ) return;
  G4VPhysicalVolume* postVol = postStep->GetPhysicalVolume();
  G4VPhysicalVolume* preVol = postStep->GetPhysicalVolume();
  if( !postVol ) return;
  if( !preVol  ) return;
  if( postStep->GetPhysicalVolume()->GetName() != "physDetector" ) return;
  if( preStep->GetPhysicalVolume()->GetName() == "physDetector" ) return; // deny double counting.

  if( track->GetDefinition()->GetParticleName() == "nu_e" ) return;
  if( track->GetDefinition()->GetParticleName() == "anti_nu_e" ) return;
  if( track->GetDefinition()->GetParticleName() == "nu_mu" ) return;
  if( track->GetDefinition()->GetParticleName() == "anti_nu_mu" ) return;

  analysisManager->FillNtupleSColumn(0, track->GetDefinition()->GetParticleName() );
  analysisManager->FillNtupleSColumn(1, track->GetCreatorProcess()->GetProcessName() );
  analysisManager->FillNtupleDColumn(2, track->GetParentID());
  analysisManager->FillNtupleDColumn(3, track->GetTrackID());
  analysisManager->FillNtupleDColumn(4, track->GetCurrentStepNumber());
  analysisManager->FillNtupleDColumn(5, (G4double)track->GetKineticEnergy()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(6, (G4double)track->GetTotalEnergy()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(7, (G4double)track->GetPosition().getX()/CLHEP::cm);
  analysisManager->FillNtupleDColumn(8, (G4double)track->GetPosition().getY()/CLHEP::cm);
  analysisManager->FillNtupleDColumn(9, (G4double)track->GetPosition().getZ()/CLHEP::cm);
  analysisManager->FillNtupleDColumn(10, (G4double)track->GetMomentum().getX()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(11, (G4double)track->GetMomentum().getY()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(12, (G4double)track->GetMomentum().getZ()/CLHEP::MeV);
  analysisManager->FillNtupleDColumn(13, (G4double)track->GetGlobalTime()/CLHEP::ns);
  analysisManager->FillNtupleDColumn(14, (G4double)track->GetLocalTime()/CLHEP::ns);
  analysisManager->AddNtupleRow();

  /*
  const std::vector<const G4Track*>* secondary = step->GetSecondaryInCurrentStep();
  for( size_t lp=0; lp < (*secondary).size(); lp++ )
  {
    // secondary particle name
    analysisManager->FillNtupleSColumn(0, (*secondary)[lp]->GetDefinition()->GetParticleName() );
    // production process of secondary particle
    analysisManager->FillNtupleSColumn(1, (*secondary)[lp]->GetCreatorProcess()->GetProcessName());
    // parent id
    analysisManager->FillNtupleDColumn(2, (*secondary)[lp]->GetParentID());
    // track id
    analysisManager->FillNtupleDColumn(3, (*secondary)[lp]->GetTrackID());
    // step number
    analysisManager->FillNtupleDColumn(4, (*secondary)[lp]->GetCurrentStepNumber());
    // kineticEnergy
    analysisManager->FillNtupleDColumn(5, (G4double)(*secondary)[lp]->GetKineticEnergy()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(6, (G4double)(*secondary)[lp]->GetTotalEnergy()/CLHEP::MeV);
    // vertex coordinates
    analysisManager->FillNtupleDColumn(7, (G4double)(*secondary)[lp]->GetPosition().getX()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(8, (G4double)(*secondary)[lp]->GetPosition().getY()/CLHEP::cm);
    analysisManager->FillNtupleDColumn(9, (G4double)(*secondary)[lp]->GetPosition().getZ()/CLHEP::cm);
    // momentum
    analysisManager->FillNtupleDColumn(10, (G4double)(*secondary)[lp]->GetMomentum().getX()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(11, (G4double)(*secondary)[lp]->GetMomentum().getY()/CLHEP::MeV);
    analysisManager->FillNtupleDColumn(12, (G4double)(*secondary)[lp]->GetMomentum().getZ()/CLHEP::MeV);
    // time info
    analysisManager->FillNtupleDColumn(13, (G4double)(*secondary)[lp]->GetGlobalTime()/CLHEP::ns);
    analysisManager->FillNtupleDColumn(14, (G4double)(*secondary)[lp]->GetLocalTime()/CLHEP::ns);
    analysisManager->AddNtupleRow();
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

