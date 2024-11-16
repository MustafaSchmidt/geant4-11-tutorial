#ifndef PMACTIONINITIALIZATION_HH
#define PMACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"

#include "PMPrimaryGenerator.hh"
#include "PMRunAction.hh"
#include "PMEventAction.hh"

class PMActionInitialization : public G4VUserActionInitialization
{
  public:
    PMActionInitialization();
    virtual ~PMActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
};

#endif
