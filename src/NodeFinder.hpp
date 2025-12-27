#pragma once

#include <Geode/Geode.hpp>
#include "NodePath.hpp"

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
c++ has variants


*/

// using Address = std::variant<NodePath, std::string>; 

// Map to map to map of tuples with address should probably have an ID given with it.

struct Address { // give better name;
    std::variant<NodePath, std::string> path;
    FunctionCallback callback;
    
}; // don't love the fact that the id is not part of the struct


//does this need to be a singleton can it just be a static class?
class NodeFinder : public CCNode{
    std::unordered_map<std::string, std::list<std::tuple<Ref<CCNode>, Address, Ref<CCNode>>>> m_foundNodes; 
    // Node to watch, Address, TopLayer
    /*
    Takes a layer id and topLayer and if topLayer is the layer it returns the node related with the id.
    */
    //static NodeFinder* instance;
public:
    //std::unordered_map<NodeAddress, std::unordered_set<FunctionCallback>> m_addresses;
    // std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, FunctionCallback>>> m_addresses; 
    std::unordered_map<std::string, std::unordered_map<std::string, Address>> m_addresses; 
    // Map from top layer id to map of all (CallbackId, Address) pairs.
    
    static NodeFinder* get(){
        static NodeFinder* instance = nullptr;
        if(!instance){
            instance = new NodeFinder;
        }
        CCScheduler::get()->scheduleUpdateForTarget(instance, 0, false);
        return instance;
    }

    /*
    Anything registered will be run once the node is created in the scene.
    */
    void registerAddress(std::string const& layer, std::string nodeID, std::string const& callbackID, FunctionCallback callback){
        m_addresses[layer][callbackID] = Address{nodeID, callback};
    }

    void registerAddress(std::string const& layer, NodePath path, std::string const& callbackID, FunctionCallback callback){
        m_addresses[layer][callbackID] = Address{path, callback};
    }

    bool removeAddress(std::string const& layer,  std::string const& callbackID){
        if(!m_addresses.contains(layer)){
            log::warn("{} does not have any registered callbacks.", layer);
            return false;
        } 
        if(!m_addresses[layer].contains(callbackID)) {
            log::warn("{} does not have any registered callbacks.", callbackID);
            return false;
        }
        m_addresses[layer].erase(callbackID);
        //if(m_addresses[layer][nodeID][callbackID].empty()) m_addresses[layer][nodeID].erase(callbackID);
        if(m_addresses[layer].empty()) m_addresses.erase(layer); // not sure if this is needed
        return true;
    }

    bool run(CCNode* topLayer) {
        if(!m_addresses.contains(topLayer->getID())) return false;
        log::debug("Running NodeFinder for layer {}", topLayer->getID());
        //if(!m_addresses[layer].contains(id)) return;
        //auto callbacks = m_addresses[layer][id];
        for(auto map : m_addresses[topLayer->getID()]) {
            log::debug("Calling callback {}", map.first);
            call(map.second, topLayer);
        }
        return true;
    }

    void call(Address const& address, CCNode* topLayer){
        if(std::holds_alternative<std::string>(address.path)){ //contains only ID
            auto id = std::get<std::string>(address.path);
            auto node = topLayer->getChildByIDRecursive(id);
            address.callback(node);

            m_foundNodes[topLayer->getID()].emplace_back(node, address, topLayer);
            return;
        }

        // change this to recursive at some point  
        auto path = std::get<NodePath>(address.path);
        CCNode* curr = topLayer;

        std::list<NodeID>::iterator it = path.path.begin();
        std::list<NodeID>::iterator end = path.path.end();
        auto modifiedNode = callRecursive(it, end, address, curr);
        m_foundNodes[topLayer->getID()].emplace_back(modifiedNode, address, topLayer);
        // for(auto const& nodeID : path.path){
        //     auto children = curr->getChildrenExt();
        //     std::string id = nodeID.idName;
        //     int index = nodeID.idIndex;

        //     auto filtered = utils::ranges::filter(children, [id](CCNode* sibling){
        //         return sibling->getID() == id;
        //     }); // children that match ID

        //     if(filtered.size() <= index){
        //         log::warn("Could not find node with ID {} at index {} among its siblings.", id, index);
        //         return;
        //     }
        //     curr = filtered[index];
        // }
        // address.callback(curr);
    }

    //Returns one of the modified nodes
    CCNode* callRecursive(std::list<NodeID>::iterator it, std::list<NodeID>::iterator end, Address address , CCNode* curr){
        
        if(it == end){
            address.callback(curr);
            return curr;
        }
        // log::debug("Looking for ID {} at index {}", it->idName, it->idIndex);

        auto children = curr->getChildrenExt();
        std::string id = it->idName;
        int index = it->idIndex;
        auto filtered = utils::ranges::filter(children, [id](CCNode* sibling){
            return sibling->getID() == id;
        }); // children that match ID

        if (index < 0){
            CCNode* lastNode = nullptr;
            for(auto node : filtered){
                lastNode = callRecursive(std::next(it), end, address, node);
            }
            return lastNode;
        }
        if(filtered.size() <= index){
            log::warn("Could not find node with ID {} at index {} among its siblings.", id, index);
            return nullptr;
        }
        
        curr = filtered[index];

        return callRecursive(std::next(it), end, address, curr);
    }
    
    void update(float df){ // i have decided iterators are evil
        for (auto const& [topLayerID, nodes] : m_foundNodes){
            for (auto it = nodes.begin(); it != nodes.end(); ) {
                auto [node, address, top] = *it;
                if(node && node->isRunning()){
                    ++it;
                    continue;
                } 
                it = m_foundNodes[topLayerID].erase(it);
                log::debug("Node {} is no longer running, re-calling callback", node->getID());
                call(address, top);
            }
        }   
    }

    void remove(CCNode* topLayer){
        if(!m_foundNodes.contains(topLayer->getID())) return;
        for(auto const& tuple : m_foundNodes[topLayer->getID()]){
            auto [node, address, top] = tuple;
            log::debug("Removing node {} from ones checked", node->getID());
        }
        m_foundNodes.erase(topLayer->getID());
    }
};

#include <Geode/modify/CCLayer.hpp>
class $modify(MyCCLayer, CCLayer) {
    
    void onExit() {//Make priority of everything very late
        CCLayer::onExit();
        auto parent = typeinfo_cast<CCScene*>(getParent());
        if(!parent){
            return;
        }
        if(this->getID() == ""){
            return;
        }
    
        NodeFinder::get()->remove(this);
    }
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

