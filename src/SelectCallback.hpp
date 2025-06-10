#pragma once
#include <Geode/Geode.hpp>
#include <Geode/utils/timer.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
class $modify(MyCCMenuItemSpriteExtra, CCMenuItemSpriteExtra) {
public:
	struct Fields {
		SEL_MenuHandler m_selectCallback = nullptr;
	};

	void selected() {
		if(m_fields->m_selectCallback){
			(m_pListener->*(m_fields->m_selectCallback))(this);
		}
		CCMenuItemSpriteExtra::selected();
	}
};