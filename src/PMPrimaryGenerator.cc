#include "PMPrimaryGenerator.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);

    // Particle position
    G4double x = 0.;
    G4double y = 0.;
    G4double z = -1. * cm;

    G4ThreeVector pos(x, y, z);

    fParticleGun->SetParticlePosition(pos);

    // Particle direction
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 0.;

    G4ThreeVector mom(px, py, pz);

    fParticleGun->SetParticleMomentumDirection(mom);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Particle type:
    G4int Z = 9;
    G4int A = 18;

    G4double charge = 0. * eplus; // atom
    G4double energy = 0. * keV;

    G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z, A, energy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(charge);
    fParticleGun->SetParticleEnergy(0.);

    // create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
