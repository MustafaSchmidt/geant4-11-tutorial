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

    G4NistManager *nist  = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *leadMat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material *detMat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");

    // Define Fluorine-18
    G4Isotope *F18 = new G4Isotope("F18", 9, 18, 18.000938 * g / mole);
    G4Element *elF18 = new G4Element("Fluorine-18", "F18", 1);
    elF18->AddIsotope(F18, 100.0 * perCent);
    G4Material *matF18 = new G4Material("F18Source", 1.51 * g / cm3, 1);
    matF18->AddElement(elF18, 100.0 * perCent);

    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);

    // Fluorine source
    G4double sourceRadius = 1. * mm;

    G4Sphere *solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. * deg, 0.0, 180. * deg);
    G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, matF18, "logicSource");
    G4VPhysicalVolume *physSource = new G4PVPlacement(0, G4ThreeVector(0., 0., -1. * cm), logicSource, "physSource", logicWorld, 0, checkOverlaps);

    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 1.0, 0.5));
    sourceVisAtt->SetForceSolid(true);
    logicSource->SetVisAttributes(sourceVisAtt);

    G4double leadThickness = 2. * mm;
    G4double leadSize = 10. * cm;
    G4Box *solidLead = new G4Box("solidLead", 0.5 * leadSize, 0.5 * leadSize, 0.5 * leadThickness);
    G4LogicalVolume *logicLead = new G4LogicalVolume(solidLead, leadMat, "logicLead");
    G4VPhysicalVolume *physLead = new G4PVPlacement(0, G4ThreeVector(0., 0., 5. * cm), logicLead, "physLead", logicWorld, false, checkOverlaps);

    G4VisAttributes *leadVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));
    leadVisAtt->SetForceSolid(true);
    logicLead->SetVisAttributes(leadVisAtt);

    G4double detectorSize = 10.0 * cm;

    G4Box *solidDetector = new G4Box("solidDetector", 0.5 * detectorSize, 0.5 * detectorSize, 0.5 * detectorSize);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0. , 10.5 * cm), logicDetector, "physDetector", logicWorld, false, checkOverlaps);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitveDetector");
    logicDetector->SetSensitiveDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
}