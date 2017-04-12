#pragma once

#include "utilities.h"

int check_sprite_collision(const sf::Sprite a, const sf::Sprite b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}