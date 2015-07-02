#include "GeKo_Gameplay/Object/AntHome.h"

AntHome::AntHome(){

}

AntHome::AntHome(glm::vec3 position, SoundFileHandler *sfh, Geometry antMesh, SoundObserver *soundObserver, ObjectObserver *objectObserver, Texture *guardTex, Texture *workerTex, Graph<AStarNode, AStarAlgorithm> *afraidGraph){
	m_myNodeName = "Anthome";
	
	m_position = position;
	m_antMesh = antMesh;
	//Texture texCV((char*)RESOURCES_PATH "/cv_logo.bmp");
	m_guardTexture = guardTex;
	m_workerTexture = workerTex;

	m_guardsDistanceToHome = 6;
	setGraphGuards();
	m_afraidGraph = afraidGraph;

	DecisionTree *aggressivedecisionTree = new DecisionTree();
	aggressivedecisionTree->setAntTreeAggressiv();
	m_guardDecisionTree = aggressivedecisionTree;
	DecisionTree *afraidDecisionTree = new DecisionTree();
	afraidDecisionTree->setAntTreeAfraid();
	m_workerDecisionTree = afraidDecisionTree;

	m_numberOfAnts = 0;
	m_numberOfGuards = 0;
	m_numerOfDeadGuards = m_numberOfGuards;
	m_numberOfWorkers = 0;
	m_numberOfDeadWorkers = m_numberOfWorkers;
	m_objectObserver = objectObserver;
	m_soundObserver = soundObserver;
	m_gravity = new Gravity();
	m_sfh = sfh;
	m_antScale = 1.0;

	m_type = ObjectType::HOUSE;
	
	Item cookie(1);
	cookie.setName("Cookie");
	cookie.setTypeId(ItemType::COOKIE);
	m_inventory->addItem(&cookie, 100);
}

AntHome::~AntHome(){

}

void AntHome::generateGuards(int i, Node *root){
	if (i < 0)
		return;
	glm::vec4 position;
	time_t t;
	time(&t);
	srand((unsigned int)t);
	std::stringstream name;
	while (i > 0){
		name << "Guard" << m_numberOfGuards++ + 1;
		Node *aiGuardNode = new Node(name.str());
		//Node (scenegraph)
		aiGuardNode->addScale(m_antScale, m_antScale, m_antScale);
		aiGuardNode->addGeometry(&m_antMesh);
		aiGuardNode->addTexture(m_guardTexture);
		aiGuardNode->addGravity(m_gravity);
		//Position
		position.x = rand() * 3 / 32767.0;
		position.z = rand() * 3 / 32767.0;
		position.y = 0.0;
		position.w = 0.0;
		//Ant (AI)
		Ant_Guardian *antAI = new Ant_Guardian();
		antAI->setAntAggressiv(m_numberOfAnts, name.str(), m_guardDecisionTree, m_guardGraph);
		aiGuardNode->setObject(antAI);
		antAI->addObserver(m_objectObserver);
		antAI->setSoundHandler(m_sfh);
		antAI->addObserver(m_soundObserver);
		generateSound(antAI);
		name.str("");
		root->addChildrenNode(aiGuardNode);
		m_guards.push_back(aiGuardNode);
		m_numerOfDeadGuards = m_numberOfGuards;
		i--;
		//printPosGuards();
	}
}


void AntHome::generateWorkers(int i, Node* root){
	if (i < 0)
		return;
	glm::vec4 position;
	time_t t;
	time(&t);
	srand((unsigned int)t);
	std::stringstream name;
	while (i > 0){
		name << "Worker" << m_numberOfWorkers++ + 1;
		Node *aiWorkerNode = new Node(name.str());
		aiWorkerNode->addScale(m_antScale, m_antScale, m_antScale);
		aiWorkerNode->addGeometry(&m_antMesh);
		aiWorkerNode->addTexture(m_workerTexture);
		aiWorkerNode->addGravity(m_gravity);
		//position.x = rand() * 1.0f / 32767.0f;
		//position.z = rand() * 1.0f / 32767.0f;
		//position.y = 10.0f;
		//position.w = 0.0;
		Ant_Worker *antAI = new Ant_Worker(glm::vec4(m_position.x, m_position.y + 10, m_position.z, 1.0), (rand()* 5.0f / 32767.0f));
		//Ant *antAI = new Ant(glm::vec4(m_position, 1.0) + position);
		antAI->setAntAfraid(m_numberOfAnts, name.str(), m_workerDecisionTree, m_afraidGraph);
		//antAI->setAntAfraid();
		aiWorkerNode->setObject(antAI);
		antAI->addObserver(m_objectObserver);
		antAI->setSoundHandler(m_sfh);
		antAI->addObserver(m_soundObserver);
		generateSound(antAI);
		name.str("");
		root->addChildrenNode(aiWorkerNode);
		m_workers.push_back(aiWorkerNode);
		m_numberOfDeadWorkers = m_numberOfWorkers;
		i--;
		//printPosWorkers();
	}
}


