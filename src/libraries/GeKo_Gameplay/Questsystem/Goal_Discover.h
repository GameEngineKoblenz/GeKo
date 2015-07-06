#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Goal.h"

/**A subclass of Goal which can be used to discovering places!*/
class Goal_Discover : public Goal
{
public: 
	///Should have an id
	Goal_Discover(int id);
	Goal_Discover();
	~Goal_Discover();

	///The positions to which the Player should go will be set
	void setPositions(std::vector<glm::vec3> pos);
	void addPosition(glm::vec3 pos);
	void deletePosition(glm::vec3 pos);

	///Increases the collection which was collected by the player
	void setActualCount(int actualC);
	///The number of collected items which has to be collected by the player
	void setGoalCount(int goalC);

	///Increases the counter
	void increase();


	protected:
		int m_actualCount;
		int m_goalCount;

		std::vector<glm::vec3> m_positions;
};