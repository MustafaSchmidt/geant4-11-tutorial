#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *leadMat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material *detMat = nist->FindOrBuildMaterial("G4_Pb");

    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, checkOverlaps);

    // Lead Absorber
    G4double leadThickness = 2.0 * mm;
    G4Box *solidLead = new G4Box("Lead", 5.0 * cm, 5.0 * cm, leadThickness / 2);
    G4LogicalVolume *logicLead = new G4LogicalVolume(solidLead, leadMat, "Lead");
    G4VPhysicalVolume *physLead = new G4PVPlacement(0, G4ThreeVector(0, 0, 5.0 * cm), logicLead, "Lead", logicWorld, false, 0);

    G4VisAttributes* leadVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    leadVisAtt->SetForceSolid(true);
    logicLead->SetVisAttributes(leadVisAtt);

    // NaI Detector
    G4double detectorSize = 5.0 * cm;
    G4Box *solidDetector = new G4Box("Detector", detectorSize, detectorSize, detectorSize);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "Detector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, 10.5 * cm), logicDetector, "Detector", logicWorld, false, 0);

    G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);

    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}