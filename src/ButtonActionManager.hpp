#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using MenuCallback = std::function<void(CCMenuItem*)>;
/*class ButtonActions : CCObject{
    
};*/
//we can change this to be just a bunch of 
class ButtonActionManager : CCObject {
    static inline ButtonActionManager* instance = nullptr;
    
public:
    MenuCallback activate = MenuCallback([](CCMenuItem* sender) {sender->activate();});
    MenuCallback selected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    MenuCallback unselected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    static ButtonActionManager* get() {
        if(!instance){
            instance = new ButtonActionManager();
            instance->retain();
        }
        return instance;
    }

    static void set(ButtonActionManager* inst){
        if(instance){
            instance->release();
        }
        instance = inst;
        inst->retain();
    }

    ButtonActionManager(){
        this->autorelease();
    }

    /*virtual void activate(CCMenuItem* sender){
        sender->activate();
    }

    virtual void selected(CCMenuItem* sender){
        sender->selected();
    }

    virtual void unselected(CCMenuItem* sender){
        sender->unselected();
    }*/
};

class OtherButtonManager : public ButtonActionManager{
    void selected(CCMenuItem* sender){
        log::debug("now does stuff on select");
        sender->selected();
    }
};

class $modify(CCMenuItemSpriteExtra){
    void selected(){
        ButtonActionManager::get()->selected(this);
    }
    void activate(){
        ButtonActionManager::get()->activate(this);
    }
    void unselected(){
        ButtonActionManager::get()->unselected(this);
    }
};