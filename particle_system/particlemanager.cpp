#include "particlemanager.h"

/* Private */
void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // apply the transform
    states.transform *= getTransform();

    // our particles don't use a texture
    states.texture = m_tex;

    // draw the vertex array
    target.draw(m_vertices, states);
}

void ParticleSystem::resetParticle(std::size_t index) {
    // give a random velocity and lifetime to the particle
    float angle = (std::rand() % m_maxAngle) * PI / 180.f;
    float speed = m_minSpeed + (std::rand() % (int)(m_maxSpeed - m_minSpeed));
    m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
    m_particles[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

    // reset the position of the corresponding quad
    int quadIndex = index * 4;
    int halfWidth = QUAD_WIDTH / 2;
    m_vertices[quadIndex].position     = sf::Vector2f(m_emitter.x - halfWidth, m_emitter.y - halfWidth);
    m_vertices[quadIndex + 1].position = sf::Vector2f(m_emitter.x + halfWidth, m_emitter.y - halfWidth);
    m_vertices[quadIndex + 2].position = sf::Vector2f(m_emitter.x + halfWidth, m_emitter.y + halfWidth);
    m_vertices[quadIndex + 3].position = sf::Vector2f(m_emitter.x - halfWidth, m_emitter.y + halfWidth);
}

void ParticleSystem::addVertexQuad() {
    m_vertices.append(sf::Vertex(sf::Vector2f(0,                   0), m_color, sf::Vector2f(0,                   0)));
    m_vertices.append(sf::Vertex(sf::Vector2f(QUAD_WIDTH,          0), m_color, sf::Vector2f(QUAD_WIDTH,          0)));
    m_vertices.append(sf::Vertex(sf::Vector2f(QUAD_WIDTH, QUAD_WIDTH), m_color, sf::Vector2f(QUAD_WIDTH, QUAD_WIDTH)));
    m_vertices.append(sf::Vertex(sf::Vector2f(0,          QUAD_WIDTH), m_color, sf::Vector2f(0,          QUAD_WIDTH)));
}

void ParticleSystem::removeVertexQuad() {
    m_vertices.resize(m_vertices.getVertexCount() - 4);
}

/* Public */
ParticleSystem::ParticleSystem(unsigned int count, sf::Texture* tex, float minSpeed, float maxSpeed) :
    m_particles(count),
    m_vertices(sf::Quads),
    m_lifetime(sf::seconds(3)),
    m_emitter(0, 0),
    m_gravity(0) {
    m_tex = tex;
    m_color = sf::Color::White;
    m_minSpeed = minSpeed;
    m_maxSpeed = maxSpeed;
    m_maxAngle = 360;

    for (int i = 0; i < count; i++) {
        addVertexQuad();
    }
}

void ParticleSystem::setEmitter(sf::Vector2f position) {
    m_emitter = position;
}

void ParticleSystem::setGravity(float newGravity) {
    m_gravity = newGravity;
}

void ParticleSystem::setColor(sf::Color newColor) {
    m_color = newColor;
}

void ParticleSystem::setMinSpeed(float newSpeed) {
    m_minSpeed = abs(newSpeed);
}

void ParticleSystem::setMaxSpeed(float newSpeed) {
    m_maxSpeed = abs(newSpeed);
}

void ParticleSystem::setMaxAngle(int newAngle) {
    m_maxAngle = std::max(0, std::min(newAngle, 360));
}

int ParticleSystem::count() {
    return m_particles.size();
}

void ParticleSystem::addParticle() {
    Particle newParticle;
    m_particles.push_back(newParticle);
    addVertexQuad();
    resetParticle(m_particles.size() - 1);
}

void ParticleSystem::removeParticle() {
    if (!m_particles.empty()) {
        m_particles.pop_back();
        removeVertexQuad();
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
        int quadIndex = i * 4;

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
            sf::Vector2f particlePos = m_vertices[quadIndex].position;
            float deltaX = forceEmitter.position.x - particlePos.x;
            float deltaY = forceEmitter.position.y - particlePos.y;
            float angleInRadians = atan2(deltaY, deltaX);
            float distance = sin(angleInRadians) * deltaY;

            float maximumXForce = forceEmitter.strength * cos(angleInRadians);
            float maximumYForce = forceEmitter.strength * sin(angleInRadians);

            float finalXForce = maximumXForce;// / (distance * distance); WHY DON'T YOU WORK
            float finalYForce = maximumYForce;// / (distance * distance);

            p.velocity.x += finalXForce * elapsed.asSeconds();
            p.velocity.y += finalYForce * elapsed.asSeconds();
        }

        // update the position of the corresponding quad
        m_vertices[quadIndex].position     += p.velocity * elapsed.asSeconds();
        m_vertices[quadIndex + 1].position += p.velocity * elapsed.asSeconds();
        m_vertices[quadIndex + 2].position += p.velocity * elapsed.asSeconds();
        m_vertices[quadIndex + 3].position += p.velocity * elapsed.asSeconds();

        // update the color of the corresponding quad
        m_vertices[quadIndex].color     = m_color;
        m_vertices[quadIndex + 1].color = m_color;
        m_vertices[quadIndex + 2].color = m_color;
        m_vertices[quadIndex + 3].color = m_color;

        // update the alpha (transparency) of the particle according to its lifetime
        float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
        sf::Uint8 alpha = static_cast<sf::Uint8>(ratio * 255);
        m_vertices[quadIndex].color.a     = alpha;
        m_vertices[quadIndex + 1].color.a = alpha;
        m_vertices[quadIndex + 2].color.a = alpha;
        m_vertices[quadIndex + 3].color.a = alpha;
    }
}
