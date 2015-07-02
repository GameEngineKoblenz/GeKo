#pragma once

#include "GeKo_Gameplay/Object/Ant.h"

///This class represents a AI which will react and decide like an Ant. 
class Ant_Worker : public Ant
{
public:

	Ant_Worker();
	Ant_Worker(glm::vec4 position, int hunger) :Ant_Worker(){
			m_hunger = hunger;
			m_position = position;
	};
	~Ant_Worker();

	virtual void decide();

	virtual void updatePath();
	void updatePathPlayer();

	void setAntAfraid(int id,  std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph);

protected:
};