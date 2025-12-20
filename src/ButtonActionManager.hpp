#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using MenuCallback = std::function<void(CCMenuItem*)>;

struct ButtonActions {
    MenuCallback activate = MenuCallback([](CCMenuItem* sender) {sender->activate();});
    MenuCallback selected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    MenuCallback unselected = MenuCallback([](CCMenuItem* sender) {sender->unselected();});
};
// this can just be a static class too I think?
class ButtonActionManager : CCObject {
    static inline ButtonActionManager* instance = nullptr;
    
public:
    ButtonActions actions;
    

    static ButtonActionManager* get() {
        if(!instance){
            instance = new ButtonActionManager();
            instance->retain();
        }
        return instance;
    }



    ButtonActionManager(){
        this->autorelease();
    }

    /*void setActivate(MenuCallback callback){
        activate = MenuCallback([](CCMenuItem* sender){

        });
    }*/

    void setActions(ButtonActions inst){
        actions = inst;
    }

    void resetActions(){
        actions = {};
    }

    void resetActivate(){
        actions.activate = MenuCallback([](CCMenuItem* sender) {sender->activate();});
    }

    void resetSelected(){
        actions.selected = MenuCallback([](CCMenuItem* sender) {sender->selected();});
    }

    void resetUnselected(){
        actions.unselected = MenuCallback([](CCMenuItem* sender) {sender->unselected();});
    }

};

#include <Geode/modify/CCMenuItemSpriteExtra.hpp> 
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
        ButtonActionManager::get()->actions.selected(this);
    }
    void activate(){
        ButtonActionManager::get()->actions.activate(this);
    }
    void unselected(){
        ButtonActionManager::get()->actions.unselected(this);
    }
};