#include <GeKo_Graphics/Scenegraph/Level.h>


Level::Level(const char* levelName)
{
	m_levelName = levelName;
	m_questHandler = new QuestHandler();
	m_fightSystem = new FightSystem();
	m_hasTerrain = false;

}

Level::~Level()
{
}

const char* Level::getLevelName()
{
	return m_levelName;
}

void Level::addScene(Scene* scene)
{
	m_sceneSet.push_back(scene);
}

void Level::removeScene(std::string sceneName)
{
	for (int i = 0; i < m_sceneSet.size(); i++)
	{
		if (m_sceneSet.at(i)->getSceneName() == sceneName)
		{
			m_sceneSet.erase(m_sceneSet.begin() + i);
		}
	}
}

Scene* Level::getScene(std::string sceneName )
{
	for (int i = 0; i < m_sceneSet.size(); i++)
	{
		if (m_sceneSet.at(i)->getSceneName() == sceneName)
		{
			return m_sceneSet.at(i);
		}
	}
	std::cout << "ERROR: The Scene with the name " << sceneName << " does not exist!" << std::endl;
	return NULL;
}

void Level::changeScene(std::string sceneName)
{
	bool success = false;

	for (int i = 0; i < m_sceneSet.size(); i++)
	{
		if (m_sceneSet.at(i)->getSceneName() == sceneName)
		{
			m_activeScene = m_sceneSet.at(i);
			success = true;
		}
	}

	if (success)
	{
		//std::cout << "SUCCESS: The scene was successfully changed!" << std::endl;
	}
	else
	{
		std::cout << "FAILURE: The scene could not be changed, name was not found!" << std::endl;
	}
}

Scene* Level::getActiveScene()
{
	return m_activeScene;
}

QuestHandler* Level::getQuestHandler()
{
	return m_questHandler;
}

FightSystem* Level::getFightSystem()
{
	return m_fightSystem;
}

void Level::setTerrain(Terrain* terrain)
{
	m_terrain = terrain;
	m_hasTerrain = true;
}

Terrain* Level::getTerrain()
{
	return m_terrain;
}

bool Level::hasTerrain()
{
	return m_hasTerrain;
}


void Level::addGUI(GUI* gui)
{
	m_guis.push_back(gui);
}

GUI* Level::getGUI(std::string name)
{
	GUI* tmp;
	for (int i = 0; i < m_guis.size(); i++)
	{
		if (m_guis.at(i)->m_windowName == name)
		{
			tmp = m_guis.at(i);
		}
	}

	return tmp;
}

void Level::setGUI(PlayerGUI* playerGUI)
{
	m_playerGui = playerGUI;
}

PlayerGUI* Level::getPlayerGUI()
{
	return m_playerGui;
}