#pragma once
#include <memory>
#include <map>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

typedef std::shared_ptr<sf::Sprite> sprite_ptr;
typedef std::shared_ptr<sf::Font> font_ptr;
typedef std::shared_ptr<sf::Sound> sound_ptr;

/* Refactor as singleton / static global object sometime later */
/* You know, I could also refactor this to just look in the given director and load whatever it can of compatible file types - that's a lot of work, though */
class AssetManager {
protected:
    bool spritesLoaded = false;
    bool fontsLoaded = false;
    bool soundsLoaded = false;

    std::map<std::string, sprite_ptr> spriteAssets;
    std::map<std::string, font_ptr> fontAssets;
    std::map<std::string, sound_ptr> soundAssets;

    void loadSpriteAssets();
    void loadFontAssets();
    void loadSoundAssets();
public:
    // Use these before you get anything from the assetmanager to configure a map of (string) asset_name -> (string) asset_path
    std::map <std::string, std::string> spritePreloader;
    std::map <std::string, std::string> fontPreloader;
    std::map <std::string, std::string> soundPreloader;

    AssetManager() {};

    sprite_ptr getSprite(std::string spriteName);
    font_ptr getFont(std::string fontName);
    sound_ptr getSound(std::string soundName);
};