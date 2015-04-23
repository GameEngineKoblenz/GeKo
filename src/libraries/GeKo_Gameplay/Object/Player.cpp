#include "GeKo_Gameplay/Object/Player.h"
#include <stdexcept>

Player::Player(std::string playerName)
{
	Player(playerName, glm::vec3(0.0f));
}

Player::Player(std::string playerName, glm::vec3 spawnPoint)
{
	m_type = ObjectType::PLAYER;
	m_class = ClassType::PLAYER;

	m_spawnPoint = spawnPoint;
	m_position = glm::vec4(m_spawnPoint, 1.0);
	m_name = playerName;

	m_hunger = 100;
	m_hungerMax = 100;
	m_health = 100;
	m_healthMax = 100;
	m_strength = 1;

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
}

Player::Player(){
}

Player::~Player(){
}

glm::vec3 Player::getSpawnPoint()
{
	return m_spawnPoint;
}

void Player::move(glm::vec3 newPosition)
{
	m_position = glm::vec4(newPosition, 1.0);
}

void Player::moveForward(){
	m_position.x += m_speed* m_deltaTime *m_viewDirection.x;
	m_position.y += m_speed* m_deltaTime *m_viewDirection.y;
	m_position.z += m_speed* m_deltaTime *m_viewDirection.z;

	//std::cout << "Player moveFwd" << std::endl;

	notify(*this, Object_Event::OBJECT_MOVED);
}

void Player::moveBackward(){
	m_position.x -= m_speed* m_deltaTime *m_viewDirection.x;
	m_position.y -= m_speed* m_deltaTime *m_viewDirection.y;
	m_position.z -= m_speed* m_deltaTime *m_viewDirection.z;

//	std::cout << "Player moveBwd" << std::endl;

	notify(*this, Object_Event::OBJECT_MOVED);
}

void Player::moveLeft(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_viewDirection), glm::vec3(0, 1, 0));
	m_position.x -= m_speed* m_deltaTime*directionOrtho.x;
	m_position.y -= m_speed* m_deltaTime*directionOrtho.y;
	m_position.z -= m_speed* m_deltaTime*directionOrtho.z;

	//std::cout << "Player moveLeft" << std::endl;

	notify(*this, Object_Event::OBJECT_MOVED);
}

void Player::moveRight(){
	glm::vec3 directionOrtho = glm::cross(glm::vec3(m_viewDirection), glm::vec3(0, 1, 0));
	m_position.x += m_speed* m_deltaTime*directionOrtho.x;
	m_position.y += m_speed* m_deltaTime*directionOrtho.y;
	m_position.z += m_speed* m_deltaTime*directionOrtho.z;

	//std::cout << "Player moveRight" << std::endl;

	notify(*this, Object_Event::OBJECT_MOVED);
}

void Player::turnLeft(){

	m_phi -= m_speedTurn* m_deltaTime;
	if (m_phi < 0) m_phi += 360.0;
	else if (m_phi > 360) m_phi -= 360;

	rotateView(m_phi, m_theta);

	notify(*this, Object_Event::OBJECT_ROTATED);
}

void Player::turnRight(){
	m_phi += m_speedTurn* m_deltaTime;
	if (m_phi < 0) m_phi += 360.0;
	else if (m_phi > 360) m_phi -= 360;

	rotateView(m_phi, m_theta);

	notify(*this, Object_Event::OBJECT_ROTATED);
}

void Player::update(){
	if (m_health == 0){
		std::cout << "Player: Died" << std::endl;
		//TODO OBJECT_DIED, Weil "stopped" den Observervorgang bescheibt , wenn sich das Object nicht mehr bewegt
		notify(*this, Object_Event::OBJECT_STOPPED);
		setStates(States::HEALTH, false);
	}
	if (getStates(States::HEALTH)){ 
		//std::cout << "<<<<<<<< UpdateMethod <<<<<<<<" << std::endl;
		updateStates();
		//std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	}
	notify(*this, Object_Event::OBJECT_STOPPED);
}

void Player::rotateView(float leftRight, float upDown)
{
	m_viewDirection = glm::vec4(0.0, 0.0, -1.0, 0.0);
	m_viewDirection = glm::rotate(m_viewDirection, upDown, glm::cross(glm::vec3(m_viewDirection), glm::vec3(0.0f, -1.0f, 0.0f)));
	m_viewDirection = glm::rotate(m_viewDirection, leftRight, glm::vec3(0.0f, -1.0f, 0.0f));
	m_viewDirection.y = glm::clamp(m_viewDirection.y, -0.5f, 0.5f);
	//notify(*this, Object_Event::OBJECT_ROTATED);
}

float Player::getPhi(){
	return m_phi;
}

float Player::getAlpha(){
	return m_alpha;
}

void Player::updateSourcesInMap()
{
	for (std::map<Soundtype, std::string>::iterator i = m_soundMap.begin(); i != m_soundMap.end(); ++i)
	{
			m_sfh->updateSourcePosition(i->second , glm::vec3(m_position));
	}
}

std::string Player::getSourceName(Soundtype type)
{
	try{
		return m_soundMap.at(type);
	}
	catch (const std::out_of_range& oor)
	{
		std::cout << "No Sound with this type was set!" <<std::endl;
		return "oor";
	}
}

void Player::setSourceName(Soundtype type , std::string sourceName, const char* filepath)
{
		m_sfh->generateSource(sourceName, glm::vec3(m_position), filepath);
		m_soundMap.insert(std::pair<Soundtype, std::string>(type, sourceName));
}

void Player::setDeltaTime(float dt){
	m_deltaTime = dt;
}

void Player::setFire(){
	if (m_inventory->countItem(ItemType::BRANCH)){
		m_inventory->reduceItem(ItemType::BRANCH, 1);
		notify(*this, Object_Event::PLAYER_SET_ON_FIRE);
	} else{
		//TODO: GUI sagt, dass das Inventar keine Branches hat
	}
}