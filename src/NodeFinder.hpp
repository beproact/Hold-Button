#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

using FunctionCallback = std::function<void(CCNode*)>;


/*
Node Path
List of NodeAddresses
array
vector???
*/
/* 

We need to have a map of top layer IDs to a list of paths along with callbacks.
When run is called with a top layer, we check if there are any registered callbacks for that top layer.
If there are, we go through each path and try to find the node.
Then we call the callback with the found node (or nullptr if not found).

This means we need a way to represent the path to a node or just the node ID and search recursively.

sum types would be nice
c++ has 
copilot what the fuck why do you hate variants


*/

//does this need to be a singleton can it just be a static class?
class NodeFinder : public CCObject{
    /*
    Takes a layer id and topLayer and if topLayer is the layer it returns the node related with the id.
    */
    //static NodeFinder* instance;
public:
    //std::unordered_map<NodeAddress, std::unordered_set<FunctionCallback>> m_addresses;
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, FunctionCallback>>> m_addresses; 

    static NodeFinder* get(){
        static NodeFinder* instance = nullptr;
        if(!instance){
            instance = new NodeFinder;
        }
        return instance;
    }

    /*
    Anything registered will be run once the node is created in the scene.
    */
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

    bool run(CCNode* topLayer) {
        if(!m_addresses.contains(topLayer->getID())) return false;
        //if(!m_addresses[layer].contains(id)) return;
        //auto callbacks = m_addresses[layer][id];
        for(auto callbacks:m_addresses[topLayer->getID()]){
            auto node = topLayer->getChildByIDRecursive(callbacks.first); // do a thing that saves the path maybe??
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
        //log::debug("This is from the {} with {} children", this->getID(), this->getChildrenCount());
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
};

