#ifndef PMPHYSICSLIST_HH
#define PMPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"

#include "G4EmStandardPhysics.hh"

class PMPhysicsList : public G4VModularPhysicsList
{
public:
    PMPhysicsList();
    ~PMPhysicsList();
};

#endif