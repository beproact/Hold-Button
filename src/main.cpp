#include <Geode/Geode.hpp>
#include "SelectCallback.hpp"


using namespace geode::prelude;
#include <user95401.gif-sprites/include/CCGIFAnimatedSprite.hpp>



/*
Things in deleteBtn we need to access:
gif sprite
timer
original callback
*/



#include <Geode/modify/EditLevelLayer.hpp>
class $modify(MyEditLevelLayer, EditLevelLayer) {

    struct Fields {
        SEL_MenuHandler m_deleteCallback;
        utils::Timer<std::chrono::high_resolution_clock> m_timer;
    };


    bool init(GJGameLevel* p1){
        if (!EditLevelLayer::init(p1)) return false;
        auto menu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
        if(!menu) {
            log::error("failed to find level-actions-menu");
            return true;
        }
        
        auto deleteBtn = static_cast<MyCCMenuItemSpriteExtra*>(menu->getChildByID("delete-button"));
        if(!deleteBtn){
            log::error("failed to find delete-button");
            return true;
        }

        auto gif = CCGIFAnimatedSprite::create("load.gif"_spr);
        gif->m_delays.back() = FLT_MAX; // this the type of solution rob uses

        auto selectSprite = CircleButtonSprite::create(gif, CircleBaseColor::Green, CircleBaseSize::Medium);
        deleteBtn->setSelectedImage(selectSprite);
        m_fields->m_deleteCallback = deleteBtn->m_pfnSelector;
        

        deleteBtn->m_pfnSelector = menu_selector(MyEditLevelLayer::deleteActivate);
        (deleteBtn->m_fields)->m_selectCallback = menu_selector(MyEditLevelLayer::deleteSelect);
        return true;
    }

    void deleteActivate(CCObject* sender) {
        auto deleteBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        if(m_fields->m_timer.elapsed() > 500) {
            ((deleteBtn->m_pListener)->*(m_fields->m_deleteCallback))(this);
        }
    }

    void deleteSelect(CCObject*){
        m_fields->m_timer.reset();
    }
};
