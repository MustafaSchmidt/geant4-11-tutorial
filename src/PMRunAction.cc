#include "PMRunAction.hh"

PMRunAction::PMRunAction()
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    analysisManager->CreateH1("Edep", "Energy deposit", 100, 0., 1.1 * MeV);

    analysisManager->CreateNtuple("Scintillator", "Scintillator");
    analysisManager->CreateNtupleIColumn("iEvent");
    analysisManager->CreateNtupleDColumn("fX");
    analysisManager->CreateNtupleDColumn("fY");
    analysisManager->CreateNtupleDColumn("fZ");
    analysisManager->CreateNtupleDColumn("fGlobalTime");
    analysisManager->CreateNtupleDColumn("fEnergy");
    analysisManager->FinishNtuple(0);
    analysisManager->CreateNtuple("Detector", "Detector");
    analysisManager->CreateNtupleIColumn("iEvent");
    analysisManager->CreateNtupleDColumn("fGlobalTime");
    analysisManager->CreateNtupleDColumn("fWlen");
    analysisManager->FinishNtuple(1);
}

PMRunAction::~PMRunAction()
{
}

void PMRunAction::BeginOfRunAction(const G4Run *run)
{
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();

    std::stringstream strRunID; // integer -> string
    strRunID << runID;

    analysisManager->OpenFile("output" + strRunID.str() + ".root");
}

void PMRunAction::EndOfRunAction(const G4Run *run)
{
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();

    man->CloseFile();

    G4int runID = run->GetRunID();

    G4cout << "Finishing run " << runID << G4endl;
}
