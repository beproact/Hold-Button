#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class HoldButtonManager : CCObject {
    //static HoldButtonManager* instance;
public:
    utils::Timer<std::chrono::high_resolution_clock> m_timer;
    CCAnimate* m_animate;
    HoldButtonManager(){
        loadAnimate();
        m_timer = utils::Timer();
    }
    ~HoldButtonManager(){
        //log::debug("deleting");
        m_animate->release();
    }


    static HoldButtonManager* get() ;

    void registerWithNode(CCNode* node);

    void registerBtn(std::string_view id, CCNode* menu);

    void btnUnselect(CCObject* sender);

    void btnActivate(CCObject* sender);

    void btnSelect(CCObject* sender);

private:
    void loadAnimate();
};




#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
public:
	struct Fields {
        std::function<void(CCObject*)> m_selectCallback = nullptr;
        std::function<void(CCObject*)> m_activateCallback = nullptr;
        std::function<void(CCObject*)> m_unselectCallback = nullptr;
        SEL_MenuHandler m_originalCallback = nullptr;
	};

	void selected() {
		if(m_fields->m_selectCallback){
			//(m_pListener->*(m_fields->m_selectCallback))(this);
            m_fields->m_selectCallback(this);
		}
		CCMenuItemSpriteExtra::selected();
	}

    void activate() {
        //log::debug("{}",rect().size);
        if(m_fields->m_activateCallback){
            m_fields->m_activateCallback(this);
        }
        CCMenuItemSpriteExtra::activate();
    }

    void unselected(){
        if(m_fields->m_unselectCallback){
            m_fields->m_unselectCallback(this);
        }
        CCMenuItemSpriteExtra::unselected();
    }
}; 
