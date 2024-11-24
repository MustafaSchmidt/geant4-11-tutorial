#include "PMSensitiveDetector.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;
}

PMSensitiveDetector::~PMSensitiveDetector()
{
}

void PMSensitiveDetector::Initialize(G4HCofThisEvent *)
{
    fTotalEnergyDeposited = 0.;
}

G4bool PMSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    if (energyDeposited > 0)
    {
        fTotalEnergyDeposited += energyDeposited;
    }

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;
}
