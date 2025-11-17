#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

using FunctionCallback = std::function<void(CCNode*)>;

//give this a better name
struct NodeID {
    std::string idName;
    int idIndex;

public:
    NodeID(std::string name, int index){
        idName = name;
        idIndex=index;
    }

    // friend std::ostream& operator << (std::ostream& os, NodeID const& p) {
    //     return os << ((p.idName != "") ? p.idName : std::to_string(p.idIndex));
    // }
    std::string to_string() {
        return (idName != "") ? idName : "num"+std::to_string(idIndex);
    }
    
    bool operator< (const NodeID& other) const {        
        if (idName != other.idName){
            return idName < other.idName;
        }

        return idIndex < other.idIndex;
    }
};

/*
Node Path
List of NodeAddresses
array
vector???
*/

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

