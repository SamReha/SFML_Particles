#include "particlemanager.h"

/* Private */
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = NULL;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void ParticleSystem::resetParticle(std::size_t index) {
    // give a random velocity and lifetime to the particle
    float angle = (std::rand() % 360) * 3.14f / 180.f;
    float speed = (std::rand() % 50) + 50.f;
    m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

    // reset the position of the corresponding vertex
    m_vertices[index].position = m_emitter;
}

/* Public */
ParticleSystem::ParticleSystem(unsigned int count, sf::Texture* tex) :
    m_particles(count),
    m_vertices(sf::Points, count),
    m_lifetime(sf::seconds(3)),
    m_emitter(0, 0),
    m_gravity(0) {
    sf::Vector2f rectSize(64, 64);
    m_rect.setSize(rectSize);
    m_rect.setTexture(tex, false);
}

void ParticleSystem::setEmitter(sf::Vector2f position) {
    m_emitter = position;
}

void ParticleSystem::setGravity(float newGravity) {
    m_gravity = newGravity;
}

int ParticleSystem::count() {
    return m_particles.size();
}

void ParticleSystem::addParticle() {
    Particle newParticle;
    m_particles.push_back(newParticle);
    m_vertices.resize(m_particles.size());
    resetParticle(m_particles.size() - 1);
}

void ParticleSystem::removeParticle() {
    if (!m_particles.empty()) {
        m_particles.pop_back();
        m_vertices.resize(m_particles.size());
    }
}

void ParticleSystem::addRepulsor(float x, float y, float strength) {
    ForceEmitter force;

    force.position.x = x;
    force.position.y = y;
    force.strength = -strength;

    m_forceEmitters.push_back(force);
}

void ParticleSystem::addAttractor(float x, float y, float strength) {
    ForceEmitter force;

    force.position.x = x;
    force.position.y = y;
    force.strength = strength;

    m_forceEmitters.push_back(force);
}

void ParticleSystem::update(sf::Time elapsed) {
    for (std::size_t i = 0; i < m_particles.size(); i++) {
        // update the particle lifetime
        Particle& p = m_particles[i];
        p.lifetime -= elapsed;

        // if the particle is dead, respawn it
        if (p.lifetime <= sf::Time::Zero)
            resetParticle(i);

        // Update velocity w/ respect to time
        p.velocity.y += m_gravity * elapsed.asSeconds();

        // Update velocity w/ respect to attractors / repulsors
        for (auto forceEmitter : m_forceEmitters) {
            float deltaX = forceEmitter.position.x - m_vertices[i].position.x;
            float deltaY = forceEmitter.position.y - m_vertices[i].position.y;
            float angleInRadians = atan2(deltaY, deltaX);
            float distance = sin(angleInRadians) * deltaY;

            float maximumXForce = forceEmitter.strength * cos(angleInRadians);
            float maximumYForce = forceEmitter.strength * sin(angleInRadians);

            float finalXForce = maximumXForce;// / (distance * distance); WHY DON'T YOU WORK
            float finalYForce = maximumYForce;// / (distance * distance);

            p.velocity.x += finalXForce * elapsed.asSeconds();
            p.velocity.y += finalYForce * elapsed.asSeconds();
        }

        // update the position of the corresponding vertex
        m_vertices[i].position += p.velocity * elapsed.asSeconds();

        // update the alpha (transparency) of the particle according to its lifetime
        float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
        m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);
    }
}
