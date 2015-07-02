#pragma once
//
#include <glm/glm.hpp>
#include <iostream>
#include <string>

#include "GeKo_Gameplay/Observer/Subject.h"
#include "GeKo_Gameplay/Item/Inventory.h"
#include "GeKo_Sound/SoundFileHandler.h"

#include "ObjectType.h"
#include "ClassType.h"
#include "States.h"

///This interface provides all information an object could need, like inventory, level, sound etc.
class Object
{
public: 
	Object();
	~Object();

	void setName(std::string newName);
	std::string getName();

	void setPosition(glm::vec4 pos);
	void setPosition(glm::vec3 pos);
	glm::vec4 getPosition();

	float getStrength();

	void setHealth(float newHealth);
	float getHealth();
	float getHealthMax();

	void setHunger(float newHunger);
	float getHunger();
	float getHungerMax();

	void setNodeName(std::string nodeName);
	std::string getNodeName();

	Inventory* getInventory();

	void setObjectType(ObjectType type);
	ObjectType getObjectType();

	void setClassType(ClassType type);
	ClassType getClassType();

	glm::vec4 getViewDirection();
	void setViewDirection(glm::vec4 viewDirection);
	bool getStates(States state);
	void setStates(States state, bool b);

	virtual void update();
	void updateStates();

	///Moves Object on Terrain
	virtual void move();

	virtual void eat();

	void collectItem(ItemType typeId, int count);

	virtual void heal();


	///Returns the m_sfh
	/**This Handler manages all the sounds of an object like the movement of a Player etc.*/
	SoundFileHandler* getSoundHandler();
	void setSoundHandler(SoundFileHandler* soundHandler);

protected:
	int m_id;
	ObjectType m_type;
	std::string m_name;
	std::string m_myNodeName;
	ClassType m_class;

	glm::vec4 m_position;
	glm::vec4 m_viewDirection;

	std::vector<std::pair<States, bool>> m_states;
	float m_hunger;
	float m_hungerMax;
	float m_health;
	float m_healthMax;
	float m_speed;
	float m_strength;
	float m_exp;
	

	Inventory* m_inventory;
	SoundFileHandler* m_sfh;

	bool m_hasSound;
};