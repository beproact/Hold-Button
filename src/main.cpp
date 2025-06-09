#include <Geode/Geode.hpp>
#include "CCMenuItemSpriteExtraHold.hpp"


using namespace geode::prelude;

#include <Geode/modify/EditLevelLayer.hpp>
class $modify(MyEditLevelLayer, EditLevelLayer) {

    struct Fields {
        SEL_MenuHandler m_deleteCallback;
    };


    bool init(GJGameLevel* p1){
        if (!EditLevelLayer::init(p1)) return false;
        auto menu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
        if(!menu) {
            log::error("failed to find level-actions-menu");
            return true;
        }
        
        auto deleteBtn = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("delete-button"));
        if(!deleteBtn){
            log::error("failed to find delete-button");
            return true;
        }

        auto holdDeleteBtn = CCMenuItemSpriteExtraHold::create(
            deleteBtn->getNormalImage(),
            deleteBtn->m_pListener,
            menu_selector(MyEditLevelLayer::deleteActivate)
        );

        holdDeleteBtn->setID(deleteBtn->getID());
        m_fields->m_deleteCallback = deleteBtn->m_pfnSelector;

        menu->removeChild(deleteBtn, true);
        menu->addChild(holdDeleteBtn,-1);
        menu->updateLayout();
        return true;
    }

    void deleteActivate(CCObject* sender) {
        auto deleteBtn = static_cast<CCMenuItemSpriteExtraHold*>(sender);
        if(deleteBtn->m_timePressed > 500) {
            ((deleteBtn->m_pListener)->*(m_fields->m_deleteCallback))(this);
        }
    }
};
