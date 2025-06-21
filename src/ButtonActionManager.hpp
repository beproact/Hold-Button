#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class ButtonActionManager : CCObject {
    static ButtonActionManager* instance;
public:
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

    virtual void activate(CCMenuItem* sender){
        log::debug("activate the main");
        sender->activate();
    }

    virtual void selected(CCMenuItem* sender){
        sender->selected();
    }

    virtual void unselected(CCMenuItem* sender){
        sender->unselected();
    }
};
ButtonActionManager* ButtonActionManager::instance = nullptr;

class OtherButtonManager : public ButtonActionManager{
    void selected(CCMenuItem* sender){
        log::debug("now does stuff on select");
        sender->selected();
    }
};

class $modify(CCMenuItemSpriteExtra){
    void selected(){
        auto buttonManager = ButtonActionManager::get();
        buttonManager->selected(this);
    }
    void activate(){
        auto buttonManager = ButtonActionManager::get();
        
        buttonManager->activate(this);
        //auto other = new OtherButtonManager();
        //ButtonActionManager::set(other);
    }
    void unselected(){
        auto buttonManager = ButtonActionManager::get();
        buttonManager->unselected(this);
    }
};