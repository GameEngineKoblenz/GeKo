#include "GeKo_Gameplay/Questsystem/Goal_Eaten.h"

Goal_Eaten::Goal_Eaten(int id){
	m_id = id;
	m_actualCount = 0;
	m_goalCount = 0;
	m_AiTypeId = ObjectType::DEFAULTOBJECT;
	m_goalType = GoalType::EATEN;
	m_finished = false;
}

Goal_Eaten::Goal_Eaten(){

}

Goal_Eaten::~Goal_Eaten(){}

void Goal_Eaten::setActualCount(int actualC){
	m_actualCount = actualC;
}
void Goal_Eaten::setGoalCount(int goalC){
	m_goalCount = goalC;
}

void Goal_Eaten::setAiTypeId(ObjectType id){
	m_AiTypeId = id;
}

ObjectType Goal_Eaten::getAiTypeId(){
	return m_AiTypeId;
}

void Goal_Eaten::increase(){
	if (!m_finished){
		m_actualCount++;
		if (m_actualCount >= m_goalCount){
			m_finished = true;
			notify(*this, Quest_Event::GOAL_FINISHED);
		}
	}
}