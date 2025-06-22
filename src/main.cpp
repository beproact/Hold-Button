#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"
#include "ButtonActionManager.hpp"
#include "NodeFinder.hpp"
using namespace geode::prelude;







#include <Geode/modify/CCScene.hpp>
class $modify(CCScene){
    static CCScene* create(){
        auto ret = CCScene::create();
        //log::debug("shit");
        //auto toggle = ToggleButton::create("HoldLoadingA.png"_spr);
        return ret;
    }
};

/*#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {

};


$execute {

}*/
$on_mod(Loaded){
    auto finder = NodeFinder::get();
    finder->registerAddress("EditLevelLayer", "edit-button", "AHHH", [](CCNode* sender){
        HoldButtonManager::get()->registerWithNode(sender);
    });
    finder->registerAddress("EditLevelLayer", "play-button", "AHHHH", [](CCNode* sender){
        HoldButtonManager::get()->registerWithNode(sender);
    });
}