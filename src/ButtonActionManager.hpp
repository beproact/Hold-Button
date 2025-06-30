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
    MenuCallback unselected = MenuCallback([](CCMenuItem* sender) {sender->unselected();});
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

    /*void setActivate(MenuCallback callback){
        activate = MenuCallback([](CCMenuItem* sender){

        });
    }*/

    void resetActivate(){
        activate = MenuCallback([](CCMenuItem* sender) {sender->activate();});
    }

    void resetSelected(){
        selected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    }

    void resetUnselected(){
        unselected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    }

};

class OtherButtonManager : public ButtonActionManager{
    void selected(CCMenuItem* sender){
        log::debug("now does stuff on select");
        sender->selected();
    }
};

class $modify(DifferentThing, CCMenuItemSpriteExtra){
    static void onModify(auto& self) {
        if (!self.setHookPriorityPre("CCMenuItemSpriteExtra::selected", Priority::First)) {
            geode::log::warn("Failed to set hook priority.");
        }
        if (!self.setHookPriorityPre("CCMenuItemSpriteExtra::activate", Priority::First)) {
            geode::log::warn("Failed to set hook priority.");
        }
        if (!self.setHookPriorityPre("CCMenuItemSpriteExtra::unselected", Priority::First)) {
            geode::log::warn("Failed to set hook priority.");
        }
    }

    void selected(){
        log::debug("2");
        ButtonActionManager::get()->selected(this);
    }
    void activate(){
        ButtonActionManager::get()->activate(this);
    }
    void unselected(){
        ButtonActionManager::get()->unselected(this);
    }
};