#include <Geode/Geode.hpp>
#include "SelectButtonManager.hpp"


using namespace geode::prelude;

SelectButtonManager* SelectButtonManager::get(){
    static SelectButtonManager* instance = nullptr;
    if(!instance){
        instance = new SelectButtonManager;
    }
    CCScheduler::get()->scheduleUpdateForTarget(instance, 0, false);
    return instance;
}

void SelectButtonManager::setButtons(){

    ButtonActions actions;
    actions.selected = MenuCallback([this](CCMenuItem* sender){
        /*if("change-button-state"_spr == sender->getID()){
            sender->selected();
            return;
        }*/
        if(s_popUp && !sender->hasAncestor(s_popUp.data())){
            return;
        }
        
        m_currBtn = sender;
        CCScheduler::get()->scheduleSelector(schedule_selector(SelectButtonManager::over), this, 0, 0, 0.5, false);
        sender->selected();
    });
    actions.unselected = MenuCallback([this](CCMenuItem* sender){

        /*if("change-button-state"_spr == sender->getID()){
            sender->unselected();
            return;
        }*/

        m_currBtn = nullptr;
        CCScheduler::get()->unscheduleSelector(schedule_selector(SelectButtonManager::over), this);
        //ButtonActionManager::get()->resetActivate();
        sender->unselected();
    });

    actions.activate = MenuCallback([this](CCMenuItem* sender){
        if(s_popUp && !sender->hasAncestor(s_popUp)){
            return;
        }
        s_popUp = nullptr;
        sender->activate();
    });
    ButtonActionManager::get()->setActions(actions);
}

void SelectButtonManager::toggleManager(CCObject*){
    static bool state = true;
    if(state){
        state = false;
        //log::debug("haiufhsi");
        setButtons();
        return;
    }
    ButtonActionManager::get()->resetSelected();
    state = true;
}

CCNode* SelectButtonManager::getTopLayer(CCNode* curr){
    while(curr && !typeinfo_cast<CCScene*>(curr->getParent())){ // while parent exists and parent is not CCScene*
        curr = curr->getParent();
    }
    return curr;
}

Result<std::vector<std::string>> SelectButtonManager::getPath(CCNode* curr){
    if(curr->getID() == ""){
        return Err("The button you tried to select does not have an ID.");
    }

    std::vector<std::string> path;
    path.push_back(curr->getID());
    while(curr && !typeinfo_cast<CCScene*>(curr->getParent())){ // while parent exists and parent is not CCScene*
        curr = curr->getParent();
        if(curr->getID() == ""){
            return Err("One of the nodes in the path does not have an ID.");
        }
        path.push_back(curr->getID());
    }
    return Ok(path);
}



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

void SelectButtonManager::over(float df){

    //because evil
    auto evil = SelectButtonManager::get();
    if(evil->m_currBtn){
        
        //log::debug("{}", evil->m_currBtn->getID());
        auto top = getTopLayer(evil->m_currBtn);
        std::string topName = "";
        if(top){
            topName = top->getID();
        }
        auto path = getPath(evil->m_currBtn);
        auto pathNum = NodePath::create(evil->m_currBtn);
        
        s_popUp = FLAlertLayer::create("Selector", fmt::format("{} selected", pathNum.to_string()), "OK");

        s_popUp->setID("SelectAlert"_spr);
        s_popUp->show();

        /*if(path.isErr()){
            FLAlertLayer::create("Selector", fmt::format("{}", path.err().value()), "OK")->show();
        } else {
            FLAlertLayer::create("Selector", fmt::format("{} selected", path.ok().value()), "OK")->show();
        }*/

        SelectButtonManager::get()->m_currBtn->unselected();
    }
} 
