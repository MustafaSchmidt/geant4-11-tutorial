#include "PMSensitiveDetector.hh"

PMSensitiveDetector::PMSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
    fTotalEnergyDeposited = 0.;

    G4cout << name << G4endl;

    detName = name;
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
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4double energyDeposited = aStep->GetTotalEnergyDeposit();

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4double globalTime = preStepPoint->GetGlobalTime();
    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();

    G4double momPhotonMag = momPhoton.mag();

    G4double wlen = (1.239841939 * eV / momPhotonMag) * 1E+03;

    G4Track *track = aStep->GetTrack();

    if (detName == "SensitiveDetector")
    {
        if (aStep->GetTrack()->GetDefinition() == G4Gamma::GammaDefinition())
        {
            analysisManager->FillNtupleIColumn(0, 0, eventID);
            analysisManager->FillNtupleDColumn(0, 1, posPhoton[0]);
            analysisManager->FillNtupleDColumn(0, 2, posPhoton[1]);
            analysisManager->FillNtupleDColumn(0, 3, posPhoton[2]);
            analysisManager->FillNtupleDColumn(0, 4, globalTime);
            analysisManager->FillNtupleDColumn(0, 5, momPhotonMag);
            analysisManager->AddNtupleRow(0);
        }

        if (aStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
        {
            if (energyDeposited > 0)
            {
                fTotalEnergyDeposited += energyDeposited;
            }
        }
    }

    if (detName == "SensitivePMT")
    {
        if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
        {
            analysisManager->FillNtupleIColumn(1, 0, eventID);
            analysisManager->FillNtupleDColumn(1, 1, globalTime);
            analysisManager->FillNtupleDColumn(1, 2, wlen);
            analysisManager->AddNtupleRow(1);
        }

        track->SetTrackStatus(fStopAndKill);
    }

    return true;
}

void PMSensitiveDetector::EndOfEvent(G4HCofThisEvent *)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillH1(0, fTotalEnergyDeposited);

    G4cout << "Deposited energy: " << fTotalEnergyDeposited << G4endl;
}
