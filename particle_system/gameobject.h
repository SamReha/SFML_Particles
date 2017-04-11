#pragma once
#include <memory>

#include <SFML/Graphics.hpp>

#include "config.h"
#include "utilities.h"

// Base class that all things should inherit from
class GameObject {
protected:
public:
    sf::Vector2f position; // Centerpoint of object in question
    sf::Vector2f velocity; // How many units per frame do we move?

    virtual void update(sf::RenderWindow& win, std::vector<GameObject*> objects);
    virtual void draw(sf::RenderWindow& win) = 0;
    virtual int getRenderBucket();
    virtual sf::Sprite getCollideable() = 0;
};

// A visible GameObject that holds it's visual data in a SFML sprite
class SpriteObject : public GameObject {
protected:
    std::shared_ptr<sf::Sprite> sprite;
    void moveSprite(); // Sprites have positions separate from our own, make sure that gets updated
public:
    SpriteObject(std::shared_ptr<sf::Sprite> sprite);
    void update(sf::RenderWindow& win, std::vector<GameObject*> objects) override;
    void draw(sf::RenderWindow& win) override;
    sf::Sprite getCollideable() { return *sprite.get(); };
};