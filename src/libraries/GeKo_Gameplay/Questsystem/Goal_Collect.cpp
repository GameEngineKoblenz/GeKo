#include "GeKo_Gameplay/Questsystem/Goal_Collect.h"

Goal_Collect::Goal_Collect(int id){
	m_id = id;
	m_actualCount = 0;
	m_goalCount = 0;
	m_itemType = ItemType::DEFAULTITEM;
	m_goalType = GoalType::COLLECT;
	m_finished = false;
}

Goal_Collect::Goal_Collect(){

}

Goal_Collect::~Goal_Collect(){}

void Goal_Collect::setActualCount(int actualC){
	m_actualCount = actualC;
}

void Goal_Collect::setGoalCount(int goalC){
	m_goalCount = goalC;
}

ItemType Goal_Collect::getItemType()
{
	return m_itemType;
}

void Goal_Collect::setItemType(ItemType iType){
	m_itemType = iType;
}

void Goal_Collect::increase(){
	if (!m_finished){
		m_actualCount++;
		if (m_actualCount >= m_goalCount){
			m_finished = true;
			notify(*this, Quest_Event::GOAL_FINISHED);
		}
	}
}