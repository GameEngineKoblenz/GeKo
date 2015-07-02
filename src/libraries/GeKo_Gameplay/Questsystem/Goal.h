#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "GoalType.h"
#include "GeKo_Gameplay/Item/ItemType.h"
#include "GeKo_Gameplay/Object/ObjectType.h"
#include "GeKo_Gameplay/Observer/Subject.h"

/**A Goal should have a condition which determines when a goal is finished. This condition could be a destination on the map,
an item to collect or a object to kill or eat.  */
class Goal : public Subject<Goal, Quest_Event>
{
public: 
	///Should have an id
	Goal(int id);
	Goal();
	~Goal();

	///Returns the id
	int getId();

	///Returns the specific type of the goal
	/**There are collect, eaten, escort, kill and travel goals! */
	GoalType getGoalType();

	///Returns true if a goal is finished
	bool getFinished();


	 virtual void setActualCount(int actualC);

	 virtual void setGoalCount(int goaldC);

	virtual ItemType getItemType();
	virtual void setItemType(ItemType iType);
	virtual void increase();

	virtual void setAiTypeId(ObjectType id);
	virtual ObjectType getAiTypeId();

	virtual void setAiID(int id);
	virtual void setPosition(glm::vec3 m_position);



	protected:
		int m_id;
		bool m_finished;
		GoalType m_goalType;

};
