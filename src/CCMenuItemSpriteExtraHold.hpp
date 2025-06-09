#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/timer.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCMenuItemSpriteExtra>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
	struct Fields {
		SEL_MenuHandler m_selectCallback;
	};

	void selected() {
		m_timer.reset();
		//scheduler->scheduleSelector(schedule_selector(CCMenuItemSpriteExtraHold::delay), this, 1, 0, 0, false);
		if(m_selectCallback){
			(m_pListener->*(m_fields->m_selectCallback))(this);
		}
		CCMenuItemSpriteExtra::selected();
	}
}

/*class CCMenuItemSpriteExtraHold : public CCMenuItemSpriteExtra { // 
public:
	int64_t m_timePressed;
	utils::Timer<std::chrono::high_resolution_clock> m_timer;
	float m_timeDelay;
	SEL_MenuHandler m_selectCallback;

	CCMenuItemSpriteExtraHold() : CCMenuItemSpriteExtra() {
		m_timePressed=0;
		m_timer = utils::Timer();
		m_timeDelay = 0.5f;
		m_selectCallback = nullptr;
	}
	static CCMenuItemSpriteExtraHold* create(CCNode* sprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler callback){
		CCMenuItemSpriteExtraHold* menuItem = new CCMenuItemSpriteExtraHold();
		if(!menuItem) {
			log::error("CCMenuItemSpriteExtraHold failed because the constructor failed.");
			return nullptr;
		}
		
		if(!menuItem->CCMenuItemSpriteExtra::init(sprite, disabledSprite, target, callback)) { // wow i made it one line shorter and made it impossible to read
			log::error("CCMenuItemSpriteExtraHold failed because CCMenuItemSpriteExtra::init failed.");
			delete menuItem;
			return nullptr;
		}
		return menuItem;
	}

    static CCMenuItemSpriteExtraHold* create(CCNode* sprite, CCObject* target, SEL_MenuHandler callback) {
        return CCMenuItemSpriteExtraHold::create(sprite, nullptr, target, callback);
    }

	void activate() {
		m_timePressed = m_timer.elapsed();
		if(m_pfnSelector) CCMenuItemSpriteExtra::activate();
	}

	void selected() {
		m_timer.reset();
		//scheduler->scheduleSelector(schedule_selector(CCMenuItemSpriteExtraHold::delay), this, 1, 0, 0, false);
		if(m_selectCallback){
			(m_pListener->*m_selectCallback)(this);
		}
		CCMenuItemSpriteExtra::selected();
	}
};*/