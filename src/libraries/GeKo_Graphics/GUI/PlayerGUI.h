#pragma once

#include "GUI.h"
#include <GeKo_Gameplay/Object/Player.h>
#include <GeKo_Gameplay/Questsystem/QuestHandler.h>
#include "GeKo_Gameplay/Questsystem/Highscore.h"


/** A class which provides a GUI for the player. It will provide all informations like exp, level and health.*/
class PlayerGUI{
public:
	PlayerGUI(const int hudWidth, const int hudHeight, const int windowHeight, const int windowWidth, const int questHeight, const int questWidth, Player* player, QuestHandler* qH, Highscore* score);
	~PlayerGUI();

	GUI* getHUD();

	std::vector<Texture*>* getTextures();
	void setTexture(char* fileName);
	std::map<std::string, Texture*>* getInventory();

	///Updates all the information in the GUI
	void update();
	void receiveFramesLeft(int framesLeft);


	void youWon();
	void youLost();

	void setTimeOver();

protected:
	GUI* m_hud;
	int m_HUD_WIDTH, m_HUD_HEIGHT, m_WINDOW_WIDTH, m_WINDOW_HEIGHT, m_QUEST_WIDTH, m_QUEST_HEIGHT;

	GuiElement::NestedWindow* m_questWindow, *m_inventoryWindow;
	GuiElement::NestedWindow* m_endGameWindow;

	int hp, hpMax, exp, expMax, level;
	Player* m_player;
	Highscore* m_highscoreInstance;

	std::vector<Quest*> activeQuests, finishedQuests;
	QuestHandler* m_questhandler;

	GuiElement::PushButton *inventoryButton, *questButton;

	GuiElement::Header *actQuests, *finQuests;

	std::map<std::string, Texture*> *m_inventoryItems;
	GuiElement::Inventory *m_inventory;
	std::vector<Texture*> m_textures;
	int m_highscore;
	int m_framesLeft;

	bool m_timeOver;
};