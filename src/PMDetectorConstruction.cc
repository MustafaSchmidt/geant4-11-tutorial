#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()
{
}

PMDetectorConstruction::~PMDetectorConstruction()
{
}

G4double PMDetectorConstruction::ConvertWavelengthToEnergy(G4double wavelength)
{
    const G4double h = 6.62607015e-34 * joule * s;
    const G4double c = 299792458 * m / s;

    G4double energy = (h * c) / (wavelength * nm);

    return energy / eV;
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()
{
    G4bool checkOverlaps = true;

    G4NistManager *nist = G4NistManager::Instance();
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *leadMat = nist->FindOrBuildMaterial("G4_Pb");
    G4Material *detMat = nist->FindOrBuildMaterial("G4_Pb");

    G4double photonEnergyMin = ConvertWavelengthToEnergy(800. * nm);
    G4double photonEnergyMax = ConvertWavelengthToEnergy(300. * nm);

    std::cout << "Minimum photon energy: " << photonEnergyMin << std::endl;
    std::cout << "Maximum photon energy: " << photonEnergyMax << std::endl;

    std::vector<G4double> photonEnergy = {photonEnergyMin * eV, photonEnergyMax * eV};
    std::vector<G4double> detRefIndex = {1.8, 1.8};
    std::vector<G4double> worldRefIndex = {1.0, 1.0};
    std::vector<G4double> fraction = {1.0, 1.0};

    G4MaterialPropertiesTable *mptDet = new G4MaterialPropertiesTable();
    mptDet->AddProperty("RINDEX", photonEnergy, detRefIndex);
    mptDet->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, fraction);
    mptDet->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 50. * ns);
    mptDet->AddConstProperty("SCINTILLATIONYIELD", 37. / keV);
    mptDet->AddConstProperty("RESOLUTIONSCALE", 1.0);
    detMat->SetMaterialPropertiesTable(mptDet);

    G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
    mptWorld->AddProperty("RINDEX", photonEnergy, detRefIndex);
    //worldMat->SetMaterialPropertiesTable(mptWorld);

    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, checkOverlaps);

    // NaI Scintillator
    G4double detectorSize = 5.0 * cm;
    G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.0, 5.0 * cm, detectorSize, 0., 360. * deg);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "Detector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, 10.5 * cm), logicDetector, "Detector", logicWorld, false, 0);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    // Photomultiplier tube
    G4double PMTSize = 2.0 * cm;
    G4Tubs *solidPMT = new G4Tubs("solidPMT", 0.0, 5.0 * cm, PMTSize, 0., 360. * deg);
    logicPMT = new G4LogicalVolume(solidPMT, detMat, "logicPMT");
    G4VPhysicalVolume *physPMT = new G4PVPlacement(0, G4ThreeVector(0, 0, 17.5 * cm), logicPMT, "physPMT", logicWorld, false, 0);

    G4VisAttributes *PMTVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5));
    PMTVisAtt->SetForceSolid(true);
    logicPMT->SetVisAttributes(PMTVisAtt);
    
    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitiveDetector");
    logicDetector->SetSensitiveDetector(sensDet);

    PMSensitiveDetector *sensPMT = new PMSensitiveDetector("SensitivePMT");
    logicPMT->SetSensitiveDetector(sensPMT);

    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
    G4SDManager::GetSDMpointer()->AddNewDetector(sensPMT);
}
