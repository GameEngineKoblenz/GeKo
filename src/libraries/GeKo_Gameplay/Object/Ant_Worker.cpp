#include "GeKo_Gameplay/Object/Ant_Worker.h"
#include <stdexcept>

Ant_Worker::Ant_Worker(){
	m_id = 0;
	m_type = ObjectType::ANT;
	m_antType = AntType::WORKER;
	m_name = "fedaultWorker";

	m_myNodeName = "NULL";

	m_strength = 5;

	m_hunger = 10;
	m_hungerMax = 10;
	m_health = 10000;
	m_healthMax = 10000;

	m_hasDied = false;

	m_decisionTree = new DecisionTree();

	m_states.clear();
	std::pair<States, bool> s(States::HUNGER, false);
	m_states.push_back(s);
	s.first = States::VIEW;
	s.second = false;
	m_states.push_back(s);
	s.first = States::HEALTH;
	s.second = true;
	m_states.push_back(s);

	m_speed = 0.04;
	m_epsilon = 0.1;

	m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);

	AStarNode* defaultNode = new AStarNode();

	m_lastTarget = defaultNode;
	m_target = defaultNode;
	m_nextTarget = defaultNode;
	m_lastTargetOnGraph = defaultNode;

	m_homeNode = defaultNode;
	//m_foodNodes.push_back(defaultNode);
	m_graph = new Graph<AStarNode, AStarAlgorithm>();

	m_viewRadius = 1.0f;

	m_inventory = new Inventory();

	m_targetType = TreeOutput::HOME;

	m_class = ClassType::AI;
}

Ant_Worker::~Ant_Worker(){}

void Ant_Worker::decide(){
	//std::cout << "********** DecideMethod **********" << std::endl;
	TreeOutput lastOutput = m_targetType;
	m_targetType = m_decisionTree->decide(m_states);

	switch (m_targetType) {
	case TreeOutput::HOME:
		if (lastOutput != m_targetType){
			m_target = m_homeNode;
			updatePath();
		}
		break;
	case TreeOutput::FOOD:
		if (lastOutput != m_targetType){
			m_target = nearestFoodNode();
			updatePath();
		}
		break;
	case TreeOutput::PLAYER:
		if (lastOutput != m_targetType){
			m_target = m_graph->searchNode(GraphNodeType::OBJECT);
			updatePathPlayer();
		}
		break;
	case TreeOutput::PATROL:
		if (lastOutput != m_targetType){
			m_target = nextNodeOnPatrol();
			updatePathPatrol();
		}
		if (checkPosition(glm::vec3(m_position), m_target->getPosition())){
			//if (m_path.size() == 1){
			updatePathPatrol();
			//m_path = m_pathPatrol;
			//}
		}
		break;
	}
	//std::cout << "**********************************" << std::endl;
}

void Ant_Worker::updatePath(){
	m_graph->calculateDistanceToGoal(m_target);
	m_graph->getAlgorithm()->startAlgorithm2(m_lastTarget, m_target, m_path);
	AStarNode defaultNode;
	m_graph->resetAlgorithm(&defaultNode);
	m_nextTarget = m_path.back();
	m_path.pop_back();

	if (m_lastTarget->getNodeType() == GraphNodeType::OBJECT && m_target->getNodeType() != GraphNodeType::OBJECT){
		m_nextTarget = m_lastTargetOnGraph;
	}
}

void Ant_Worker::updatePathPlayer(){
	m_path.clear();

	m_path.push_back(m_target);
	//std::cout << "Laufe auf Player zu!!!" << std::endl;
	m_nextTarget = m_target;
	m_lastTargetOnGraph = m_lastTarget;
}

void Ant_Worker::setAntAfraid(int id, std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph){
	m_id = id;
	m_name = name;

	m_hunger = m_hungerMax; //Crazy bug without this setting xD

	m_decisionTree = tree;

	//generiert hunger zwischen 5 und 10
	//m_hunger = (rand() % ((10 + 1) - 5)) + 5;

	m_graph = antGraph;

	//Start position
	m_target = antGraph->searchNode(GraphNodeType::HOME);
	m_lastTarget = m_target;
	m_nextTarget = m_target;
	m_lastTargetOnGraph = m_target;
	m_position = glm::vec4(m_target->getPosition(), 1);

	//...
	m_homeNode = m_target;
	m_foodNodes.clear(); //Delete DefaultNode
	addFoodNodes();
}