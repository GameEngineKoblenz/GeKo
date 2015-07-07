#pragma once
#include <glm/ext.hpp>
#include <GeKo_Gameplay/Observer/Observer.h>
#include <GeKo_Graphics/Scenegraph/Level.h>


/**A Questobserver manages all the quest-changes like increasing a collecting-goal-value or finishing a quest.*/
class HighscoreObserver : public Observer<Player, Object_Event>, public Observer<Quest, Quest_Event>, public Observer<Goal, Quest_Event>, public Observer<FightSystem, Quest_Event>
{
public:
	HighscoreObserver(Level* level){ m_level = level; }

	~HighscoreObserver(){}

	void onNotify(Quest& quest, Quest_Event event)  
	 {
		switch (event){
		case Quest_Event::QUEST_FINISHED:

			//TODO: Wenn Quest beendet, addScore()
			m_level->getHighscore()->addScore(5);
			break;
	
		}
	 }

	void onNotify(Goal& goal, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:
			//TODO: Wenn Goal beendet, addScore()
			m_level->getHighscore()->addScore(3);
			break;
		}
			
	}

	void onNotify(FightSystem& fight, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:

			//TODO: Wenn Goal beendet, addScore()
			m_level->getHighscore()->addScore(3);
			break;
		}
	
	}

	void onNotify(Player& player, Object_Event event)
	{
		glm::vec4 pos;
		glm::vec4 posPlayer;
		bool gotScore = false;
		switch (event)
		{
		case Object_Event::PLAYER_SET_ON_FIRE:
			//TODO: + Punkte, wenn der Spieler am Fireplace ist 
			//Lösung: Gehe durch szenegraph, suche object type fireplace, überprüfe positionen,dann gibt es + punkte
			for (int i = 0; i < m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet()->size(); i++)
			{
				if (m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet()->at(i)->hasObject())
				{
					if (m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet()->at(i)->getType() == ClassType::STATIC)
					{
						if (m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet()->at(i)->getStaticObject()->getObjectType() == ObjectType::FIREPLACE)
						{
							pos = m_level->getActiveScene()->getScenegraph()->getRootNode()->getChildrenSet()->at(i)->getStaticObject()->getPosition();
							posPlayer = player.getPosition();
							if ((pos.x + 4.0f) > player.getPosition().x && (pos.x - 4.0f) < player.getPosition().x)
							{
								if ((pos.z + 4.0f) > player.getPosition().z && (pos.z - 4.0f) < player.getPosition().z)
								{
									m_level->getHighscore()->addScore(2);
									player.setExp(5);
									gotScore = true;

									std::vector<Goal*> goal = m_level->getQuestHandler()->getQuests(GoalType::ACTION_FIRE);
									for (int i = 0; i < goal.size(); i++){
										goal.at(i)->increase();
									}
								}

							}
						}
					}
				}
			}
			if (!gotScore)
			{
				m_level->getHighscore()->addScore(-2);
			}
			break;

		case Object_Event::PLAYER_LVLUP:
			m_level->getHighscore()->addScore(10);
			break;

		case Object_Event::PLAYER_DIED:
			m_level->getHighscore()->addScore(-30);
			break;
		}
	}

protected: 
	Level* m_level;
};