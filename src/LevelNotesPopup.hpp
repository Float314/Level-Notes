#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>

using namespace geode::prelude;

class LevelNotesPopup : public Popup {
protected:
    GJGameLevel* m_level = nullptr;
    TextInput* m_input = nullptr;

    std::string getLevelKey() const;
    std::string noteKey() const;
    std::string dateKey() const;
    static std::string getDate();

    bool init(GJGameLevel* level);

public:
    static LevelNotesPopup* create(GJGameLevel* level);
    void onClear(CCObject*);
};
