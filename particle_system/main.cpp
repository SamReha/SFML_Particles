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
    sf::Texture smokeTexture;
    if (!smokeTexture.loadFromFile("whitePuff00.png")) {
        return EXIT_FAILURE;
    }

    sf::Texture sparkTexture;
    if (!sparkTexture.loadFromFile("spark6.png")) {
        return EXIT_FAILURE;
    }

    // create the particle system
    ParticleSystem purpleSmoke(15000, &smokeTexture, 25.f, 75.f);
    purpleSmoke.setGravity(-50);
    purpleSmoke.setColor(sf::Color::Magenta);
    purpleSmoke.addAttractor(SCREENWIDTH / 2, SCREENHEIGHT / 2, 800);

    ParticleSystem sparks(40, &sparkTexture, 50.f, 350.f);
    sparks.setGravity(1000);
    sparks.setColor(sf::Color::Yellow);
    sparks.addRepulsor(0, 0, 1600);

    ParticleSystem activeSystem = purpleSmoke;

    sf::Text particlesCount("particles", font, 25);
    particlesCount.setColor(sf::Color::White);
    particlesCount.setPosition(0, 0);

    sf::Text increaseParticlesText("Use W and S to change the number of particles", font, 25);
    increaseParticlesText.setColor(sf::Color::White);
    increaseParticlesText.setPosition(0, 25);

    sf::Text changeSystemText("Use Q and E to change the active particle system", font, 25);
    changeSystemText.setColor(sf::Color::White);
    changeSystemText.setPosition(0, 50);

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
                    activeSystem.addParticle();
                } else if (event.key.code == sf::Keyboard::S) {
                    activeSystem.removeParticle();
                } else if (event.key.code == sf::Keyboard::Q) {
                    activeSystem = purpleSmoke;
                } else if (event.key.code == sf::Keyboard::E) {
                    activeSystem = sparks;
                }
            }
        }

        // make the particle system emitter follow the mouse
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        activeSystem.setEmitter(window.mapPixelToCoords(mouse));

        // update it
        sf::Time elapsed = clock.restart();
        activeSystem.update(elapsed);

        // Update some text
        particlesCount.setString(std::to_string(activeSystem.count()) + " particles");

        // draw it
        window.clear();
        window.draw(activeSystem);
        window.draw(particlesCount);
        window.draw(increaseParticlesText);
        window.draw(changeSystemText);
        window.display();
    }

    return 0;
}