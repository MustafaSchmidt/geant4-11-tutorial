#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

#include "PMRunAction.hh"

class PMEventAction : public G4UserEventAction
{
public:
    PMEventAction(PMRunAction *);
    ~PMEventAction();

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);
};

#endif
