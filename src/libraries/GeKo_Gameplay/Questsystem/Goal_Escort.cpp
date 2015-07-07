#include "GeKo_Gameplay/Questsystem/Goal_Escort.h"

Goal_Escort::Goal_Escort(int id){
	m_id = id;
	m_AiId = NULL;
	m_goalType = GoalType::ESCORT;
	m_finished = false;
}

Goal_Escort::Goal_Escort(){

}

Goal_Escort::~Goal_Escort(){}

void Goal_Escort::setAiId(int id){
	m_AiId = id;
}

void Goal_Escort::setPosition(glm::vec3 pos){
	m_position = pos;
}

void Goal_Escort::completed(){
	if (!m_finished){
		m_finished = true;
		notify(*this, Quest_Event::GOAL_FINISHED);
	}
}