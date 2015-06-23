#include "GeKo_Gameplay/Object/Geko.h"

Geko::Geko(std::string gekoName){
	Geko(gekoName, glm::vec3(0.0f));
}

Geko::Geko(std::string gekoName, glm::vec3 spawnPoint){

	m_type = ObjectType::PLAYER;
	m_class = ClassType::PLAYER;

	m_spawnPoint = spawnPoint;
	m_position = glm::vec4(m_spawnPoint, 1.0);
	m_name = gekoName;

	m_hunger = 100;
	m_hungerMax = 100;
	m_health = 10000;
	m_healthMax = 10000;
	m_strength = 3;

	m_inventory = new Inventory();

	std::pair<States, bool> s(States::HUNGER, false);
	m_states.push_back(s);
	s.first = States::HEALTH;
	s.second = true;
	m_states.push_back(s);

	m_viewDirection = glm::vec4(0.0, 0.0, -1.0, 0.0);
	m_deltaTime = 0.0;
	m_phi = 0.0;
	m_theta = 2.0;
	m_alpha = 0;

	m_speed = 0.01;
	m_speedTurn = 0.1;

	m_levelThreshold = 100.0;
	m_exp = 0;
	m_level = 0;
}

Geko::Geko(){}

Geko::~Geko(){
}

void Geko::setFire(){
	//TODO: Attention! The input calls this method two times, so that the fire uses two branches!
	if (getStates(States::HEALTH)){
		if (m_inventory->countItem(ItemType::BRANCH) > 0){
			m_inventory->reduceItem(ItemType::BRANCH, 1);
			notify(*this, Object_Event::PLAYER_SET_ON_FIRE);
		}
		else{
			//TODO: GUI sagt, dass das Inventar keine Branches hat
		}
	}
}