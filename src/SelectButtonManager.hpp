#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class SelectButtonManager : public CCObject {
    CCMenuItem* m_currBtn = nullptr;
public:
    static SelectButtonManager* get(){
        static SelectButtonManager* instance = nullptr;
        if(!instance){
            instance = new SelectButtonManager;
        }
        CCScheduler::get()->scheduleUpdateForTarget(instance, 0, false);
        return instance;
    }

    void setButtons(){
        ButtonActionManager::get()->selected = MenuCallback([this](CCMenuItem* sender){
            /*if("change-button-state"_spr == sender->getID()){
                sender->selected();
                return;
            }*/

            if(m_currBtn){
                m_currBtn->release();
            }
            m_currBtn = sender;
            m_currBtn->retain();
            CCScheduler::get()->scheduleSelector(schedule_selector(SelectButtonManager::over), this, 0, 0, 0.5, false);
            sender->selected();
        });

        ButtonActionManager::get()->unselected = MenuCallback([this](CCMenuItem* sender){
            /*if("change-button-state"_spr == sender->getID()){
                sender->unselected();
                return;
            }*/

            if(m_currBtn){
                m_currBtn->release();
            }
            m_currBtn = nullptr;
            CCScheduler::get()->unscheduleSelector(schedule_selector(SelectButtonManager::over), this);
            //ButtonActionManager::get()->resetActivate();
            sender->unselected();
        });
        ButtonActionManager::get()->resetActivate();
    }

    void toggleManager(CCObject*){
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

    

    void over(float df){

        //because evil
        auto evil = SelectButtonManager::get();
        if(evil->m_currBtn){
            
            //log::debug("{}", evil->m_currBtn->getID());
            FLAlertLayer::create("Selector", fmt::format("{} selected", evil->m_currBtn->getID()), "OK")->show();
            SelectButtonManager::get()->m_currBtn->unselected();

            
            //CCTouchDispatcher::get()->setDispatchEvents(true);

            
            //find a better way of doing this
            ButtonActionManager::get()->activate = MenuCallback([evil](CCMenuItem* sender){
                if(sender->getID()== "button-1") {
                    sender->activate();
                    SelectButtonManager::get()->setButtons();
                    //evil->m_currBtn->unselected();
                }
            });
            
            ButtonActionManager::get()->unselected = MenuCallback([evil] (CCMenuItem* sender){
                if(sender->getID()== "button-1") {
                    sender->unselected();
                }
            });

            ButtonActionManager::get()->selected = MenuCallback([evil](CCMenuItem* sender){
                if(sender->getID()== "button-1") {
                    sender->selected();
                }
            });
        }
    }
    
};