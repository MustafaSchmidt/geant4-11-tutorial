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
    G4Material *shieldMat = nist->FindOrBuildMaterial("G4_Fe");

    // Define Fluorine-18
    G4Isotope* F18 = new G4Isotope("F18", 9, 18, 18.000938 * g / mole);
    G4Element* elF18 = new G4Element("Fluorine-18", "F18", 1);
    elF18->AddIsotope(F18, 100.0 * perCent);
    G4Material* matF18 = new G4Material("F18Source", 1.51 * g / cm3, 1);
    matF18->AddElement(elF18, 100.0 * perCent);

    G4double photonEnergyMin = ConvertWavelengthToEnergy(800. * nm);
    G4double photonEnergyMax = ConvertWavelengthToEnergy(300. * nm);

    std::cout << "Minimum photon energy: " << photonEnergyMin << std::endl;
    std::cout << "Maximum photon energy: " << photonEnergyMax << std::endl;

    std::vector<G4double> photonEnergy = {photonEnergyMin * eV, photonEnergyMax * eV};
    std::vector<G4double> detRefIndex = {1.8, 1.8};
    std::vector<G4double> worldRefIndex = {1.0, 1.0};
    std::vector<G4double> fraction = {1.0, 1.0};
    std::vector<G4double> reflectivity = {0.9, 0.9};

    // Mirror surface
    G4OpticalSurface *mirrorSurface = new G4OpticalSurface("lg_mirror_op_surf");

    mirrorSurface->SetType(dielectric_metal);
    mirrorSurface->SetModel(unified);
    mirrorSurface->SetFinish(ground);
    mirrorSurface->SetSigmaAlpha(0.0001);

    // Material properties
    G4MaterialPropertiesTable *mptDet = new G4MaterialPropertiesTable();
    mptDet->AddProperty("RINDEX", photonEnergy, detRefIndex);
    mptDet->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, fraction);
    mptDet->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 50. * ns);
    mptDet->AddConstProperty("SCINTILLATIONYIELD", 37. / keV);
    mptDet->AddConstProperty("RESOLUTIONSCALE", 1.0);
    detMat->SetMaterialPropertiesTable(mptDet);

    G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
    mptMirror->AddProperty("REFLECTIVITY", photonEnergy, reflectivity);
    mirrorSurface->SetMaterialPropertiesTable(mptMirror);

    G4double xWorld = 0.5 * m;
    G4double yWorld = 0.5 * m;
    G4double zWorld = 0.5 * m;

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, checkOverlaps);

    // Fluorine Source
    G4double sourceRadius = 1.0 * mm;

    G4Sphere *solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. *deg, 0.0, 180. * deg);
    G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, matF18, "logicSource");
    G4VPhysicalVolume *physSource = new G4PVPlacement(0, G4ThreeVector(0, 0, -1 * cm), logicSource, "physSource", logicWorld, false, 0);

    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 0.5));
    sourceVisAtt->SetForceSolid(true);
    logicSource->SetVisAttributes(sourceVisAtt);

    // NaI Scintillator
    G4double detectorLength = 5.0 * cm;
    G4double detectorRadius = 5.0 * cm;
    G4double detectorShift = 10. * cm;

    G4Tubs *solidDetector = new G4Tubs("solidDetector", 0.0, detectorRadius, detectorLength, 0., 360. * deg);
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "Detector");
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, detectorShift), logicDetector, "Detector", logicWorld, false, 0);

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.5));
    detVisAtt->SetForceSolid(true);
    logicDetector->SetVisAttributes(detVisAtt);

    // Photomultiplier tube
    G4double PMTLength = 0.5 * cm;
    G4double PMTShift = detectorLength + PMTLength + detectorShift;

    G4Tubs *solidPMT = new G4Tubs("solidPMT", 0.0, detectorRadius, PMTLength, 0., 360. * deg);
    logicPMT = new G4LogicalVolume(solidPMT, detMat, "logicPMT");
    G4VPhysicalVolume *physPMT = new G4PVPlacement(0, G4ThreeVector(0, 0, PMTShift), logicPMT, "physPMT", logicWorld, false, 0);

    G4VisAttributes *PMTVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.5));
    PMTVisAtt->SetForceSolid(true);
    logicPMT->SetVisAttributes(PMTVisAtt);

    // Shielding
    G4double shieldLength = PMTShift + PMTLength;
    G4double shieldThickness = 2.0 * cm;
    G4double shieldRadius = detectorRadius + shieldThickness;

    G4Tubs *solidShield = new G4Tubs("solidPMT", detectorRadius, shieldRadius, shieldLength, 0., 360. * deg);
    G4LogicalVolume *logicShield = new G4LogicalVolume(solidShield, shieldMat, "logicPMT");
    G4VPhysicalVolume *physShield = new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * cm), logicShield, "physShield", logicWorld, false, 0);

    G4VisAttributes *ShieldVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    ShieldVisAtt->SetForceSolid(true);
    logicShield->SetVisAttributes(ShieldVisAtt);

    // Mirror surface
    G4LogicalBorderSurface *surfaceMirror = new G4LogicalBorderSurface("MirrorSurface", physDetector, physShield, mirrorSurface);

    // Endcaps
    G4double endcapLength = 2.0 * cm;
    G4double endcapShift = shieldLength + endcapLength;

    G4Tubs *solidEndcap = new G4Tubs("solidEndap", 0.0 * cm, shieldRadius, endcapLength, 0., 360. * deg);
    G4LogicalVolume *logicEndcap = new G4LogicalVolume(solidEndcap, shieldMat, "logicEndcap");
    G4VPhysicalVolume *physEndcap1 = new G4PVPlacement(0, G4ThreeVector(0, 0, endcapShift), logicEndcap, "physEndcap1", logicWorld, false, 0);
    G4VPhysicalVolume *physEndcap2 = new G4PVPlacement(0, G4ThreeVector(0, 0, -endcapShift), logicEndcap, "physEndcap2", logicWorld, false, 0);

    logicEndcap->SetVisAttributes(ShieldVisAtt);

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
