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
/// \file DMSDetectorConstruction.cc
/// \brief Implementation of the DMSDetectorConstruction class

#include "DMSDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSDetectorConstruction::DMSDetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DMSDetectorConstruction::~DMSDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DMSDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  // Geometry parameters
  // World
  const G4double WorldXY = 20.0*m;
  const G4double WorldZ  = 50.0*m;
  // W dump
  const G4double DumpRadius = 0.5 * m;
  const G4double DumpDepth = 1.0 * m;
  // Neutron Moderator
  const G4double ModeratorCoverRadius = 0.5 * m;
  const G4double ModeratorThickness = 0.2 * m; // thickness of the moderator is 20 cm
  // Vacuum vessel parameters
  const G4double pVacVesselThickness = 0.6*cm;
  const G4double pVacVesselRmax = 5.00*m;
  const G4double pVacVesselRmin = pVacVesselRmax - pVacVesselThickness; // 4.994*m
  const G4double pVacVesselDz    = 10.00*m;  // length
  const G4double pVacVesselSphi  = 0; // radian
  const G4double pVacVesselDphi  = 2.0*M_PI;
  const G4double pVacVesselUpStreamEndcapRmin = 0.;
  const G4double pVacVesselUpStreamEndcapRmax = pVacVesselRmin;
  const G4double pVacVesselUpStreamEndcapDz   = pVacVesselThickness;
  // Detector box parameters
  const G4double DetectorX = 10.0 * m;
  const G4double DetectorY = 10.0 * m;
  const G4double DetectorDepth = 2 * m;

  // positions
  const G4double pVacVesselZ = -0.4*m;
  const G4double ModeratorCoverZ = pVacVesselZ - 0.5 * ( pVacVesselDz + ModeratorThickness + pVacVesselThickness );
  const G4double DumpZ = ModeratorCoverZ - 0.5 * (ModeratorThickness + DumpDepth);
  const G4double DetectorPositionZ = -0.4*m + 0.5 * ( pVacVesselDz + pVacVesselThickness + DetectorDepth );

  // Material Definitions
  G4Material* WorldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* DumpMaterial = nist->FindOrBuildMaterial("G4_W");
  // Polyurethane C3H8N2O1
  // Element definition from https://apc.u-paris.fr/~franco/g4doxy4.10/html/class_materials.html
  // See the constructor source code line 54.
  //
  //                                          Z, A
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", 1, 1.00794*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", 7, 14.00674*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", 8, 15.9994*g/mole);
  G4Material* polyurethane = new G4Material(
      "Polyurethane",   // its name
      1100*kg/m3,       // its density
      4,                // its number of elements
      kStateSolid);     // its state
  G4int natoms=3;
  polyurethane->AddElement(C, natoms);
  natoms=8;
  polyurethane->AddElement(H, natoms);
  natoms=2;
  polyurethane->AddElement(N, natoms);
  natoms=1;
  polyurethane->AddElement(O, natoms);
  G4Material* mod_mat = polyurethane;
  //G4Material* mod_mat = nist->FindOrBuildMaterial("G4_B"); // for moderator test with boron

  G4Material* vacuumVessel_mat = nist->FindOrBuildMaterial("G4_Fe");
  G4Material* DetectorMaterial = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*WorldXY, 0.5*WorldXY, 0.5*WorldZ);//its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        WorldMaterial,       //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  G4Tubs* solidDump =
    new G4Tubs("WDump",  //its name
        0,               // inner radius
        DumpRadius,          // outer radius
        0.5 * DumpDepth,    // half-length
        0,
        2.* M_PI * rad);

  G4LogicalVolume* logicDump =
    new G4LogicalVolume(solidDump,            //its solid
                        DumpMaterial,             //its material
                        "Dump");              //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0, 0, DumpZ),         //at (0,0,0)
                    logicDump,               //its logical volume
                    "Dump",                  //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking


  G4Tubs* solid_mod_volume =
    new G4Tubs("mod_volume",
        0.,           // inner radius
        ModeratorCoverRadius,        // outer raduis
        0.5 * ModeratorThickness,  // half-length of the moderator
        0.,           // starting phi
        2.* M_PI * rad ); // ending phi (full cylinder)
  G4LogicalVolume* logic_mod_volume =
    new G4LogicalVolume(solid_mod_volume,
        mod_mat,
        "mod_volume");
  new G4PVPlacement(
      0,                                              // no rotation
      G4ThreeVector(0, 0, ModeratorCoverZ), // translation
      logic_mod_volume,                               // its logical volume
      "mod_volume",                                   // its name
      logicWorld,                                     // its mother volume
      false,                                          // no boolean operation
      0,                                              // a copy number
      checkOverlaps);                                 // overlaps checking

  // Neutron moderator sidewall
  G4Tubs* solid_mod_sidewall =
    new G4Tubs("mod_sidewall",
        ModeratorCoverRadius,
        ModeratorCoverRadius + ModeratorThickness,
        0.5*(DumpDepth + ModeratorThickness),
        0.,
        2.* M_PI * rad );
  G4LogicalVolume* logic_mod_sidewall =
    new G4LogicalVolume(solid_mod_sidewall,
        mod_mat,
        "mod_sidewall");
  G4Colour yellow(1.0, 1.0, 0.0);
  G4VisAttributes* ModeratorVisAttributes = new G4VisAttributes(yellow);
  logic_mod_volume->SetVisAttributes(ModeratorVisAttributes);
  logic_mod_sidewall->SetVisAttributes(ModeratorVisAttributes);

  new G4PVPlacement(
      0,                              // no rotation
      G4ThreeVector(0, 0, DumpZ + 0.5*ModeratorThickness), // translation
      logic_mod_sidewall,             // its logical volume
      "mod_sidewall",                 // its name
      logicWorld,                     // its mother volume
      false,                          // no boolean operation
      0,                              // a copy number
      checkOverlaps);                 // overlaps checking

  //
  // Vacuum Vessel
  //

  G4Tubs* solidVacuumVessel = new G4Tubs(
      "vessel",
      pVacVesselRmin,
      pVacVesselRmax,
      0.5*pVacVesselDz,   // half-length
      pVacVesselSphi,
      pVacVesselDphi);

  G4LogicalVolume* logicalVacuumVessel =
    new G4LogicalVolume(solidVacuumVessel,
        vacuumVessel_mat,
        "vessel");

  new G4PVPlacement(0,
      G4ThreeVector(0, 0, pVacVesselZ),
      logicalVacuumVessel,
      "physVessel",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidVacuumVesselEndcap = new G4Tubs(
      "vesselUpstreamEndcap",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalVacuumVesselUpstreamEndcap =
    new G4LogicalVolume(solidVacuumVesselEndcap,
        vacuumVessel_mat,
        "vesselUpstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, pVacVesselZ - (0.5 * ( pVacVesselDz - pVacVesselThickness ) ) ),
      logicalVacuumVesselUpstreamEndcap,
      "physVesselUpstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);
  G4LogicalVolume* logicalVacuumVesselDownstreamEndcap =
    new G4LogicalVolume(solidVacuumVesselEndcap,
        vacuumVessel_mat,
        "vesselDownstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, pVacVesselZ + ( 0.5 * ( pVacVesselDz - pVacVesselThickness ))),
      logicalVacuumVesselDownstreamEndcap,
      "physVesselDownstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);


  // Detector
  G4Box* solidDetector =
      new G4Box("Detector",     // its name
                0.5*DetectorX,  // half-length in X
                0.5*DetectorY,  // half-length in Y
                0.5*DetectorDepth); // half-length in Z

  G4LogicalVolume* logicDetector =
      new G4LogicalVolume(solidDetector,
                          DetectorMaterial,
                          "Detector");
  G4Colour red(1.0, 0.0, 0.0);
  G4VisAttributes* DetectorVisAttributes = new G4VisAttributes(red);
  logicDetector->SetVisAttributes(DetectorVisAttributes);

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, DetectorPositionZ),
                    logicDetector,
                    "physDetector",
                    logicWorld,
                    false,
                    0,
                    checkOverlaps);


  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
