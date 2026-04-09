#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "LevelNotesPopup.hpp"

using namespace geode::prelude;

class NotesHandler : public CCNode {
protected:
    GJGameLevel* m_level;

public:
    static NotesHandler* create(GJGameLevel* level) {
        auto ret = new NotesHandler();
        ret->m_level = level;
        ret->autorelease();
        return ret;
    }

    void open(CCObject*) {
        LevelNotesPopup::create(m_level)->show();
    }
};

static void addButton(cocos2d::CCMenu* menu, GJGameLevel* level) {
    if (!menu) return;

    auto spr = ButtonSprite::create("notes");
    spr->setScale(0.4f);

    auto handler = NotesHandler::create(level);
    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        handler,
        menu_selector(NotesHandler::open)
    );

    btn->setID("level-notes-button"_spr);
    menu->addChild(btn);
    menu->updateLayout();
}

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        addButton(this->m_playBtnMenu, level);
        return true;
    }
};

class $modify(EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;
        addButton(this->m_buttonMenu, level);
        return true;
    }
};
