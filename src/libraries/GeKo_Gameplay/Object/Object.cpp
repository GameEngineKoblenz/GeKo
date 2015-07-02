#include "GeKo_Gameplay/Object/Object.h"

Object::Object()
{
		m_id = 0;
		m_type = ObjectType::DEFAULTOBJECT;
		m_name = "defaultObject";
		m_myNodeName = "NULL";
	
		m_hunger = 10;
		m_hungerMax = 10;
		m_health = 10;
		m_healthMax = 10;
		m_strength = 0.5;
		m_exp = 0.0;
		
	
		m_speed = 0.01;
	
		m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);
		m_viewDirection = glm::vec4(0.0, 0.0, -1.0, 0.0);
	
		m_inventory = new Inventory();
		
		m_class = ClassType::OBJECT;

		std::pair<States, bool> s(States::DEFAULTSTATE, false);
		m_states.push_back(s);
		m_hasSound = false;
}

Object::~Object(){}

void Object::setName(std::string newName){
	m_name = newName;
}

std::string Object::getName(){
	return m_name;
}

void Object::setPosition(glm::vec4 pos){
	m_position = pos;
}

void Object::setPosition(glm::vec3 pos){
	m_position = glm::vec4(pos, 1.0);
}

glm::vec4 Object::getPosition()
{
	return m_position;
}

float Object::getStrength(){
	return m_strength;
}

void Object::setHealth(float newHealth){
	m_health = newHealth;
}

float Object::getHealth(){
	return m_health;
}

float Object::getHealthMax(){
	return m_healthMax;
}

void Object::setHunger(float newHunger){
	m_hunger = newHunger;
}

float Object::getHunger(){
	return m_hunger;
}

float Object::getHungerMax(){
	return m_hungerMax;
}

void Object::setNodeName(std::string nodeName)
{
	m_myNodeName = nodeName;
}

std::string Object::getNodeName()
{
	return m_myNodeName;
}

Inventory* Object::getInventory()
{
	return m_inventory;
}

void Object::setObjectType(ObjectType type)
{
	m_type = type;
}

ObjectType Object::getObjectType()
{
	return m_type;
}

glm::vec4 Object::getViewDirection()
{
	return m_viewDirection;
}

void Object::setViewDirection(glm::vec4 viewDirection)
{
	 m_viewDirection = viewDirection;
}

bool Object::getStates(States state){
	for (int i = 0; i < m_states.size(); i++){
		if (m_states.at(i).first == state) {
			return m_states.at(i).second;
		}
	}
	return NULL;
}

void Object::setStates(States state, bool b){
	for (int i = 0; i < m_states.size(); i++){
		if (m_states.at(i).first == state){
			m_states.at(i).second = b;
		}
	}
}

void Object::update(){
}

void Object::updateStates(){
	if (m_hunger > 0){
		m_hunger -= 0.01;
		if (m_hunger <= m_hungerMax / 2){
			//std::cout << "Object" << m_name << ": Is hungry! Hunger: " << m_hunger << std::endl;
			setStates(States::HUNGER, true);
		}
		else {
			if (m_health < m_healthMax)
				m_health += 10;
		}
	}
	else {
		m_health--;
		//::cout << "Object " << m_name << ": Looses health because it is too hungry!" << std::endl;
	}
	if (m_health == 0){
		std::cout << "Object " << m_name << ": Died" << std::endl;
		setStates(States::HEALTH, false);
		
	}
}

void Object::move(){}

void Object::eat(){
	m_hunger = m_hungerMax;
	//std::cout << "Object" << m_name << ": Has eaten." << std::endl;
	setStates(States::HUNGER, false);
}

void Object::collectItem(ItemType typeId, int count){
	for (int i = 0; i < count; i++){
		m_inventory->addItem(typeId);
	}
}

void Object::setClassType(ClassType type)
{
	m_class = type;
}

ClassType Object::getClassType()
{
	return m_class;
}

void Object::heal()
{
}



SoundFileHandler* Object::getSoundHandler()
{
	if (m_hasSound)
	{
		return m_sfh;
	}
	else
	{
		std::cout << "ERROR: The Node has no SoundFileHandler attached!" << std::endl;
		return 0;
	}
}

void Object::setSoundHandler(SoundFileHandler* soundHandler)
{
	m_hasSound = true;
	m_sfh = soundHandler;
}