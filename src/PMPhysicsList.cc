#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Optical photons
    RegisterPhysics(new G4OpticalPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}
