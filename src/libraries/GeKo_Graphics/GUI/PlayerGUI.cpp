#include "PlayerGUI.h"

PlayerGUI::PlayerGUI(const int hudWidth, const int hudHeight, const int windowWidth, const int windowHeight, const int questHeight, const int questWidth, Player* player, QuestHandler* qH, Highscore* score)
{
	m_HUD_WIDTH = hudWidth;
	m_HUD_HEIGHT = hudHeight;
	m_WINDOW_HEIGHT = windowHeight;
	m_WINDOW_WIDTH = windowWidth;
	m_QUEST_WIDTH = questWidth;
	m_QUEST_HEIGHT = questHeight;
	m_highscore = 0;
	m_timeOver = false;

	Texture bricks((char*)RESOURCES_PATH "/Texture/bricks.bmp");
	m_hud = new GUI("testGUI", m_HUD_WIDTH, m_HUD_HEIGHT);
	m_hud->setPosition(250, 500);
	m_hud->setCollapsable(false);
	m_hud->setTitleBarVisible(false);
	m_hud->setBackgroundAlpha(0.5f);
	m_hud->setResizable(false);
	m_hud->setUseScrollbar(false);
	m_hud->setMoveable(false);

	//m_scoreHUD = new GUI("testScore", 30, 10);
	//m_scoreHUD->setPosition(100, 100);
	//m_scoreHUD->setCollapsable(false);
	//m_scoreHUD->setTitleBarVisible(false);
	//m_scoreHUD->setBackgroundAlpha(0.5f);
	//m_scoreHUD->setResizable(false);
	//m_scoreHUD->setUseScrollbar(false);
	//m_scoreHUD->setMoveable(false);
	//m_scoreHUD->addElement(new GuiElement::Text("Score"));

	//Generate GUI for EXP, LVL, HP
	m_player = player;
	m_questhandler = qH;
	m_highscoreInstance = score;
	hp = m_player->getHealth();
	hpMax = m_player->getHealth();
	exp = m_player->getExp();
	expMax = m_player->getLevelThreshold();

	GuiElement::ProgressBar *hpBar = new GuiElement::ProgressBar(&hp, &hpMax, 300, glm::fvec4(1.0f, 0.0f, 0.0f, 1.0f));
	m_hud->addElement(hpBar);
	m_hud->addElement(new GuiElement::SameLine());
	m_hud->addElement(new GuiElement::Text("HP"));

	GuiElement::ProgressBar *expBar = new GuiElement::ProgressBar(&exp, &expMax, 300, glm::fvec4(1.0f, 0.9960784f, 0.9529411f, 1.0f));
	m_hud->addElement(expBar);
	m_hud->addElement(new GuiElement::SameLine());
	m_hud->addElement(new GuiElement::Text("EXP"));

	m_hud->addElement(new GuiElement::Spacing());
	m_hud->addElement(new GuiElement::Separator());
	m_hud->addElement(new GuiElement::Spacing());
	m_hud->addElement(new GuiElement::Text("LVL"));

	level = m_player->getLvl();
	m_hud->addElement(new GuiElement::SameLine());
	GuiElement::IntBox *lvlBox = new GuiElement::IntBox(&level, glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f), glm::fvec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_hud->addElement(lvlBox);
	
	//Generate GUI for buttons to open/close the Inventory and Quests
	m_hud->addElement(new GuiElement::SameLine());
	inventoryButton = new GuiElement::PushButton("Inventory");
	m_hud->addElement(inventoryButton);
	m_hud->addElement(new GuiElement::SameLine());
	questButton = new GuiElement::PushButton("Quests");
	m_hud->addElement(questButton);
	m_hud->addElement(new GuiElement::SameLine());

	m_hud->addElement(new GuiElement::Text("   Score"));
	m_hud->addElement(new GuiElement::SameLine());
	GuiElement::IntBox *scoreBox = new GuiElement::IntBox(&m_highscore, glm::fvec4(1.0f, 1.0f, 1.0f, 1.0f), glm::fvec4(0.7f, 0.7f, 0.7f, 1.0f));
	m_hud->addElement(scoreBox);

	m_questWindow = new GuiElement::NestedWindow();
	m_questWindow->setPosition(560, 300);
	m_questWindow->hide();
	
	//Generate GUI output for active an finished quests
	actQuests = new GuiElement::Header("Active quests");
	activeQuests = m_questhandler->getActiveAndNotFinished();
	for (int i = 0; i < activeQuests.size(); i++){
		actQuests->addElement(new GuiElement::Text(activeQuests.at(i)->getDescription()));
	}

	finQuests = new GuiElement::Header("Finished quests");
	finishedQuests = m_questhandler->getFinished();
	if (finishedQuests.size() == 0){
		finQuests->addElement(new GuiElement::Text("No finished quests available."));
	}
	else{
		for (int i = 0; i < finishedQuests.size(); i++){
			finQuests->addElement(new GuiElement::Text(finishedQuests.at(i)->getDescription()));
		}
	}
	
	m_questWindow->addElement(actQuests);
	m_questWindow->addElement(finQuests);
	m_questWindow->setName("Quests");
	m_questWindow->setCollapsable(false);
	//m_questWindow->setPosition(m_WINDOW_WIDTH - m_QUEST_WIDTH, (m_WINDOW_HEIGHT / 2) - (m_QUEST_HEIGHT / 2));
	m_questWindow->setSize(m_QUEST_WIDTH, m_QUEST_HEIGHT);
	m_hud->addNestedWindow(m_questWindow);

	//Generate GUI for Inventory
	m_inventoryWindow = new GuiElement::NestedWindow();
	m_inventoryWindow->hide();
	
	m_inventoryWindow->setName("Inventory");
	m_inventoryWindow->setCollapsable(false);
	m_inventoryWindow->setResizable(false);
	m_inventoryWindow->setPosition(560, 0);
	//m_inventoryWindow->setPosition(m_WINDOW_WIDTH - m_QUEST_WIDTH, (m_WINDOW_HEIGHT / 2) - (m_QUEST_HEIGHT / 2));
	m_inventoryWindow->setSize(m_QUEST_WIDTH, m_QUEST_HEIGHT);
	m_hud->addNestedWindow(m_inventoryWindow);

	m_inventoryItems = new std::map<std::string, Texture*>();
	m_inventory = new GuiElement::Inventory(m_inventoryItems, 6);
	m_inventoryWindow->addElement(m_inventory);

	//Generate GUI for win or loose game
	m_endGameWindow = new GuiElement::NestedWindow();
	m_endGameWindow->hide();

	m_endGameWindow->setName("EndGame");
	m_endGameWindow->setCollapsable(false);
	m_endGameWindow->setResizable(false);
	m_endGameWindow->setPosition(25,25);
	m_endGameWindow->setSize(m_WINDOW_HEIGHT - 50, m_WINDOW_WIDTH - 50);
	m_hud->addNestedWindow(m_endGameWindow);

	m_endGameWindow->addElement(new GuiElement::Text("Already running."));
}

