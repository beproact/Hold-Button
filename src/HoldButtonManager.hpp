// I dont't want to make a singleton
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class HoldButtonManager : CCNode {
    static HoldButtonManager* instance;
public:
    utils::Timer<std::chrono::high_resolution_clock> m_timer;
    CCAnimate* m_animate;
    HoldButtonManager(){
        embodyAnimate();
        //CC_SAFE_RETAIN(m_animate);
        //CC_SAFE_RETAIN(m_animate);
        //log::debug("afterafter{}", m_animate->retainCount());
        m_timer = utils::Timer();
        //m_timer = new utils::Timer();
        //m_timer = new utils::Timer();
    }
    ~HoldButtonManager(){
        log::debug("deleting");
        m_animate->release();
        //delete(m_timer);
    }


    static HoldButtonManager* get() {
        //log::debug("FUNKNJDFSNK");
        if(!instance){
            instance = new HoldButtonManager();
            log::debug("this should only happen once");
            log::debug("afterafterafter{}", instance->m_animate->retainCount());
        }
        log::debug("afterafterafter{}", instance->m_animate->retainCount());
        return instance;
    }

    void registerWithNode(CCNode* node ,CircleBaseSize size);

    void registerBtn(std::string_view id, CCNode* menu, CircleBaseSize size);

    void registerBtn(std::string_view id, CCNode* menu) {
        registerBtn(id, menu, CircleBaseSize::Medium);
    }

    void btnUnselect(CCObject* sender);

    void btnActivate(CCObject* sender);

    void btnSelect(CCObject* sender);

private:
    //https://www.merriam-webster.com/thesaurus/instantiate
    void embodyAnimate();
};




#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
public:
	struct Fields {
		//SEL_MenuHandler m_selectCallback = nullptr;
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
        //auto manager = HoldButtonManager::get();
        //manager->btnSelect(this);
		CCMenuItemSpriteExtra::selected();
	}

    void activate() {
        //log::debug("{}",rect().size);
        //auto manager = HoldButtonManager::get();
        //manager->m_pfnSelector(this);
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


//utils::Timer<std::chrono::high_resolution_clock>* HoldButtonManager::m_timer = nullptr;
//CCAnimate* HoldButtonManager::m_animate = nullptr;
//ok making a singleton was not hard at all