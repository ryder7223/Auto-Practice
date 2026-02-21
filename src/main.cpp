#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Auto Practice Modifier
class $modify(PlayLayer) {
    // Hooks the init function
    bool init(GJGameLevel* level, bool p1, bool p2) {
        // If you aren't in a level it won't crash your game
        if (!PlayLayer::init(level, p1, p2))
            return false;

        bool allowNormalMode = Mod::get()->getSettingValue<bool>("enable-in-normal");
        bool allowTestMode = Mod::get()->getSettingValue<bool>("enable-in-testmode");
        bool allowPlatformerMode = Mod::get()->getSettingValue<bool>("enable-in-platformer");

        // Disables auto practice if any of these conditions are met
        if (!this->m_isPracticeMode && !m_isTestMode && !m_level->isPlatformer() && allowNormalMode) {
            this->togglePracticeMode(true);
        } else if (m_isTestMode && allowTestMode) {
            this->togglePracticeMode(true);
        } else if (m_level->isPlatformer() && allowPlatformerMode) {
            this->togglePracticeMode(true);
        }

        // Event listener for keybinds inside PlayLayer
        this->addEventListener(
            KeybindSettingPressedEventV3(Mod::get(), "toggle-practice"),
            [this](const Keybind& keybind, bool down, bool repeat, double timestamp) {
                if (down && !repeat) {
                    // Toggle practice mode when a key is pressed
                    this->togglePracticeMode(!this->m_isPracticeMode);
                }
            }
        );

        return true;
    }
};
