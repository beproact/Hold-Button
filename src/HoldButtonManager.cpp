#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"

//HoldButtonManager* HoldButtonManager::instance = nullptr;



HoldButtonManager* HoldButtonManager::get() {
    static HoldButtonManager* instance = nullptr;
    if(!instance){
        instance = new HoldButtonManager();
    }
    return instance;
}

void HoldButtonManager::registerWithNodeAndBase(CCNode* node, std::function<BasedButtonSprite*(CCSprite*)> createFunc){
    auto button = static_cast<MyCCMenuItemSpriteExtra*>(node);
    if(!button){
        log::error("failed to cast {}", node->getID());
        return;
    }
    auto gif = CCAnimatedSprite::createWithSpriteFrame(
        CCSpriteFrameCache::get()->spriteFrameByName("HoldLoadingA.png"_spr)
    );
    auto selectSprite = createFunc(gif);
    button->setSelectedImage(selectSprite);
    button->m_fields->m_originalCallback = button->m_pfnSelector;
    button->m_pfnSelector = nullptr;
    //button->m_pfnSelector = menu_selector(HoldButtonManager::btnActivate);
    //(button->m_fields)->m_selectCallback = menu_selector(HoldButtonManager::btnSelect);
    button->m_fields->m_selectCallback = std::bind(&HoldButtonManager::btnSelect, this, std::placeholders::_1);
    button->m_fields->m_unselectCallback = std::bind(&HoldButtonManager::btnUnselect, this, std::placeholders::_1);
    button->m_fields->m_activateCallback = std::bind(&HoldButtonManager::btnActivate, this, std::placeholders::_1);
}

void HoldButtonManager::registerWithNode(CCNode* node) { //final goal is to make user pass in a basedButtonSprite
    //but everything is protected cause geode hates me
    auto button = static_cast<MyCCMenuItemSpriteExtra*>(node);
    if(!button){
        log::error("failed to cast {}", node->getID());
        return;
    }
    auto gif = CCAnimatedSprite::createWithSpriteFrame(
        CCSpriteFrameCache::get()->spriteFrameByName("HoldLoadingA.png"_spr)
    );
    
    //add code here to try to guess what type of button it is
    auto guess = guessButton(static_cast<CCMenuItemSpriteExtra*>(node));
    //auto selectSprite = BasedButtonSprite::create(gif, guess.first, guess.second, static_cast<int>(CircleBaseColor::Green));
    auto selectSprite = CircleButtonSprite::create(nullptr, CircleBaseColor::Green, CircleBaseSize::Medium);
    //selectSprite->setTopRelativeScale(0.9);
    //selectSprite->init(gif, BaseType::Circle, 7, 0);
    
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
    BasedButtonSprite* sprite = typeinfo_cast<BasedButtonSprite*>(button->getSelectedImage());
    if(sprite) {
        auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
        if (animSprite) {
            animSprite->stopAction(m_animate);
            //log::debug("stopping actions");
            //animSprite->stopAllActions();
        }
    }
}

void HoldButtonManager::btnActivate(CCObject* sender) {
    auto button = static_cast<MyCCMenuItemSpriteExtra*>(sender);
    if(m_timer.elapsed() > 1000) {
        ((button->m_pListener)->*(button->m_fields->m_originalCallback))(sender); // this seems bad
    }
}

void HoldButtonManager::btnSelect(CCObject* sender){
    auto button = static_cast<CCMenuItemSpriteExtra*>(sender);

    m_timer.reset();    
    BasedButtonSprite* sprite = typeinfo_cast<BasedButtonSprite*>(button->getSelectedImage());
    if(sprite) {
        auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
        if (animSprite) {
            //animSprite->stopAllActions();
            animSprite->stopAction(m_animate);
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

// this should be in nodefinder i think?
// actually may be should be own class
std::pair<BaseType, int> HoldButtonManager::guessButton(CCMenuItemSpriteExtra* sender){
    auto size = sender->rect().size;
    log::debug("Size of: {}",size);
    if(size == CCSize(78.f, 81.5f)){
        return std::pair<BaseType, int>(BaseType::Circle, static_cast<int>(CircleBaseSize::Large));
    }
    return std::pair<BaseType, int>(BaseType::Circle, static_cast<int>(CircleBaseSize::Medium));
}
