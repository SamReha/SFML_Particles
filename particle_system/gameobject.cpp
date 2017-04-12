#include "gameobject.h"

/* GameObject */
void GameObject::update(sf::RenderWindow& win, std::vector<GameObject*> objects) {
    float newXPosition = std::max((float)(-1), std::min(position.x + velocity.x, (float)(SCREENWIDTH + 1)));
    float newYPosition = std::max((float)(-1), std::min(position.y + velocity.y, (float)(SCREENHEIGHT + 1)));

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