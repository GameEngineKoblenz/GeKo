#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"


/**A subclass of Goal which can be used for collect quests!*/
class Goal_Action : public Goal
{
public: 
	///Should have an id
	Goal_Action(int id);
	Goal_Action(int id, GoalType gT);
	Goal_Action();
	~Goal_Action();

	///Increases the collection which was collected by the player
	void setActualCount(int actualC);
	///The number of collected items which has to be collected by the player
	void setGoalCount(int goalC);

	///Increases the counter
	void increase();

	protected:
		int m_actualCount;
		int m_goalCount;
};