void AntHome::generateSound(AI *ai){
	std::stringstream name;
	name << "AIFootsteps" << m_numberOfAnts + 1;
	std::string namestring;
	namestring = name.str();
	ai->setSourceName(MOVESOUND_AI, namestring, RESOURCES_PATH "/Sound/Footsteps.wav");
	m_sfh->disableLooping(namestring);
	name.str("");
	name << "AIDeath" << m_numberOfAnts + 1;
	namestring = name.str();
	ai->setSourceName(DEATHSOUND_AI, namestring, RESOURCES_PATH "/Sound/death.wav");
	m_sfh->disableLooping(namestring);
	name.str("");
	name << "AIEssen" << m_numberOfAnts + 1;
	namestring = name.str();
	ai->setSourceName(EATSOUND_AI, namestring, RESOURCES_PATH "/Sound/Munching.wav");
	m_sfh->disableLooping(namestring);
	name.str("");
	name << "Flies" << m_numberOfAnts++ + 1;
	namestring = name.str();
	ai->setSourceName(DEATHSOUND_FLIES_AI, namestring, RESOURCES_PATH "/Sound/Fliege kurz.wav");
	//m_sfh->disableLooping(namestring);
	m_sfh->setGain(namestring, 7.0);
	name.str("");
}

void AntHome::addAntsToSceneGraph(Node *rootNode){
	for (Node* antNode : m_guards){
		rootNode->addChildrenNode(antNode);
	}
	for (Node* antNode : m_workers){
		rootNode->addChildrenNode(antNode);
	}
}

void AntHome::updateAnts(){

	for (int i = 0; i < m_guards.size(); i++){
		m_guards.at(i)->getAI()->update();
	}
	for (int i = 0; i < m_workers.size(); i++){
		m_workers.at(i)->getAI()->update();
	}
}

void AntHome::printPosGuards(){
	for (int i = 0; i < m_guards.size(); i++){
		std::cout << "Guard " << i << " Pos : x :" << m_guards[i]->getAI()->getPosition().x << " ; z: " << m_guards[i]->getAI()->getPosition().z << std::endl;
	}
}

void AntHome::printPosWorkers(){
	for (int i = 0; i < m_workers.size(); i++){
		std::cout << "Worker " << i << " Pos : x :" << m_workers[i]->getAI()->getPosition().x << " ; z: " << m_workers[i]->getAI()->getPosition().z << std::endl;
	}
}

void AntHome::setAntScale(float scale){
	m_antScale = scale;
}

float AntHome::getAntScale(){
	return m_antScale;
}

void AntHome::setGraphGuards(){
	//TODO: Anpassen der Höhenwerte (mGuardGraph.y) an die Höhenwerte vom Terrain anpassen! Sonst erkennt die Ant nicht, dass sie am Ziel angekommen ist
	Graph<AStarNode, AStarAlgorithm>* antAggressiveGraph = new Graph<AStarNode, AStarAlgorithm>();
	antAggressiveGraph->setExampleAntAggressiv(m_position, m_guardsDistanceToHome);
	m_guardGraph = antAggressiveGraph;
}

void AntHome::setGrapHighOnTerrain(Terrain* t){
	for (int i = 0; i < m_guardGraph->getGraph()->size(); i++){
		AStarNode* node = m_guardGraph->getGraph()->at(i);
		glm::vec3 pos = node->getPosition();
		node->setPosition(glm::vec3(pos.x, t->getHeight(glm::vec2(pos.x, pos.z))+1, pos.z));
	}
}

void AntHome::resetDeadGuard(int i){
	//Ant_Guardian guard = m_guards.at(i)->getAI();

	//TODO: Can´t walk after dead, (First fix, that can walk after meet player)
	if (m_guards.size() > 0 && i < m_guards.size()){
		m_guards.at(i)->getAI()->setPosition(glm::vec3(m_position.x - 5, m_position.y, m_position.z + 5));

		m_guards.at(i)->getAI()->setHealth(m_guards.at(i)->getAI()->getHealthMax());
		m_guards.at(i)->getAI()->setHunger(m_guards.at(i)->getAI()->getHungerMax());
		m_numerOfDeadGuards += 1;
		std::stringstream name;
		name << "Guard" << m_numerOfDeadGuards;
		m_guards.at(i)->getAI()->setName(name.str());
		m_guards.at(i)->getAI()->getInventory()->clearInventory();

		m_guards.at(i)->getAI()->setHasDied(false);
	}
}

void AntHome::resetDeadWorker(int i){
	//Ant worker = m_workers.at(i)->getAI();

	if (m_workers.size() > 0 && i < m_workers.size()){
		m_workers.at(i)->getAI()->setPosition(m_position);
		m_workers.at(i)->getAI()->setHealth(m_workers.at(i)->getAI()->getHealthMax());
		m_workers.at(i)->getAI()->setHunger(m_workers.at(i)->getAI()->getHungerMax());
		m_numberOfDeadWorkers += 1;
		std::stringstream name;
		name << "Worker" << m_numberOfDeadWorkers;
		m_workers.at(i)->getAI()->setName(name.str());
		m_workers.at(i)->getAI()->getInventory()->clearInventory();

		m_workers.at(i)->getAI()->setHasDied(false);
	}
}

void AntHome::resetDeadAnt(AntType type){
	if (type == AntType::GUARD){
		resetDeadGuard(0);
	}
	if (type == AntType::QUEEN){
		resetDeadWorker(0);
	}
}