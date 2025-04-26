#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class $modify(AutoPracticePlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool p1, bool p2) {
        if (!PlayLayer::init(level, p1, p2)) // Check if you are in a level
            return false;

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
                // Not sure who would even want a notification
                if (Mod::get()->getSettingValue<bool>("show-notification")) {
                    Notification::create(
                        "Auto Practice Enabled",
                        CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"), // Practice button icon
                        0.7f
                    )->show();
                }
            }
        }

        return true;
    }
};