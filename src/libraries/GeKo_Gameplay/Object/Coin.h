#pragma once

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


class Coin : StaticObject{

public:
	Coin();
	Coin(Terrain *terrain, Geometry *coinGeometry, Texture *coinTexture, Node *rootNode);
	~Coin();
	void generateCoin();
	void update();

private:
	void generateNewPosition();
	Node* m_coinInstance;
	Texture *m_coinTexture;
	Geometry *m_coinGeometry;
	Terrain *m_terrain;

	Node* m_rootNode;
	StaticObject* m_staticCoin;


};