#include "PMRunAction.hh"

PMRunAction::PMRunAction()
{
}

PMRunAction::~PMRunAction()
{
}

void PMRunAction::BeginOfRunAction(const G4Run *run)
{
}

void PMRunAction::EndOfRunAction(const G4Run *run)
{
    G4int runID = run->GetRunID();
    
    G4cout << "Finishing run " << runID << G4endl;
}
