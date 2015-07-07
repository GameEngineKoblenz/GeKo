#include "GeKo_Gameplay/Questsystem/Quest.h"

Quest::Quest(int id){
	m_id = id;
	m_active = false;
	m_finished = false;
	Goal* default_goal = new Goal();
	m_goals.push_back(default_goal);
	m_text = "";

	m_rewards.push_back(new Reward());
}

Quest::Quest(){

}

Quest::~Quest(){}

std::vector<Goal*> Quest::getGoals(){
	return m_goals;
}

void Quest::addGoal(Goal* g){
	if (m_goals.at(0)->getId() == -1){
		m_goals.pop_back();
		m_goals.push_back(g);
	}
	else {
		m_goals.push_back(g);
	}
}

std::vector<Reward*> Quest::getRewards()
{
	return m_rewards;
}

void Quest::addReward(Reward* r)
{
	m_rewards.push_back(r);
}

bool Quest::getActive(){
	return m_active;
}

void Quest::setActive(bool activity){
	m_active = activity;
}

void Quest::isFinished(){
	if (!m_finished){
		m_finished = true;
		for (int i = 0; i < m_goals.size(); i++){
			if (!m_goals.at(i)->getFinished()){
				m_finished = false;
			}
		}
		if (m_finished){
			m_active = false;
			notify(*this, Quest_Event::QUEST_FINISHED);
		}
	}
}

bool Quest::getFinished(){ 
	return m_finished; 
}

int Quest::getID(){
	return m_id;
}

std::string Quest::getDescription(){
	return m_text;
}

void Quest::setDescription(std::string tex){
	m_text = tex;
}