#include <Geode/Geode.hpp>
#include "SelectCallback.hpp"


using namespace geode::prelude;
#include <user95401.gif-sprites/include/CCGIFAnimatedSprite.hpp>

#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
public:
	struct Fields {
		SEL_MenuHandler m_selectCallback = nullptr;
	};

	void selected() {
		if(m_fields->m_selectCallback){
			(m_pListener->*(m_fields->m_selectCallback))(this);
		}
		CCMenuItemSpriteExtra::selected();
	}
};

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
        CCArray* m_frames;
        CCAnimation* m_animation;
        //CCAnimate* m_animate;
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

        
        auto frames = CCArray::create();
        frames->retain();
        //
        for(int i = 'A'; i<='T'; i++) { // the most terrible code ever
            auto frameName = fmt::format("HoldLoading{}.png"_spr, static_cast<char>(i));
            log::debug("{}",frameName);
            
            auto frame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str());
            if(!frame) {
                log::warn("Failed to get the frame {}", frameName);
            }
            frames->addObject(frame);

            //m_fields->m_animation->addSpriteFrame(CCSpriteFrameCache::get()->spriteFrameByName(ahhh));
        }
        
        auto animation = CCAnimation::createWithSpriteFrames(frames, 0.1f);
       
        //m_fields->m_animate = CCAnimate::create(m_fields->m_animation);
        m_fields->m_animation = animation;
        m_fields->m_frames=frames;
        auto gif = CCAnimatedSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(frames->objectAtIndex(5)));
        auto fs =animation->getDuration();


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
            auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
            if (animSprite) {
                
                animSprite->stopAllActions();
                animSprite->runAction(CCAnimate::create(CCAnimation::createWithSpriteFrames(m_fields->m_frames, 0.025f)));
            }
        }
    }
};
