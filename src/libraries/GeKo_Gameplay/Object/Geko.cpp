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
	m_strength = 100;

	m_inventory = new Inventory();

	m_states.clear();
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

void Geko::moveForward(){
	if (getStates(States::HEALTH)){
		m_position.x += m_speed* m_deltaTime *m_viewDirection.x;
		m_position.y += m_speed* m_deltaTime *m_viewDirection.y;
		m_position.z += m_speed* m_deltaTime *m_viewDirection.z;

		//std::cout << "Player moveFwd to: x:" << m_position.x << "; z: " << m_position.z << std::endl;

		notify(*this, Object_Event::OBJECT_MOVED);
		if (m_rainDanceActive)
		{
			if (!m_step1)
			{
				m_step1 = true;
			}
			else if (m_step2 && !m_step3)
			{
				m_step3 = true;
			}
		}
	}
}

void Geko::moveBackward(){
	if (getStates(States::HEALTH)){
		m_position.x -= m_speed* m_deltaTime *m_viewDirection.x;
		m_position.y -= m_speed* m_deltaTime *m_viewDirection.y;
		m_position.z -= m_speed* m_deltaTime *m_viewDirection.z;

		//std::cout << "Player moveBwd" << std::endl;

		notify(*this, Object_Event::OBJECT_MOVED);
		if (m_rainDanceActive)
		{
			if (m_step1 && !m_step2)
			{
				m_step2 = true;
			}
			else if (m_step3 && !m_step4)
			{
				m_step4 = true;
			}
		}
	}
}

void Geko::moveLeft(){
	if (getStates(States::HEALTH)){
		glm::vec3 directionOrtho = glm::cross(glm::vec3(m_viewDirection), glm::vec3(0, 1, 0));
		m_position.x -= m_speed* m_deltaTime*directionOrtho.x;
		m_position.y -= m_speed* m_deltaTime*directionOrtho.y;
		m_position.z -= m_speed* m_deltaTime*directionOrtho.z;

		//std::cout << "Player moveLeft" << std::endl;

		notify(*this, Object_Event::OBJECT_MOVED);
	}
}

void Geko::moveRight(){
	if (getStates(States::HEALTH)){
		glm::vec3 directionOrtho = glm::cross(glm::vec3(m_viewDirection), glm::vec3(0, 1, 0));
		m_position.x += m_speed* m_deltaTime*directionOrtho.x;
		m_position.y += m_speed* m_deltaTime*directionOrtho.y;
		m_position.z += m_speed* m_deltaTime*directionOrtho.z;

		//std::cout << "Player moveRight" << std::endl;

		notify(*this, Object_Event::OBJECT_MOVED);
		if (m_rainDanceActive)
		{
			if (m_step4 && !m_step5)
			{
				m_step5 = true;
				notify(*this, Object_Event::RAINDANCE);
			}
			
		}
	}
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

void Geko::setDancing(bool dancing)
{
	m_rainDanceActive = dancing;
}

bool Geko::isDancing()
{
	return m_rainDanceActive;
}