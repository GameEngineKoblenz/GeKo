#pragma once
#include "GeKo_Gameplay/Object/Object.h"
#include "GeKo_Gameplay/Object/Ant.h"
#include "GeKo_Gameplay/Object/Ant_Guardian.h"
#include "GeKo_Gameplay/Object/Ant_Worker.h"

#include "GeKo_Gameplay/AI_Decisiontree/DecisionTree.h"
#include "GeKo_Gameplay/AI_Pathfinding/Graph.h"

#include "GeKo_Graphics/Geometry/AntMesh.h"
#include "GeKo_Graphics/Material/Texture.h"
#include "GeKo_Graphics/Scenegraph/Node.h"

#include "GeKo_Gameplay/Observer/ObjectObserver.h"
#include "GeKo_Gameplay/Observer/SoundObserver.h"
#include "GeKo_Gameplay/Observer/GravityObserver.h"

#include "GeKo_Sound/SoundFileHandler.h"

#include <GeKo_Graphics/Geometry/Terrain.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <vector>


///This class represents an anthome and is used to generate ants at a certain spawning position.
class AntHome : public StaticObject, public Subject<StaticObject, Object_Event>{
public:
	AntHome();
	AntHome(glm::vec3 position, SoundFileHandler *sfh, Geometry antMesh,SoundObserver *soundObserver, ObjectObserver *objectObserver, Texture *guardTex, Texture *workerTex, Graph<AStarNode, AStarAlgorithm> *afraidGraph);
	~AntHome();
	void generateGuards(int i, Node* root);
	void generateWorkers(int i);
	void generateWorkers(int i, Node* root);

	void generateSound(AI *ai);
	void updateAnts();
	void addAntsToSceneGraph(Node *rootNode);
	//void putObserver();
	void printPosGuards();
	void printPosWorkers();
	void setAntScale(float f);
	float getAntScale();

	void setGraphGuards();
	void setGrapHighOnTerrain(Terrain* t);

	void resetDeadGuard(int i);
	void resetDeadWorker(int i);
	void resetDeadAnt(AntType type);

protected:
	std::vector<Node*> m_guards;
	std::vector<Node*> m_workers;
	glm::vec3 m_position;

	//AI: n_id; SoundFile: identification
	int m_numberOfAnts;
	
	Geometry m_antMesh;
	Texture *m_guardTexture;
	Texture *m_workerTexture;
	Gravity *m_gravity;

	Graph<AStarNode, AStarAlgorithm> *m_guardGraph;
	int m_guardsDistanceToHome;
	Graph<AStarNode, AStarAlgorithm> *m_afraidGraph;

	DecisionTree *m_guardDecisionTree;
	DecisionTree *m_workerDecisionTree;

	int m_numberOfGuards;
	int m_numerOfDeadGuards;
	int m_numberOfWorkers;
	int m_numberOfDeadWorkers;

	SoundFileHandler *m_sfh;
	ObjectObserver *m_objectObserver;
	SoundObserver *m_soundObserver;
	float m_antScale;
};