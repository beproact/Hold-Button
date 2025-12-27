#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"
#include "ButtonActionManager.hpp"
#include "NodeFinder.hpp"
#include "SelectButtonManager.hpp"
using namespace geode::prelude;



#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init())return false;
		
        auto sprite = CircleButtonSprite::create(nullptr, CircleBaseColor::Green, CircleBaseSize::Small);
        auto button = CCMenuItemSpriteExtra::create(sprite, SelectButtonManager::get(), menu_selector(SelectButtonManager::toggleManager));
        //auto winSize = CCDirector::get()->getWinSize();
        //auto menu = CCMenu::create();
        auto menu = getChildByID("side-menu");
        button->setID("change-button-state"_spr);

        //menu->setPosition((winSize.width/64)*6 ,(winSize.height/16)*15);
        menu->addChild(button);
        //auto thing = static_cast<CCMenu*>(getChildByID("side-menu"));
        //menu->setZOrder(100);
        
        //addChild(menu);
        //SceneManager::get()->keepAcrossScenes(menu);
        menu->setLayout(ColumnLayout::create(), true, true);
		return true;
	}

    
    
};

$on_mod(Loaded){
    auto finder = NodeFinder::get();
    finder->registerAddress("EditLevelLayer", "edit-button", "AHHH", [](CCNode* sender){
        HoldButtonManager::get()->registerWithNodeAndBase(
            sender,
            BASED_BUTTON_CREATE_FUNCTION(CircleButtonSprite, CircleBaseColor::Green, CircleBaseSize::Large)
        );
    });

    finder->registerAddress("EditLevelLayer", "play-button", "AHHHH", [](CCNode* sender){
        HoldButtonManager::get()->registerWithNodeAndBase(
            sender,
            BASED_BUTTON_CREATE_FUNCTION(CrossButtonSprite, CrossBaseColor::Green, CrossBaseSize::Small)
        );
    });
}