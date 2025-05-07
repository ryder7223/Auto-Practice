#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#ifndef GEODE_IS_IOS
#include <Geode/loader/Setting.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
// Fancy keybind
$execute {
    BindManager::get()->registerBindable({
        "ryder7223.autopractice/toggle-practice"_spr,
        "Toggle Practice Mode",
        "Toggles practice mode while in a level.",
        { Keybind::create(KEY_C, Modifier::None) },
        "PlayLayer"
    });
}
#endif

// Auto Practice Modifier
class $modify(PlayLayer) {
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

        #ifndef GEODE_IS_IOS
        // Listener for custom keybind
        this->template addEventListener<InvokeBindFilter>([this](InvokeBindEvent* event) {
            if (event->isDown()) {
                // Another normal mode check!!!111
                this->togglePracticeMode(!this->m_isPracticeMode);
            }
            return ListenerResult::Propagate;
        }, "ryder7223.autopractice/toggle-practice"_spr);
        
#endif
        return true;
    }
};
