#pragma once

#include "GeKo_Gameplay/Object/Ant.h"

///This class represents a AI which will react and decide like an Ant. 
class Ant_Guardian : public Ant
{
public:

	Ant_Guardian();
	Ant_Guardian(glm::vec4 position, int hunger) :Ant(){
			m_hunger = hunger;
			m_position = position;
	};
	~Ant_Guardian();

	void decide();

	//Clips (deletes all nodes not type OTHER) from m_path to m_pathPatrol
	void setPathPatrol();
	//Updates DIRECT path to home(food) or player
	void updatePath();
	//Updates/rotates path to walk on patrol
	//void rotatePathPatrol();

	void setAntAggressiv(int id, std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph);

protected:
	std::vector<AStarNode*> m_pathPatrol;
	AStarNode* m_playerNode;
};