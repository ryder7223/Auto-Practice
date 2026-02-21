#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

// Auto Practice Modifier
class $modify(PlayLayer) {

    struct Fields {
        ListenerHandle keybindListener;
    };

    // Hooks a function probably
    bool init(GJGameLevel* level, bool p1, bool p2) {
        // If you aren't in a level it won't crash your game
        if (!PlayLayer::init(level, p1, p2))
            return false;

        bool allowNormalMode = Mod::get()->getSettingValue<bool>("enable-in-normal");
        bool allowTestMode = Mod::get()->getSettingValue<bool>("enable-in-testmode");
        bool allowPlatformerMode = Mod::get()->getSettingValue<bool>("enable-in-platformer");
        bool shouldEnable = true;

        // Disables auto practice if any of these conditions are met
        if (!this->m_isPracticeMode && !m_isTestMode && !m_level->isPlatformer() && allowNormalMode) {
            this->togglePracticeMode(true);
        } else if (m_isTestMode && allowTestMode) {
            this->togglePracticeMode(true);
        } else if (m_level->isPlatformer() && allowPlatformerMode) {
            this->togglePracticeMode(true);
        }

        // Listener for keybind setting
        m_fields->keybindListener =
            listenForKeybindSettingPresses("toggle-practice",
                [this](const Keybind& keybind, bool down, bool repeat, double timestamp) {
                    if (down && !repeat) {
                        // Another normal mode check!!!111
                        this->togglePracticeMode(!this->m_isPracticeMode);
                    }
                }
            );

        return true;
    }

    void onExit() {
        // Cancel listener before layer is destroyed
        m_fields->keybindListener.cancel();
        PlayLayer::onExit();
    }
};
