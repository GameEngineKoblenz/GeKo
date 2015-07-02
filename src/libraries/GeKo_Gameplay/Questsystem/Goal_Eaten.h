#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"
#include "GeKo_Gameplay/Object/ObjectType.h"

/**A subclass of Goal which can be used for eat quests!*/
class Goal_Eaten : public Goal
{
public: 
	///Should have an id
	Goal_Eaten(int id);
	Goal_Eaten();
	~Goal_Eaten();

	///Increases the food which was eaten by the player
	void setActualCount(int actualC);
	///The number of food which has to be eaten by the player will be set
	void setGoalCount(int goalC);

	///Sets the type of object which has to be eaten
	void setAiTypeId(ObjectType id);
	ObjectType getAiTypeId();
	///Increases the counter
	void increase();

	protected:
		int m_actualCount;
		int m_goalCount;
		ObjectType m_AiTypeId;
};