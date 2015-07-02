#include "GeKo_Gameplay/Questsystem/Goal.h"

Goal::Goal(int id){
	m_id = id;
	m_finished = false;
	m_goalType = GoalType::GOAL;
}

Goal::Goal(){
	m_id = -1;
}

Goal::~Goal(){}

int Goal::getId(){
	return m_id;
}

GoalType Goal::getGoalType(){
	return m_goalType;
}

bool Goal::getFinished(){
	return m_finished;
}

void Goal::setActualCount(int actualC)
{

}

void Goal::setGoalCount(int goaldC){

}


 ItemType Goal::getItemType(){

	 return ItemType::DEFAULTITEM;
}

void Goal::setItemType(ItemType iType){}
void Goal::increase(){}
void Goal::setAiTypeId(ObjectType id){}
ObjectType Goal::getAiTypeId(){ return ObjectType::DEFAULTOBJECT; }
void Goal::setAiID(int id){}
void Goal::setPosition(glm::vec3 m_position){}
