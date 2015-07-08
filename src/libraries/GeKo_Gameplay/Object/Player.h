#pragma once
#include <glm/ext.hpp>
#include <iostream>

#include "Object.h"
/** 
The Player class is a Basic class to provide a playable character. It is just an Basic-Class and every "real" character-class has to be a generalisation of this class.
It provides a spawnpoint location, a  name, a current, a speed and can be visualized by a Geometry. The Geometry has to be bound to a Node (see Scenegraph for details!).
*/
enum Soundtype{
	MOVESOUND, BACKGROUNDMUSIC, FIGHTSOUND, EATSOUND, QUESTSOUND, ITEMSOUND, FIRESOUND, LEVELUP, RAIN, COIN, SECRET, CHOP, BATTLEMUSIC, WIN, LOSE
};

class Player : public Object, public Subject<Player, Object_Event>
{

public:	

	/// Contructor needs at least a name
	Player(std::string playerName);
	/// Second constructor with a Name and a Spawnpoint
	/** The Spawnpoint will be the first position of the character on the playground*/
	Player(std::string playerName, glm::vec3 spawnPoint);
	Player();
	~Player();

	///Returns the m_spawnPoint position
	/**/
	glm::vec3 getSpawnPoint();

	//Updates the object per Frame
	void update();

	///A method to move forward in view Direction
	/**Changes the position vector*/
	virtual void moveForward();
	///A method to move backwards in relation to view direction
	/**Changes the position vector*/
	virtual void moveBackward();
	///A method to move sidewards, right
	/**Changes the position vector*/
	virtual	void moveRight();
	///A method to move sidewards, left
	/**Changes the position vector*/
	virtual	void moveLeft();

	///A method to turn left
	/**Changes the direction vector*/
	void turnLeft();
	///A method to turn right
	/**Changes the direction vector*/
	void turnRight();
	
	///A method to rotate the view direction
	/**This method is used to simulate the movement of the head of the character*/
	void rotateView(float leftRight, float upDown);

	float getPhi();
	float getAlpha();

	///Returns the m_sourceName string
	/**If a Sound-File-Handler was attached, the m_sourceName contains the name of the source which should be played!*/
	std::string getSourceName(Soundtype type);
	///Sets a specific source-file to the node
	/**This method uses the sfh to generate a new sound-source which can be played with the sfh later ingame!*/
	void setSourceName(Soundtype type, std::string sourceName, const char* filepath);
	///Updates the positions of the sources in the map to the most current position of the Player. The Method is called primarily by the event OBJECT_MOVED.
	void updateSourcesInMap();

	void setDeltaTime(float dt);

	//TODO: Open/Close Inventory: Prüfen ob Inventar offen oder geschlossen ist; Inventar mit GUI öffnen
	void showInventory();

	///Adds all pressed keys to m_activeKey
	/**Filled in key_callback*/
	void addKeyInput(int key);
	///Clears m_activeKey
	/**Should be used in the render-loop*/
	void deleteKeyInput();
	///Checks if W, A, S, D are active
	/**Used for activating/deactivating the sound*/
	bool checkActiveMoveKeys();


	float getExp();
	void addExp(float exp);
	void setExp(float exp);

	int getLvl();
	void levelUp();
	void setLevel(int level);

	float getLevelThreshold();
	void setLevelThreshold(float threshold);

	//Implemented in the specific Player
	virtual void setFire();
	virtual bool isDancing();
	virtual void setDancing(bool dancing);

protected:

	int m_level;
	float m_levelThreshold;

	glm::vec3 m_spawnPoint;

	float m_deltaTime;

	//Phi: step-size of x-axis-angle; Theta: step-size of y-axis-angle; Alpha: complete size of x-axis-angle
	float m_phi, m_theta, m_alpha;

	float m_speedTurn;

	std::map<Soundtype, std::string> m_soundMap;

	//For adjustment of the player's step-sound
	int m_StoppSoundCounter;
	std::vector<int> m_activeKey;

	int m_i;


};