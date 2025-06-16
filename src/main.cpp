#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"

using namespace geode::prelude;

/*#include <Geode/modify/CCAnimate.hpp>
class $modify(MyCCAnimate, CCAnimate) {
    void release(){ // REALLY BAD IDEA

        log::debug("release");
        //release();
    }
};*/

#include <Geode/modify/CCLayer.hpp>
class $modify(MyCCLayer, CCLayer) {
    void onEnterTransitionDidFinish() {//Make priority of everything very late
        CCLayer::onEnter();
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
        //log::debug("this should fail"); // but hopefully not crash
        btnManager->registerBtn("pause-button", this);
    }
    /*bool init(){
        if(!CCLayer::init()) return false;
        log::debug("yah");
        return true;
    }*/
};

//get a btn script

//takes a layer and id of a node in addition to sender and callback
//when it notices a load the it calls the call back from the sender

//Make a hashmap associated with with each (layer, id) pair and sender target


using NodeAddress = std::pair<std::string_view,std::string_view>;

class NodeFinder{
    /*
    Takes a layer id and sender and if sender is the layer it returns the node related with the id.
    */
    static NodeFinder* instance;
public:
    static std::unordered_map<NodeAddress, std::function<void(CCNode*)>> m_addresses;
    static CCNode* find(std::string_view layer, std::string_view id, CCNode* sender) {
        if(sender->getID() == layer){
            auto node = sender->getChildByIDRecursive(id);
            if(!node) log::debug("Failed to find {}", id);
            return node;
        }
        return nullptr;
    }

    static NodeFinder* get(){
        if(!instance){
            instance = new NodeFinder;
        }
        return instance;
    }

    static void registerAddress(NodeAddress address, std::function<void(CCNode*)> callback){
        
        
    }
};

NodeFinder* NodeFinder::instance = nullptr;