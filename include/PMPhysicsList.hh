#ifndef PMPHYSICSLIST_HH
#define PMPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

class PMPhysicsList : public G4VModularPhysicsList
{
public:
    PMPhysicsList();
    ~PMPhysicsList();
};

#endif
