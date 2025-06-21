#include <Geode/Geode.hpp>
#include "HoldButtonManager.hpp"
#include "ButtonActionManager.hpp"
using namespace geode::prelude;



//get a btn script

//takes a layer and id of a node in addition to sender and callback
//when it notices a load the it calls the call back from the sender

//Make a hashmap associated with with each (layer, id) pair and sender target

//first is the layer name
//second is the node id
//using NodeAddress = std::pair<std::string_view,std::string_view>;
using FunctionCallback = std::function<void(CCNode*)>;
//shit ton of maps probably

class NodeFinder{
    /*
    Takes a layer id and sender and if sender is the layer it returns the node related with the id.
    */
    //static NodeFinder* instance;
public:
    //std::unordered_map<NodeAddress, std::unordered_set<FunctionCallback>> m_addresses;
    //beautiful
    //theres no hash for FunctionCallback or smth
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, FunctionCallback>>> m_addresses; 

    /*static CCNode* find(std::string_view layer, std::string_view id, CCNode* sender) {
        if(sender->getID() == layer){
            auto node = sender->getChildByIDRecursive(id);
            if(!node) log::debug("Failed to find {}", id);
            return node;
        }
        return nullptr;
    }*/ // I have no clue why I made this

    static NodeFinder* get(){
        static NodeFinder* instance = nullptr;
        if(!instance){
            instance = new NodeFinder;
        }
        return instance;
    }

    void registerAddress(std::string const& layer, std::string const& nodeID, std::string const& callbackID, FunctionCallback callback){
        m_addresses[layer][nodeID][callbackID] = callback;
    }

    bool removeAddress(std::string const& layer, std::string const& nodeID, std::string const& callbackID){
        if(!m_addresses.contains(layer)){
            log::warn("{} does not have any registered callbacks.", layer);
            return false;
        } 
        if(!m_addresses[layer].contains(nodeID)) {
            log::warn("{} does not have any registered callbacks.",nodeID);
            return false;
        }
        if(!m_addresses[layer][nodeID].contains(callbackID)) {
            log::warn("{} has not been registered.", callbackID);
            return false;
        }

        m_addresses[layer][nodeID].erase(callbackID);

        //if(m_addresses[layer][nodeID][callbackID].empty()) m_addresses[layer][nodeID].erase(callbackID);
        if(m_addresses[layer][nodeID].empty()) m_addresses[layer].erase(nodeID); //this may be bad
        if(m_addresses[layer].empty()) m_addresses.erase(layer);
        return true;
    }

    bool run(CCNode* sender) {
        if(!m_addresses.contains(sender->getID())) return false;
        //if(!m_addresses[layer].contains(id)) return;
        //auto callbacks = m_addresses[layer][id];
        for(auto callbacks:m_addresses[sender->getID()]){
            auto node = sender->getChildByIDRecursive(callbacks.first); // do a thing that saves the path maybe??
            for(auto callback: callbacks.second) {
                callback.second(node);
            }
        }
        return true;
    }
};

#include <Geode/modify/CCLayer.hpp>
class $modify(MyCCLayer, CCLayer) {
    void onEnter() {//Make priority of everything very late
        CCLayer::onEnter();
        auto parent = typeinfo_cast<CCScene*>(getParent());
        if(!parent){
            return;
        }
        if(this->getID() == ""){
            return;
        }
        
        NodeFinder::get()->run(this);
        log::debug("This is from the {} with {} children", this->getID(), this->getChildrenCount());
        //auto node = typeinfo_cast<CCTransitionFade
        /*auto btnManager = HoldButtonManager::get();
        btnManager->registerBtn("help-button", this);
        btnManager->registerBtn("delete-button", this);
        //MyEditLevelLayer::registerBtn("edit-button", ffff, CircleBaseSize::Large);
        btnManager->registerBtn("play-button", this);
        btnManager->registerBtn("edit-button", this);
        //log::debug("this should fail"); // but hopefully not crash
        btnManager->registerBtn("pause-button", this);*/

    }

    bool ccTouchBegan(CCTouch* p1, CCEvent* p2){
        bool ret = CCLayer::ccTouchBegan(p1,p2);
        log::debug("{} touched with {}", this->getID(), ret);
        return ret;
    }
    /*bool init(){
        if(!CCLayer::init()) return false;
        log::debug("yah");
        return true;
    }*/
};

/*#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayer) {

}


$execute {

}*/