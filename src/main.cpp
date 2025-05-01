#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#ifndef GEODE_IS_IOS
#include <Geode/loader/Setting.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
// Register the custom keybind when the mod loads
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

// Auto Practice Mode Modifier
class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2))
            return false;

        if (Mod::get()->getSettingValue<bool>("enable-auto-practice")) {
            bool allowTestMode = Mod::get()->getSettingValue<bool>("enable-in-testmode");
            bool allowPlatformerMode = Mod::get()->getSettingValue<bool>("enable-in-platformer");

            bool shouldEnable = true;

            if (m_isTestMode && !allowTestMode)
                shouldEnable = false;
            if (m_level->isPlatformer() && !allowPlatformerMode)
                shouldEnable = false;

            if (shouldEnable) {
                this->togglePracticeMode(true);
            }
        }

#ifndef GEODE_IS_IOS
        // Add listener for custom keybind
        this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
            if (event->isDown()) {
                this->togglePracticeMode(!this->m_isPracticeMode);
            }
            return ListenerResult::Propagate;
        }, "ryder7223.autopractice/toggle-practice"_spr);
        
#endif
        return true;
    }
};