PlayerGUI::~PlayerGUI()
{

}

GUI* PlayerGUI::getHUD()
{
	return m_hud;
}


void PlayerGUI::update()
{
	hp = m_player->getHealth();
	hpMax = m_player->getHealthMax();
	exp = m_player->getExp();
	expMax = m_player->getLevelThreshold();
	level = m_player->getLvl();
	m_highscore = m_highscoreInstance->getScore();

	activeQuests = m_questhandler->getActiveAndNotFinished();
	actQuests->clearElements();
	for (int i = 0; i < activeQuests.size(); i++){
		actQuests->addElement(new GuiElement::Text(activeQuests.at(i)->getDescription()));
	}

	finishedQuests = m_questhandler->getFinished();
	finQuests->clearElements();
	if (finishedQuests.size() == 0){
		finQuests->addElement(new GuiElement::Text("No finished quests available."));
	}
	else{
		for (int i = 0; i < finishedQuests.size(); i++){
			finQuests->addElement(new GuiElement::Text(finishedQuests.at(i)->getDescription()));
		}
	}

	//if (finishedQuests.size() == m_questhandler->getQuests().size()){
	//	m_endGameWindow->clearElements();
	//	m_endGameWindow->show();
	//	m_endGameWindow->addElement(new GuiElement::Text("YOU WON! :-D"));
	//	//TODO: Notify Sound
	//}

	if (m_timeOver){
		m_endGameWindow->clearElements();
		m_endGameWindow->show();
		/*m_level->getPlayerGUI()->setTexture((char*)RESOURCES_PATH "/Texture/Cookie_02.png");
		m_level->getPlayerGUI()->getInventory()->insert(std::pair<std::string, Texture*>(std::string("Cookie"), m_level->getPlayerGUI()->getTextures()->back()));*/
		m_endGameWindow->addElement(new GuiElement::Text("Your Time is up. \n Congratulations, your Score is" + m_highscore));
		//TODO: Notify Sound
	}


	if (m_player->getHealth() <= 0){
		m_endGameWindow->clearElements();
		m_endGameWindow->show();
		m_endGameWindow->addElement(new GuiElement::Text("GAME OVER! YOU LOST! :-("));
		//TODO Notify Sound
	}

	if (inventoryButton->isPushed())
	{
		m_inventoryWindow->toggleVisibility();
	}

	if (questButton->isPushed())
	{
		m_questWindow->toggleVisibility();
	}
}

std::map<std::string, Texture*>* PlayerGUI::getInventory(){
	return m_inventoryItems;
}

void PlayerGUI::setTexture(char* fileName){
	Texture* tex = new Texture(fileName);
	m_textures.push_back(tex);
}

std::vector<Texture*>* PlayerGUI::getTextures(){
	return &m_textures;
}

void PlayerGUI::setTimeOver()
{
	m_timeOver = true;
}