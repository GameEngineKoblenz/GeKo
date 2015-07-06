#include "GeKo_Gameplay/Questsystem/Goal_Discover.h"

Goal_Discover::Goal_Discover(int id){
	m_id = id;
	m_goalType = GoalType::DISCOVER;
	m_finished = false;

	m_actualCount = 0;
	m_goalCount = 0;
}

Goal_Discover::Goal_Discover(){

}

Goal_Discover::~Goal_Discover(){}

void Goal_Discover::setPositions(std::vector<glm::vec3> pos){
	m_positions = pos;
}

void Goal_Discover::addPosition(glm::vec3 pos){
	m_positions.push_back(pos);
}

void Goal_Discover::deletePosition(glm::vec3 pos){
	for (int i = 0; i < m_positions.size(); i++){
		if (m_positions.at(i) == pos){
			m_positions.pop_back();
			increase();
		}
	}
}

void Goal_Discover::setActualCount(int actualC){
	m_actualCount = actualC;
}

void Goal_Discover::setGoalCount(int goalC){
	m_goalCount = goalC;
	if (m_goalCount > m_positions.size()){
		m_goalCount = m_positions.size();
	}
}

void Goal_Discover::increase(){
	m_actualCount++;
	//Attention: Hier wird nur der Counter verändert, aber nicht die Position aus dem Vektor m_positions gelöscht!
	if (m_actualCount >= m_goalCount){
		m_finished = true;
		notify(*this, Quest_Event::GOAL_FINISHED);
	}
}