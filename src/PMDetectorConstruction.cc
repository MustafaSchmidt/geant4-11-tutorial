#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
    DefineMaterials();

    xWorld = 1. * m;
    yWorld = 1. * m;
    zWorld = 1. * m;
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

void PMDetectorConstruction::DefineMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    worldMat = nist->FindOrBuildMaterial("G4_AIR");
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, checkOverlaps);

    return physWorld;
}
