#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
	// set printing event number per each event
	G4RunManager::GetRunManager()->SetPrintProgress(1);
	
	
	
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	
	// Create directories
	analysisManager->SetVerboseLevel(1);
//	analysisManager->CreateH1("Energy"," Energy in each step", 5000, 0., 100*GeV);
//	analysisManager->CreateH1("Tracklength","Tracklength/X_{0}", 5000, 0., 20.);
//	analysisManager->CreateH2("TrckLenVsEnergy","Tracklength/X_{0} vs Energy", 5000, 0., 100*GeV,
//														5000, 0., 20.);
	analysisManager->SetNtupleMerging(true);
	// Creating ntuple
	analysisManager->CreateNtuple("brem", "e- bremsstrahlung in C-12");
	analysisManager->CreateNtupleIColumn("TrackID");
	analysisManager->CreateNtupleDColumn("fKineticEnergy");
	analysisManager->CreateNtupleDColumn("fTrackLength_X0");
	analysisManager->CreateNtupleSColumn("fProcess");
	analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// default mode
void RunAction::BeginOfRunAction(const G4Run*)
{
	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	// set output file name
	G4String fileName = "bremOutput.root";
	analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run*)
{
	auto analysisManager = G4AnalysisManager::Instance();
	//write and close the file at the end of each run
	analysisManager->Write();
	analysisManager->CloseFile();
}
