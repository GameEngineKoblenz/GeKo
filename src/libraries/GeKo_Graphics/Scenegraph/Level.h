#pragma once

#include <GeKo_Graphics/Scenegraph/Scene.h>
#include <GeKo_Gameplay/Questsystem/QuestHandler.h>
#include <GeKo_Gameplay/FightSystem/FightSystem.h>
#include <iostream>
#include <GeKo_Graphics/Geometry/Terrain.h>
#include <GeKo_Graphics/GUI/PlayerGUI.h>
#include <GeKo_Gameplay/Questsystem/Highscore.h>

///A Level is the necessary unit to create a game
/**
The class "Level" has a unique name and can be identified with this name. 
A level manages his scenes, can change them, add and remove them. It provides a Quest-Handler and a Fight-System Object.
Furthermore a level can have a terrain and one or more guis. 
*/
class Level
{
public:
    ///The constructor for the level
	/**The level needs a name, but does not consist of any scenes at the beginning*/
	Level(const char* levelName);
	~Level();
	
	///A getter Method which returns a const char
	/**A getter for the name of the level*/
	const char* getLevelName();


	///This method adds a Scene to the level
	/**/
	void addScene(Scene* scene);

	///This method deletes a specific Scene by its name
	/**/
	void removeScene(std::string sceneName);

	///Returns a Scene object
	/**Needs a Scenename to find a Scene. Returns NULL if the Scene does not exist*/
	Scene* getScene(std::string sceneName);

	///Changes the active Scene to the given scene
	/**/
	void changeScene(std::string sceneName);

	///Returns the m_activeScene
	/**/
	Scene* getActiveScene();

	///Return the m_questHandler
	/**The Quest-Handler is managing all the Quests of one level and is unique for every level*/
	QuestHandler* getQuestHandler();
	void setQuestHandler(QuestHandler* qH);

	///Returns the m_fightSystem
	/**The FightSystem is unique for every level and handles all the fights of KI and Player*/
	FightSystem* getFightSystem();

	void setTerrain(Terrain* terrain);
	Terrain* getTerrain();

	///This method checks if a terrain was defined for this level
	bool hasTerrain();

	///This methods adds a GUI for the level
	/**The GUI will be add to a vector and then it can be rendered*/
	void addGUI(GUI* gui);
	GUI* getGUI(std::string name);

	void setGUI(PlayerGUI* playerGUI);
	PlayerGUI* getPlayerGUI();

	Highscore* getHighscore();

protected:
	const char* m_levelName;
	bool m_hasTerrain;

	std::vector<Scene*> m_sceneSet;
	Scene* m_activeScene;

	QuestHandler* m_questHandler;

	FightSystem* m_fightSystem;

	Terrain* m_terrain;

	std::vector<GUI*> m_guis;

	PlayerGUI* m_playerGui;

	Highscore* m_highscore;
};