#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}
