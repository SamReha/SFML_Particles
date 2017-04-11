#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

int check_sprite_collision(const sf::Sprite a, const sf::Sprite b) {
    return a.getGlobalBounds().intersects(b.getGlobalBounds());
}

template <typename T>
T clamp(T val, T min, T max) {
    if (val < min) {
        val = min;
    } else if (val > max) {
        val = max;
    }
    return val;
}