#include <vector>
#include <memory>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "config.h"
#include "assetmanager.h"
#include "gameobject.h"

using namespace std;
//typedef shared_ptr<GameObject> GameObject_ptr;

// Basic prototypes
void update();
void addGameObject(GameObject* obj);
void deleteGameObjects();
void renderFrame();

// Basic structures to hold game objects
vector<GameObject*> objectList;
vector<GameObject*> renderBuckets[RENDERDEPTH];

// Create the main window
sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Particle Demo");

int main(int, char const**) {
    // Load our assets!
    AssetManager assetManager();
    string assetPath = "assets/";

    // Preload sprites
    string imagePath = assetPath + "images/";
    // assetManager.spritePreloader.emplace("test", imagePath + "test.png");

    // Preload fonts
    string fontPath = assetPath + "fonts/";
    // assetManager.spritePreloader.emplace("test", fontPath + "test.tff");

    // Preload sounds
    string soundPath = assetPath + "sounds/";
    // assetManager.spritePreloader.emplace("test", soundPath + "test.wav");

    // Create our game objects
    /*Ball* ball = new Ball(sprites["ball"]);
    addGameObject(ball);

    Paddle* paddle = new Paddle(sprites["paddle"], ball);
    addGameObject(paddle);*/

    sf::Clock clock;
    float accumulatedTime = 0;

    while (window.isOpen()) {
        // Handle OS events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //// Handle key press events
            //if (event.type == sf::Event::KeyPressed) {
            //    switch (event.key.code) {
            //    case sf::Keyboard::Escape:
            //        window.close();
            //        break;
            //    case sf::Keyboard::Space:
            //        paddle->fireBall();
            //        break;
            //    default:
            //        break;
            //    }
            //}
        }

        float deltaTime = clock.restart().asSeconds();
        accumulatedTime += deltaTime;
        while (accumulatedTime >= ONE_SIXTIETH) {
            update();
            accumulatedTime -= ONE_SIXTIETH;
        }

        window.clear();
        renderFrame();
        window.display();
    }

    deleteGameObjects();
}

void update() {
    for (auto obj : objectList) {
        obj->update(window, objectList);
    }
}

void addGameObject(GameObject* obj) {
    objectList.push_back(obj);
    int i = obj->getRenderBucket();
    renderBuckets[i].push_back(obj);
}

void deleteGameObjects() {
    for (auto obj : objectList) {
        delete obj;
    }
}

void renderFrame() {
    for (int i = 0; i < RENDERDEPTH; ++i) {
        for (auto obj : renderBuckets[i])
            obj->draw(window);
    }
}