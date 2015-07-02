#include "GeKo_Gameplay/Questsystem/Goal_Kill.h"

Goal_Kill::Goal_Kill(int id){
	m_id = id;
	m_actualCount = 0;
	m_goalCount = 0;
	m_AiTypeId = ObjectType::DEFAULTOBJECT;
	m_goalType = GoalType::KILL;
	m_finished = false;
}

Goal_Kill::Goal_Kill(){

}

Goal_Kill::~Goal_Kill(){}

void Goal_Kill::setActualCount(int actualC){
	m_actualCount = actualC;
}
void Goal_Kill::setGoalCount(int goalC){
	m_goalCount = goalC;
}

void Goal_Kill::setAiTypeId(ObjectType id){
	m_AiTypeId = id;
}

ObjectType Goal_Kill::getAiTypeId(){
	return m_AiTypeId;
}

void Goal_Kill::increase(){
	m_actualCount++;
	if (m_actualCount >= m_goalCount){
		m_finished = true;
		notify(*this, Quest_Event::GOAL_FINISHED);
	}
}