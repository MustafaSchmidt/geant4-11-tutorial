#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    //EM Physics
    RegisterPhysics(new G4EmStandardPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}