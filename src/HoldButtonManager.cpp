#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"

//HoldButtonManager* HoldButtonManager::instance = nullptr;


HoldButtonManager* HoldButtonManager::get() {
    static HoldButtonManager* instance = nullptr;
    if(!instance){
        instance = new HoldButtonManager();
        //log::debug("this should only happen once");
        //log::debug("afterafterafter{}", instance->m_animate->retainCount());
    }
    log::debug("afterafterafter{}", instance->m_animate->retainCount());
    return instance;
}

void HoldButtonManager::registerWithNode(CCNode* node) { //final goal is to make user pass in a basedButtonSprite
    auto button = static_cast<MyCCMenuItemSpriteExtra*>(node);
    if(!button){
        log::error("failed to cast {}", node->getID());
        return;
    }
    auto gif = CCAnimatedSprite::createWithSpriteFrame(
        CCSpriteFrameCache::get()->spriteFrameByName("HoldLoadingA.png"_spr)
    );
    auto selectSprite = CircleButtonSprite::create(gif, CircleBaseColor::Green, CircleBaseSize::Medium);

    button->setSelectedImage(selectSprite);
    button->m_fields->m_originalCallback = button->m_pfnSelector;
    button->m_pfnSelector = nullptr;
    //button->m_pfnSelector = menu_selector(HoldButtonManager::btnActivate);
    //(button->m_fields)->m_selectCallback = menu_selector(HoldButtonManager::btnSelect);
    button->m_fields->m_selectCallback = std::bind(&HoldButtonManager::btnSelect, this, std::placeholders::_1);
    button->m_fields->m_unselectCallback = std::bind(&HoldButtonManager::btnUnselect, this, std::placeholders::_1);
    button->m_fields->m_activateCallback = std::bind(&HoldButtonManager::btnActivate, this, std::placeholders::_1);
}

void HoldButtonManager::registerBtn(std::string_view id, CCNode* menu){
    auto node = menu->getChildByIDRecursive(id);
    if(!node) {
        //log::debug("failed to find {}", id);
        return;
    }
    log::debug("Making {} a hold button", id);
    registerWithNode(node);
}


void HoldButtonManager::btnUnselect(CCObject* sender) {
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    CircleButtonSprite* sprite = typeinfo_cast<CircleButtonSprite*>(button->getSelectedImage());
    if(sprite) {
        auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
        if (animSprite) {
            log::debug("unselected");
            animSprite->stopAction(m_animate);
        }
    }
}

void HoldButtonManager::btnActivate(CCObject* sender) {
    auto button = static_cast<MyCCMenuItemSpriteExtra*>(sender);
    if(m_timer.elapsed() > 500) {
        ((button->m_pListener)->*(button->m_fields->m_originalCallback))(sender);
    }
}

void HoldButtonManager::btnSelect(CCObject* sender){
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
    //log::debug("<<<<");
    /*if(!m_timer){
        log::debug("timer is null");
        m_timer = new utils::Timer();
    }*/
    m_timer.reset();
    //log::debug(">>>>");
    
    CircleButtonSprite* sprite = typeinfo_cast<CircleButtonSprite*>(button->getSelectedImage());
    if(sprite) {
        auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
        if (animSprite) {
            animSprite->stopAllActions();
            animSprite->runAction(m_animate);
        }
    }
}


void HoldButtonManager::loadAnimate(){
    auto frames = CCArray::create();

    for(int i = 'A'; i<='T'; i++) { 
        auto frameName = fmt::format("HoldLoading{}.png"_spr, static_cast<char>(i));
        //log::debug("{}",frameName);
        
        auto frame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str());
        if(!frame) {
            log::warn("Failed to get the frame {}", frameName);
            continue;
        }
        frames->addObject(frame);
        //m_fields->m_animation->addSpriteFrame(CCSpriteFrameCache::get()->spriteFrameByName(ahhh));
    }
    auto animation = CCAnimation::createWithSpriteFrames(frames, 0.025);
    m_animate = CCAnimate::create(animation);

    CC_SAFE_RETAIN(m_animate);
    
}

