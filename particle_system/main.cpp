#include <vector>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "particlemanager.h"

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Particle Demo");

    // Initialize text elements
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }

    // Load a texture or two
    sf::Texture testTexture;
    if (!testTexture.loadFromFile("whitePuff00.png")) {
        return EXIT_FAILURE;
    }

    // create the particle system
    ParticleSystem particles(10000, &testTexture);
    particles.setGravity(500);
    //particles.addAttractor(SCREENWIDTH/2, SCREENHEIGHT/2, 800);
    //particles.addRepulsor(SCREENWIDTH, SCREENHEIGHT, 500);

    sf::Text particlesCount("particles", font, 25);
    particlesCount.setColor(sf::Color::White);
    particlesCount.setPosition(0, 0);

    sf::Text increaseParticlesText("Use W and S to change the number of particles", font, 25);
    increaseParticlesText.setColor(sf::Color::White);
    increaseParticlesText.setPosition(0, 25);

    // create a clock to track the elapsed time
    sf::Clock clock;

    // run the main loop
    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::W) {
                    particles.addParticle();
                } else if (event.key.code == sf::Keyboard::S) {
                    particles.removeParticle();
                }
            }
        }

        // make the particle system emitter follow the mouse
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        particles.setEmitter(window.mapPixelToCoords(mouse));

        // update it
        sf::Time elapsed = clock.restart();
        particles.update(elapsed);

        // Update some text
        particlesCount.setString(std::to_string(particles.count()) + " particles");

        // draw it
        window.clear();
        window.draw(particles);
        window.draw(particlesCount);
        window.draw(increaseParticlesText);
        window.display();
    }

    return 0;
}