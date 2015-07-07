#include "GeKo_Gameplay/Questsystem/Goal_Action.h"

Goal_Action::Goal_Action(int id, GoalType gT){
	m_id = id;
	m_actualCount = 0;
	m_goalCount = 0;
	m_goalType = gT;
	m_finished = false;
}

Goal_Action::Goal_Action(){

}

Goal_Action::~Goal_Action(){}

void Goal_Action::setActualCount(int actualC){
	m_actualCount = actualC;
}

void Goal_Action::setGoalCount(int goalC){
	m_goalCount = goalC;
}

void Goal_Action::increase(){
	if (!m_finished){
		m_actualCount++;
		if (m_actualCount >= m_goalCount){
			m_finished = true;
			notify(*this, Quest_Event::GOAL_FINISHED);
		}
	}
}