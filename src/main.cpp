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
//wtf are even c++ structs
//are they literally just objects but we have to be compatible with c code somehow and public default
struct BtnParameters: public CCObject {
    SEL_MenuHandler m_originalCallback;
    utils::Timer<std::chrono::high_resolution_clock> m_timer;
    BtnParameters(SEL_MenuHandler callback) : m_originalCallback(callback){
        m_timer = utils::Timer();
        this->autorelease();
    }
};


#include <Geode/modify/EditLevelLayer.hpp>
class $modify(MyEditLevelLayer, EditLevelLayer) {
    struct Fields{
        std::vector<CCSpriteFrame*> m_frames;
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

        //making btnSprite for when selected
        //auto gif = CCGIFAnimatedSprite::create("hh.gif"_spr);
        //auto gif = CCSprite::createWithSpriteFrameName("HoldLoadingA.png"_spr);
        //gif->m_delays.back() = FLT_MAX; // this the type of solution rob uses
        m_fields->m_frames.reserve(20); 
        for(int i = 'A'; i<='T'; i++) { // the most terrible code ever
            //auto texture = new CCTexture2D();
            //auto name = "HoldLoading" + sprintf("%i",i) + ".png";
            auto ahh = "HoldLoadingA.png"_spr; 
            char ahhh[17+sizeof(GEODE_MOD_ID)]; // GEODE_MOD_ID - \0 + / + 17 (include \0)
            memcpy(ahhh, ahh, 17+sizeof(GEODE_MOD_ID)); //ahhhhhhhhhhhhhhhhhh
            ahhh[sizeof(GEODE_MOD_ID) + 11] = i;
            log::debug("{}",ahhh);
            /* Hi me in the future
            This is going to cause a crash please fix it.
            */
            m_fields->m_frames.push_back(CCSpriteFrameCache::get()->spriteFrameByName(ahhh));
        }

        auto gif = CCSprite::createWithSpriteFrame(m_fields->m_frames.at(5));



        auto selectSprite = CircleButtonSprite::create(gif, CircleBaseColor::Green, CircleBaseSize::Medium);
        //selectSprite->setTopRelativeScale(0.9);



        deleteBtn->setSelectedImage(selectSprite);
        //m_fields->m_deleteCallback = deleteBtn->m_pfnSelector;
        deleteBtn->setUserObject(new BtnParameters(deleteBtn->m_pfnSelector));
        

        deleteBtn->m_pfnSelector = menu_selector(MyEditLevelLayer::btnActivate);
        (deleteBtn->m_fields)->m_selectCallback = menu_selector(MyEditLevelLayer::btnSelect);
        return true;
    }

    void btnActivate(CCObject* sender) {
        auto deleteBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto params = typeinfo_cast<BtnParameters*>(deleteBtn->getUserObject());
        if(params && params->m_timer.elapsed() > 500) {
            ((deleteBtn->m_pListener)->*(params->m_originalCallback))(this);
        }
    }

    void btnSelect(CCObject* sender){
        auto deleteBtn = static_cast<CCMenuItemSpriteExtra*>(sender);
        auto params = typeinfo_cast<BtnParameters*>(deleteBtn->getUserObject());
        if(params){
            params->m_timer.reset();;
        }
        
        CircleButtonSprite* sprite = typeinfo_cast<CircleButtonSprite*>(deleteBtn->getSelectedImage());
        if(sprite) {
            auto gif = typeinfo_cast<CCGIFAnimatedSprite*>(sprite->getTopNode());
            if(gif) {
                log::debug("funk");
                gif->m_index = 0;
                gif->m_elapsed = 0;
                gif->update(0);
            }
        }
    }
};
