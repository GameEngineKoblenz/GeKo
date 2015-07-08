#pragma once

#include "GeKo_Gameplay/Object/Player.h"

/**This class is a first generalisation of the Player class.*/
class Geko : public Player
{
public:
	///Same Constructors as the Player-class
	/** The Speed will be set to 0.8 and viewDirection and movement vector will be set automatically!*/
	Geko(std::string gekoName);
	Geko(std::string gekoName, glm::vec3 spawnPoint);
	Geko();
	~Geko();

	///A method to move forward in view Direction
	/**Changes the position vector*/
	void moveForward();
	///A method to move backwards in relation to view direction
	/**Changes the position vector*/
	void moveBackward();
	///A method to move sidewards, right
	/**Changes the position vector*/
	void moveRight();
	///A method to move sidewards, left
	/**Changes the position vector*/
	void moveLeft();


	///Sets a fire into the world
	/**This method deletes one branch in the inventory and creates a fire with the help of the particlesystem*/
	void setFire();
	
	void setDancing(bool dancing);
	bool isDancing();

protected:
	bool m_rainDanceActive;
	bool m_step1;
	bool m_step2;
	bool m_step3;
	bool m_step4;
	bool m_step5;


};