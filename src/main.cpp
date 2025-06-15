#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"

using namespace geode::prelude;

#include <Geode/modify/CCScene.hpp>
class $modify(MyCCScene, CCScene){
    // bool init(){
    //     if(!CCScene::init()) return false;
    //     //log::debug("{}",this->getChildren()->count());
    //     auto btnManager = HoldButtonManager::get();
    //     btnManager->registerBtn("help-button", this);
    //     btnManager->registerBtn("delete-button", this);
    //     //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
    //     btnManager->registerBtn("play-button", this, CircleBaseSize::Large);
    //     btnManager->registerBtn("edit-button", this, CircleBaseSize::Large);
    //     return true;
    // }
    /*CCScene* create(){
        auto ret = CCScene::create();
        
        auto btnManager = HoldButtonManager::get();
        btnManager->registerBtn("help-button", this);
        btnManager->registerBtn("delete-button", this);
        //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        btnManager->registerBtn("play-button", this, CircleBaseSize::Large);
        btnManager->registerBtn("edit-button", this, CircleBaseSize::Large);
        return ret;
    }*/
};

#include <Geode/modify/CCLayer.hpp>
class $modify(MyCCLayer, CCLayer) {
    void onEnterTransitionDidFinish() {
        auto parent = typeinfo_cast<CCScene*>(getParent());
        if(!parent){
            return;
        }
        if(this->getID() == ""){
            return;
        }
        log::debug("This is from the {} with {} children", this->getID(), this->getChildrenCount());
        //auto node = typeinfo_cast<CCTransitionFade
        auto btnManager = HoldButtonManager::get();
        btnManager->registerBtn("help-button", this);
        btnManager->registerBtn("delete-button", this);
        //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        btnManager->registerBtn("play-button", this, CircleBaseSize::Large);
        btnManager->registerBtn("edit-button", this, CircleBaseSize::Large);
        log::debug("this should fail"); // but hopefully not crash
        btnManager->registerBtn("pause-button", this);
    }
    /*bool init(){
        if(!CCLayer::init()) return false;
        log::debug("yah");
        return true;
    }*/
};

/*#include <Geode/modify/EditLevelLayer.hpp>
class $modify(MyEditLevelLayer, EditLevelLayer) {

    bool init(GJGameLevel* p1){
        if (!EditLevelLayer::init(p1)) return false;
        
        auto menu = static_cast<CCMenu*>(getChildByID("level-actions-menu"));
        auto ffff = static_cast<CCMenu*>(getChildByID("level-edit-menu"));


        auto btnManager = HoldButtonManager::get();
        btnManager->registerBtn("help-button", this);
        btnManager->registerBtn("delete-button", this);
        //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        btnManager->registerBtn("play-button", this, CircleBaseSize::Large);
        btnManager->registerBtn("edit-button", this, CircleBaseSize::Large);
        
        return true;
    }
};*/