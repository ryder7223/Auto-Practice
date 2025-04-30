#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#ifndef GEODE_IS_IOS
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDispatcher.h>
#endif

using namespace geode::prelude;

// Auto Practice Mode Modifier
class $modify(AutoPracticePlayLayer, PlayLayer) {
    static inline AutoPracticePlayLayer* s_instance = nullptr;

    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2)) // Check if you are in a level
            return false;

        s_instance = this; // Save instance to access later from keyboard hook

        if (Mod::get()->getSettingValue<bool>("enable-auto-practice")) {
            bool allowTestMode = Mod::get()->getSettingValue<bool>("enable-in-testmode");
            bool allowPlatformerMode = Mod::get()->getSettingValue<bool>("enable-in-platformer");

            bool shouldEnable = true; // Will remain true if unmodified

            // Setting and play state checks
            if (m_isTestMode && !allowTestMode)
                shouldEnable = false;
            if (m_level->isPlatformer() && !allowPlatformerMode)
                shouldEnable = false;

            if (shouldEnable) {
                this->togglePracticeMode(true);
            }
        }

        return true;
    }

    static AutoPracticePlayLayer* getInstance() {
        return s_instance;
    }
};

#ifndef GEODE_IS_IOS
// Keyboard Input Hook (non-iOS only)
class $modify(KeyListenerDispatcher, CCKeyboardDispatcher) {
    bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool repeat) {
        if (down && key == enumKeyCodes::KEY_C) {
            if (auto layer = PlayLayer::get()) { // Only toggle practice while in PlayLayer
                bool currentlyPractice = layer->m_isPracticeMode;
                layer->togglePracticeMode(!currentlyPractice);
            }
            else {
                return false;
            }
        }

        return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, repeat);
    }
};
#endif
