#include <SFML/Graphics.hpp>

#include <math.h>
#include <iostream>

#include "config.h"

class ParticleSystem : public sf::Drawable, public sf::Transformable {
private:
    struct Particle {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    struct ForceEmitter {
        sf::Vector2f position;
        float strength;
    };

    std::vector<Particle> m_particles;
    std::vector<ForceEmitter> m_forceEmitters;
    sf::VertexArray m_vertices;
    sf::RectangleShape m_rect;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
    float m_gravity;       // Acceleration due to gravity (units per second)

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void resetParticle(std::size_t index);

public:
    ParticleSystem(unsigned int count, sf::Texture* tex);

    void setEmitter(sf::Vector2f position);
    void setGravity(float newGravity);

    int count();

    void addParticle();
    void removeParticle();

    void addRepulsor(float x, float y, float strength);
    void addAttractor(float x, float y, float strength);

    void update(sf::Time elapsed);
};