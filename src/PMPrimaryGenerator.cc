#include "PMPrimaryGenerator.hh"

PMPrimaryGenerator::PMPrimaryGenerator()
{
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Particle position
    G4double x = 0.;
    G4double y = 0.;
    G4double z = 0.;

    G4ThreeVector pos(x, y, z);

    // Particle direction
    G4double px = 0.;
    G4double py = 0.;
    G4double pz = 1.;

    G4ThreeVector mom(px, py, pz);

    // Particle type:
    G4ParticleTable *particleTable = G4ParticleTable ::GetParticleTable();
    G4ParticleDefinition *particle = particleTable->FindParticle("e+");    

    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(1. * MeV);
    fParticleGun->SetParticleDefinition(particle);

    // create vertex
    fParticleGun->GeneratePrimaryVertex(anEvent);
}