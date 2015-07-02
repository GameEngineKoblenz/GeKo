#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"
#include "GeKo_Gameplay/Object/AI.h"
#include "GeKo_Gameplay/Object/ObjectType.h"

/**A subclass of Goal which can be used for kill quests!*/
class Goal_Kill : public Goal
{
public: 
	///Should have an id
	Goal_Kill(int id);
	Goal_Kill();
	~Goal_Kill();

	///Increases the kills which was done by the player
	void setActualCount(int actualC);
	///The number of kills which has to be done by the player will be set
	void setGoalCount(int goalC);
	///Sets the type of object which has to be killed
	void setAiTypeId(ObjectType id);
	ObjectType getAiTypeId();

	///Increases the counter
	void increase();

	protected:
		int m_actualCount;
		int m_goalCount;
		ObjectType m_AiTypeId;
};