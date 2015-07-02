#include "GeKo_Gameplay/Object/Ant_Guardian.h"
#include <stdexcept>

Ant_Guardian::Ant_Guardian(){
	m_id = 0;
	m_type = ObjectType::ANT;
	m_antType = AntType::GUARD;
	m_name = "defaultGuard";

	m_myNodeName = "NULL";

	m_hunger = 10;

	m_hungerMax = 10;
	m_health = 1000;
	m_healthMax = 1000;
	m_strength = 10;
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

	m_speed = 0.1;
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

Ant_Guardian::~Ant_Guardian(){}

void Ant_Guardian::decide(){
	//std::cout << "********** DecideMethod **********" << std::endl;
	TreeOutput lastOutput = m_targetType;
	m_targetType = m_decisionTree->decide(m_states);

	switch (m_targetType) {
	case TreeOutput::FOOD:
		if (lastOutput != m_targetType){
			m_target = m_homeNode;
			updatePath();
		}
		break;
	case TreeOutput::PLAYER:
		//TODO Nach dem Player läuft die Ant nicht mehr zu anderen Zielen
		if (lastOutput != m_targetType){
			m_target = m_playerNode;
			updatePath();
		}
		break;
	case TreeOutput::PATROL:
		//New decision to walk an path
		if (lastOutput != m_targetType){
			m_path = m_pathPatrol;
			m_target = m_path.back();
			m_path.pop_back();
		}
		//Already walking on path and coming to end
		if (m_path.size() == 0){
			m_path = m_pathPatrol;
		}
		//if (checkPosition(glm::vec3(m_position), m_target->getPosition())){
		//	//if (m_path.size() == 1){
		//	updatePathPatrol();
		//	//m_path = m_pathPatrol;
		//	//}
		//}
		break;
	}
	//std::cout << "**********************************" << std::endl;
}

void Ant_Guardian::setPathPatrol(){
	for (int i = 0; i < m_graph->getGraph()->size(); i++){
		if (m_graph->getGraph()->at(i)->getNodeType() == GraphNodeType::OTHER){
			m_pathPatrol.push_back(m_graph->getGraph()->at(i));
		}
	}
}

void Ant_Guardian::updatePath(){
	m_path.clear();

	m_path.push_back(m_target);
	//std::cout << "Laufe auf Player oder Home(Food) zu!!!" << std::endl;

	m_nextTarget = m_target;
	m_lastTargetOnGraph = m_lastTarget;
}

//void Ant_Guardian::rotatePathPatrol(){
//	m_path.clear();
//	std::vector<AStarNode*> tmp = m_pathPatrol;
//	m_pathPatrol.clear();
//	for (int i = 0; i < tmp.size() - 1; i++){
//		m_pathPatrol.push_back(tmp.at(i + 1));
//	}
//	m_pathPatrol.push_back(tmp.at(0));
//	m_path = m_pathPatrol;
//	m_path.pop_back();
//	m_nextTarget = m_path.back();
//	m_target = m_path.at(0);
//	//std::cout << "Laufe auf Patrouille!!!" << " Next target at path: " << m_nextTarget->getName() << std::endl;
//}

void Ant_Guardian::setAntAggressiv(int id, std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph){
	m_id = id;
	m_name = name;
	m_decisionTree = tree;

	m_graph = antGraph;
	setPathPatrol();
	m_path = m_pathPatrol;

	//Start position
	m_target = m_path.back();
	m_lastTarget = m_target;
	m_lastTargetOnGraph = m_target;
	m_nextTarget = m_target;
	m_position = glm::vec4(m_target->getPosition(), 1);
	
	//...
	m_homeNode = antGraph->searchNode(GraphNodeType::FOOD);
	m_foodNodes.clear(); //Delete DefaultNode
	m_playerNode = m_graph->searchNode(GraphNodeType::OBJECT);
}