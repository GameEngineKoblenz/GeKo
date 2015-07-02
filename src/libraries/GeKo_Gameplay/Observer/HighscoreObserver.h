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
			m_level->getHighscore()->addScore(5);
			break;
		}
			
	}

	void onNotify(FightSystem& fight, Quest_Event event)
	{
		switch (event)
		{
		case Quest_Event::GOAL_FINISHED:

			//TODO: Wenn Goal beendet, addScore()
			m_level->getHighscore()->addScore(5);
			break;
		}
	
	}

	void onNotify(Player& player, Object_Event event)
	{
		switch (event)
		{
		case Object_Event::PLAYER_SET_ON_FIRE:
			m_level->getHighscore()->addScore(2);
			break;

		}
	}

protected: 
	Level* m_level;
};