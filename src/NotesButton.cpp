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

static void addButton(CCNode* layer, GJGameLevel* level, float yOffset) {
    auto size = CCDirector::sharedDirector()->getVisibleSize();

    auto spr = ButtonSprite::create("notes");
    spr->setScale(0.4f);

    auto handler = NotesHandler::create(level);
    layer->addChild(handler);

    auto btn = CCMenuItemSpriteExtra::create(
        spr,
        handler,
        menu_selector(NotesHandler::open)
    );

    btn->setAnchorPoint({1, 0});
    btn->setPosition({size.width - 10, yOffset});

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->addChild(btn);
    layer->addChild(menu, 9999);
}

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        addButton(this, level, 12.f);
        return true;
    }
};

class $modify(EditLevelLayer) {
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;
        addButton(this, level, 6.f);
        return true;
    }
};
