#include "LevelNotesPopup.hpp"

std::string LevelNotesPopup::getEditorKey() const {
    if (!m_level) return "editor-unknown";

    if (m_level->m_levelString.empty())
        return "editor-fallback";

    std::hash<std::string> hasher;
    size_t hash = hasher(m_level->m_levelString);

    return "editor-" + geode::utils::numToString(hash);
}

std::string LevelNotesPopup::noteKey() const {
    if (m_level && m_level->m_levelID.value() > 0)
        return "note-" + geode::utils::numToString(m_level->m_levelID.value());
    return "note-" + getEditorKey();
}

std::string LevelNotesPopup::dateKey() const {
    if (m_level && m_level->m_levelID.value() > 0)
        return "date-" + geode::utils::numToString(m_level->m_levelID.value());
    return "date-" + getEditorKey();
}

std::string LevelNotesPopup::getDate() {
    std::time_t now = std::time(nullptr);
    char buf[32];
    std::tm tm{};

#if defined(_WIN32)
    localtime_s(&tm, &now);
#else
    localtime_r(&now, &tm);
#endif

    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &tm);
    return buf;
}

bool LevelNotesPopup::init(GJGameLevel* level) {
    m_level = level;
    if (!Popup::init(260.f, 150.f))
        return false;

    this->setTitle("level notes");

    auto size = m_mainLayer->getContentSize();

    std::string created = Mod::get()->getSavedValue<std::string>(dateKey(), "");
    if (created.empty()) {
        created = getDate();
        Mod::get()->setSavedValue<std::string>(dateKey(), created);
    }

    auto dateLabel = CCLabelBMFont::create(("added: " + created).c_str(), "bigFont.fnt");
    dateLabel->setScale(0.3f);
    dateLabel->setAnchorPoint({1, 0});
    dateLabel->setPosition({size.width - 10, 10});
    m_mainLayer->addChild(dateLabel);

    m_input = TextInput::create(size.width - 20, "type anything...", "chatFont.fnt");
    m_input->setCommonFilter(CommonFilter::Any);
    m_input->setPosition({size.width / 2, size.height / 2});
    m_input->setMaxCharCount(200);

    std::string saved = Mod::get()->getSavedValue<std::string>(noteKey(), "");
    m_input->setString(saved, false);

    m_input->setCallback([this, created](std::string const& text) {
        Mod::get()->setSavedValue<std::string>(noteKey(), text);
        Mod::get()->setSavedValue<std::string>(dateKey(), created);
        log::info("saved note ({} chars)", text.size());
    });

    m_mainLayer->addChild(m_input);

    auto clearSpr = ButtonSprite::create("clear");
    clearSpr->setScale(0.5f);

    auto clearBtn = CCMenuItemSpriteExtra::create(
        clearSpr,
        this,
        menu_selector(LevelNotesPopup::onClear)
    );
    clearBtn->setPosition({size.width / 2, 25});

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});
    menu->addChild(clearBtn);
    m_mainLayer->addChild(menu);

    return true;
}

LevelNotesPopup* LevelNotesPopup::create(GJGameLevel* level) {
    auto ret = new LevelNotesPopup();
    if (ret && ret->init(level)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void LevelNotesPopup::onClear(CCObject*) {
    if (m_input) m_input->setString("", true);
    Mod::get()->setSavedValue<std::string>(noteKey(), "");
    log::warn("note cleared");
}
