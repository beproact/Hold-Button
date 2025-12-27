#pragma once
#include <Geode/Geode.hpp>
#include "NodePath.hpp"
#include "ButtonActionManager.hpp"

using namespace geode::prelude;

class SelectButtonManager : public CCObject {
    Ref<CCMenuItem> m_currBtn = nullptr;
    Ref<FLAlertLayer> s_popUp = nullptr;
public:
    static SelectButtonManager* get();

    void setButtons();

    void toggleManager(CCObject*);

    CCNode* getTopLayer(CCNode* curr);

    Result<std::vector<std::string>> getPath(CCNode* curr);

    

    // int getNumID(CCNode* node) {
    //     auto parent = node->getParent();
    //     auto siblings = parent->getChildren();
    //     return siblings->indexOfObject(node);
    // }

    // std::vector<NodeID> getPathNum(CCNode* curr){
    //     std::vector<NodeID> path;
    //     //struct NodeID test = {curr->getID(),getNumID(curr)};
    //     path.push_back({curr->getID(),getNumID(curr)});
    //     while(curr && !typeinfo_cast<CCScene*>(curr->getParent())){ // while parent exists and parent is not CCScene*
    //         curr = curr->getParent();
    //         path.push_back({curr->getID(),getNumID(curr)});
    //     }
    //     return path;
    // }

    void over(float df);

    
};