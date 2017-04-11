#include "gameobject.h"

/* GameObject */
void GameObject::update(sf::RenderWindow& win, std::vector<GameObject*> objects) {
    int newXPosition = clamp((int)(position.x + velocity.x), -1, SCREENWIDTH + 1);
    int newYPosition = clamp((int)(position.y + velocity.y), -1, SCREENHEIGHT + 1);

    position.x = newXPosition;
    position.y = newYPosition;
}

int GameObject::getRenderBucket() {
    return 0;
}

/* SpriteObject */
/* Protected methods */
void SpriteObject::moveSprite() {
    const int width = sprite->getTextureRect().width;
    const int height = sprite->getTextureRect().height;

    sprite->setPosition(position.x - width / 2, position.y - height / 2);
}

/* Public methods */
SpriteObject::SpriteObject(std::shared_ptr<sf::Sprite> sprite) {
    this->sprite = sprite;

    position.x = SCREENWIDTH / 2;
    position.y = SCREENHEIGHT / 2;
    moveSprite();
}

void SpriteObject::update(sf::RenderWindow& win, std::vector<GameObject*> objects) {
    GameObject::update(win, objects);
    moveSprite();
}

void SpriteObject::draw(sf::RenderWindow& win) {
    win.draw(*sprite);
}