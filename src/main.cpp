#include <Geode/Geode.hpp>
#include "SelectCallback.hpp"


using namespace geode::prelude;
#include <user95401.gif-sprites/include/CCGIFAnimatedSprite.hpp>

#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
public:
	struct Fields {
		SEL_MenuHandler m_selectCallback = nullptr;
        SEL_MenuHandler m_originalCallback = nullptr;
	};

	void selected() {
		if(m_fields->m_selectCallback){
			(m_pListener->*(m_fields->m_selectCallback))(this);
		}
		CCMenuItemSpriteExtra::selected();
	}

    void activate() {
        log::debug("{}",rect().size);
        CCMenuItemSpriteExtra::activate();
    }
};

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
        utils::Timer<std::chrono::high_resolution_clock>* m_timer = new utils::Timer();
    };

    bool init(GJGameLevel* p1){
        if (!EditLevelLayer::init(p1)) return false;
        auto menu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
        auto ffff = static_cast<CCMenu*>(getChildByID("level-edit-menu"));
        if(!menu) {
            log::error("failed to find level-actions-menu");
            return true;
        }
        
        auto frames = CCArray::create();
        frames->retain();

        for(int i = 'A'; i<='T'; i++) { 
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
        m_fields->m_frames=frames;
        


        MyEditLevelLayer::registerBtn("help-button", menu);
        MyEditLevelLayer::registerBtn("delete-button", menu);
        //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        MyEditLevelLayer::registerBtn("play-button", ffff, CircleBaseSize::Large);
        MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        
        return true;
    }

    void registerBtn(std::string_view id,CCMenu* menu) {
        registerBtn(id, menu, CircleBaseSize::Medium);
    }

    void registerBtn(std::string_view id, CCMenu* menu, CircleBaseSize size){
        auto gif = CCAnimatedSprite::createWithSpriteFrame(static_cast<CCSpriteFrame*>(m_fields->m_frames->objectAtIndex(5)));
        auto selectSprite = CircleButtonSprite::create(gif, CircleBaseColor::Green, size);
        auto button = static_cast<MyCCMenuItemSpriteExtra*>(menu->getChildByID(id));
        if(!button){
            log::error("failed to find delete-button");
            return;
        }
        button->setSelectedImage(selectSprite);
        button->setUserObject(new BtnParameters(button->m_pfnSelector));
        button->m_fields->m_originalCallback = button->m_pfnSelector;
        button->m_pfnSelector = menu_selector(MyEditLevelLayer::btnActivate);
        (button->m_fields)->m_selectCallback = menu_selector(MyEditLevelLayer::btnSelect);
    }

    void btnActivate(CCObject* sender) {
        auto button = static_cast<MyCCMenuItemSpriteExtra*>(sender);
        auto params = typeinfo_cast<BtnParameters*>(button->getUserObject());
        if(m_fields->m_timer && m_fields->m_timer->elapsed() > 500) {
            ((button->m_pListener)->*(button->m_fields->m_originalCallback))(this);
        }
    }

    void btnSelect(CCObject* sender){
        auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
        if(m_fields->m_timer){
            m_fields->m_timer->reset();
        }
        
        CircleButtonSprite* sprite = typeinfo_cast<CircleButtonSprite*>(button->getSelectedImage());
        if(sprite) {
            auto animSprite = static_cast<CCAnimatedSprite*>(sprite->getTopNode());
            if (animSprite) {
                animSprite->stopAllActions();
                animSprite->runAction(CCAnimate::create(CCAnimation::createWithSpriteFrames(m_fields->m_frames, 0.025f)));
            }
        }
    }
};
