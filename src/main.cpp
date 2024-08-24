#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <cctype>

#include "CommentCell.h"
#include "YouTuberLayer.hpp"

extern bool ytdownload;
extern bool twitchdownload;

using namespace geode::prelude;

class $modify(YTListLayer, MenuLayer) {

	struct Fields {
		CCSprite* ytBtnSprite;
		CCMenuItemSpriteExtra* ytBtn;
		YouTuberLayer* yt;
	};

	bool init() {
		bool result = MenuLayer::init();
		if (!ytdownload) downloadYT();
		if (!twitchdownload) downloadTwitch();
		auto rightMenu = this->getChildByID("right-side-menu");

		m_fields->ytBtnSprite = CCSprite::create("yt-btn.png"_spr);
		m_fields->ytBtnSprite->setScale(0.35);
		m_fields->ytBtn = CCMenuItemSpriteExtra::create(m_fields->ytBtnSprite, this, menu_selector(YTListLayer::openList));

		#if !defined(GEODE_IS_ANDROID)
			rightMenu->addChild(m_fields->ytBtn, -1);
			rightMenu->updateLayout(false);
		#endif

		return result;
	}

	void openList(CCObject*) {
		auto screenSize = CCDirector::sharedDirector()->getWinSize();
		YouTuberLayer::create()->show();
	}
};

class $modify(YouTuberAlert, ProfilePage) {

	struct Fields {
		CCSprite * badge;
		CCMenuItemSpriteExtra* icon;
		bool is_mod;
		std::string player_name;
	};

	void setupPageInfo(gd::string name, char const* chars) {
		ProfilePage::setupPageInfo(name,chars);

		auto director = CCDirector::sharedDirector();
		auto size = director->getWinSize();
		auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		auto usernameMenu = mainLayer->getChildByID("username-menu");

		downloadYT();
		downloadTwitch();

		m_fields->player_name = m_usernameLabel->getString();
		addBadge("yt");
		addBadge("twitch");

		usernameMenu->updateLayout();
	}

	void addBadge(std::string badge) {
		if (badge == "yt") {
			for (const auto& names : YouTubers) {
				std::string lower_names(names.begin(), names.end());
				std::string lower_player_name(m_fields->player_name.begin(), m_fields->player_name.end());
				for (char &c : lower_names) {
					c = std::tolower(c);
				}
				for (char &c : m_fields->player_name) {
					c = std::tolower(c);
				}
				lower_player_name.erase(remove_if(lower_player_name.begin(), lower_player_name.end(), isspace), lower_player_name.end());
				if (lower_names == lower_player_name) {
					auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
					auto usernameMenu = mainLayer->getChildByID("username-menu");

					m_fields->badge = CCSprite::create("youtuber.png"_spr);
					m_fields->badge->setScale(0.95);
					
					m_fields->icon = CCMenuItemSpriteExtra::create(m_fields->badge, this, menu_selector(YouTuberAlert::found_youtube));
					m_fields->icon->setID("xanii.youtubers/YouTube-Badge");

					if (!static_cast<CCMenu*>(usernameMenu->getChildByID("xanii.youtubers/YouTube-Badge"))) {
						usernameMenu->addChild(m_fields->icon);
						usernameMenu->updateLayout();
						m_fields->icon->setPositionY(m_fields->icon->getPositionY() - 1);
					}
				}
			}
		}
		if (badge == "twitch") {
			for (const auto& names : Streamers) {
				std::string lower_names(names.begin(), names.end());
				std::string lower_player_name(m_fields->player_name.begin(), m_fields->player_name.end());
				for (char &c : lower_names) {
					c = std::tolower(c);
				}
				for (char &c : m_fields->player_name) {
					c = std::tolower(c);
				}
				lower_player_name.erase(remove_if(lower_player_name.begin(), lower_player_name.end(), isspace), lower_player_name.end());
				if (lower_names == lower_player_name) {
					auto mainLayer = static_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
					auto usernameMenu = mainLayer->getChildByID("username-menu");

					m_fields->badge = CCSprite::create("twitch.png"_spr);
					m_fields->badge->setScale(0.95);
					
					m_fields->icon = CCMenuItemSpriteExtra::create(m_fields->badge, this, menu_selector(YouTuberAlert::found_streamer));
					m_fields->icon->setID("xanii.youtubers/Twitch-Badge");

					if (!static_cast<CCMenu*>(usernameMenu->getChildByID("xanii.youtubers/Twitch-Badge"))) {
						usernameMenu->addChild(m_fields->icon);
						usernameMenu->updateLayout();
						m_fields->icon->setPositionY(m_fields->icon->getPositionY() - 1);
					}
				}
			}
		}
	}

	void found_youtube(CCObject*) {
		FLAlertLayer::create("Youtuber Found!","This user is a <cr>prominent member</c> of the <cy>Geometry Dash</c> Community!","OK")->show();
		if (m_fields->is_mod) m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
		else m_fields->icon->setScale(m_usernameLabel->getScale() + 0.1f);
	}

	void found_streamer(CCObject*) {
		FLAlertLayer::create("Streamer Found!","This user is a <cr>prominent member</c> of the <cy>Geometry Dash</c> Community!","OK")->show();
		if (m_fields->is_mod) m_fields->icon->setScale(m_usernameLabel->getScale() + 0.05f);
		else m_fields->icon->setScale(m_usernameLabel->getScale() + 0.1f);
	}
};