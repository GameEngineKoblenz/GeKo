#include "GeKo_Gameplay/Questsystem/Goal_Travel.h"

Goal_Travel::Goal_Travel(int id){
	m_id = id;
	m_goalType = GoalType::TRAVEL;
	m_finished = false;
}

Goal_Travel::Goal_Travel(){

}

Goal_Travel::~Goal_Travel(){}

void Goal_Travel::setPosition(glm::vec3 pos){
	m_position = pos;
}

void Goal_Travel::completed(){
	if (!m_finished){
		m_finished = true;
		notify(*this, Quest_Event::GOAL_FINISHED);
	}
}