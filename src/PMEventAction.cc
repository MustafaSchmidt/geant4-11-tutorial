#include "PMEventAction.hh"

PMEventAction::PMEventAction(PMRunAction *)
{
}

PMEventAction::~PMEventAction() {}

void PMEventAction::BeginOfEventAction(const G4Event *event)
{
    G4cout << "Event number: " << event->GetEventID() << G4endl;
}

void PMEventAction::EndOfEventAction(const G4Event *event)
{
}