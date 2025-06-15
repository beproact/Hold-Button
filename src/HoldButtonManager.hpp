// I dont't want to make a singleton
#include <Geode/Geode.hpp>

using namespace geode::prelude;

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
        //log::debug("{}",rect().size);
        CCMenuItemSpriteExtra::activate();
    }
}; 

class HoldButtonManager {
    static HoldButtonManager* instance;
    
    
public:
    static utils::Timer<std::chrono::high_resolution_clock>* m_timer;
    static CCAnimate* m_animate;

    HoldButtonManager(){
        embodyAnimate();
        //m_timer = new utils::Timer();
        //m_timer = new utils::Timer();
    }
    ~HoldButtonManager(){
        m_animate->release();
        delete(m_timer);
    }


    static HoldButtonManager* get() {
        //log::debug("FUNKNJDFSNK");
        if(!instance){
            instance = new HoldButtonManager();
            log::debug("this should only happen once");
        }
        return instance;
    }

    void registerWithBtn(MyCCMenuItemSpriteExtra* button,CircleBaseSize size) {
        
        auto gif = CCAnimatedSprite::createWithSpriteFrame(
            CCSpriteFrameCache::get()->spriteFrameByName("HoldLoadingA.png"_spr)
        );
        auto selectSprite = CircleButtonSprite::create(gif, CircleBaseColor::Green, size);

        button->setSelectedImage(selectSprite);
        button->m_fields->m_originalCallback = button->m_pfnSelector;
        button->m_pfnSelector = menu_selector(HoldButtonManager::btnActivate);
        (button->m_fields)->m_selectCallback = menu_selector(HoldButtonManager::btnSelect);
    }

    void registerBtn(std::string_view id, CCNode* menu, CircleBaseSize size){
        auto node = menu->getChildByIDRecursive(id);
        if(!node) {
            log::error("failed to find {}", id);
            return;
        }
        auto button = static_cast<MyCCMenuItemSpriteExtra*>(node); // I feel like this should break but it doesn't somehow
        if(!button){
            log::error("failed to cast {}", id);
            return;
        }
        log::debug("Making {} a hold button", id);
        registerWithBtn(button, size);
    }

    void registerBtn(std::string_view id, CCNode* menu) {
        registerBtn(id, menu, CircleBaseSize::Medium);
    }

    void btnActivate(CCObject* sender) {
        auto button = static_cast<MyCCMenuItemSpriteExtra*>(sender);
        if(m_timer && m_timer->elapsed() > 500) {
            ((button->m_pListener)->*(button->m_fields->m_originalCallback))(sender);
        }
    }

    void btnSelect(CCObject* sender){
        auto button = static_cast<CCMenuItemSpriteExtra*>(sender);
        //log::debug("<<<<");
        if(!m_timer){
            log::debug("timer is null");
            m_timer = new utils::Timer();
        }
        m_timer->reset();
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

private:
    //https://www.merriam-webster.com/thesaurus/instantiate
    void embodyAnimate(){
        auto frames = CCArray::create();

        for(int i = 'A'; i<='T'; i++) { 
            auto frameName = fmt::format("HoldLoading{}.png"_spr, static_cast<char>(i));
            //log::debug("{}",frameName);
            
            auto frame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str());
            if(!frame) {
                log::warn("Failed to get the frame {}", frameName);
            }
            frames->addObject(frame);
            //m_fields->m_animation->addSpriteFrame(CCSpriteFrameCache::get()->spriteFrameByName(ahhh));
        }
        auto animation = CCAnimation::createWithSpriteFrames(frames, 0.025);
        m_animate = CCAnimate::create(animation);
        m_animate->retain();
    }

    
};
HoldButtonManager* HoldButtonManager::instance = nullptr;
utils::Timer<std::chrono::high_resolution_clock>* HoldButtonManager::m_timer = nullptr;
CCAnimate* HoldButtonManager::m_animate = nullptr;
//ok making a singleton was not hard at all