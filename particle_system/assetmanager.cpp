#include "AssetManager.h"

void AssetManager::loadSpriteAssets() {
    for (std::map<std::string, std::string>::iterator it = spritePreloader.begin(); it != spritePreloader.end(); it++) {
        std::string assetName = it->first;
        std::string assetPath = it->second;
        
        // First, load image as texture (should raise FileNotFound)
        sf::Texture texture;
        if (!texture.loadFromFile(assetPath)) {
            return;
        }

        // If that was successful, then initialize a sprite and stick it in the asset map
        sf::Sprite sprite(texture);
        spriteAssets.emplace(assetName, std::make_shared<sf::Sprite>(sprite));
    }

    // Finally, set the loaded flag
    spritesLoaded = true;
}

void AssetManager::loadFontAssets() {
    for (std::map<std::string, std::string>::iterator it = spritePreloader.begin(); it != spritePreloader.end(); it++) {
        std::string assetName = it->first;
        std::string assetPath = it->second;

        // First, load the font (should raise FileNotFound)
        sf::Font font;
        if (!font.loadFromFile(assetPath)) {
            return;
        }

        // If that was successful, then stick them in the asset map
        fontAssets.emplace(assetName, std::make_shared<sf::Font>(font));
    }

    // Finally, set the loaded flag
    fontsLoaded = true;
}

void AssetManager::loadSoundAssets() {
    for (std::map<std::string, std::string>::iterator it = spritePreloader.begin(); it != spritePreloader.end(); it++) {
        std::string assetName = it->first;
        std::string assetPath = it->second;

        // First, load the file as a soundbuffer (should raise FileNotFound)
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(assetPath)) {
            return;
        }

        // If that was successful, then initialize the sound and stick it in the asset map
        sf::Sound sound;
        sound.setBuffer(buffer);

        soundAssets.emplace(assetName, std::make_shared<sf::Sound>(sound));
    }

    // Finally, set the loaded flag
    soundsLoaded = true;
}

sprite_ptr AssetManager::getSprite(std::string spriteName) {
    if (!spritesLoaded)
        loadSpriteAssets();

    return spriteAssets[spriteName];
}

font_ptr AssetManager::getFont(std::string fontName) {
    if (!fontsLoaded)
        loadFontAssets();

    return fontAssets[fontName];
}

sound_ptr AssetManager::getSound(std::string soundName) {
    if (!soundsLoaded)
        loadSoundAssets();

    return soundAssets[soundName];
}
