#include "PMPhysicsList.hh"

PMPhysicsList::PMPhysicsList()
{
    // EM physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Optical photons
    RegisterPhysics(new G4OpticalPhysics());

    // Radioactive decay->decays of R-ion
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // Handling radioactive decays
    RegisterPhysics(new G4DecayPhysics());
}

PMPhysicsList::~PMPhysicsList()
{
}
