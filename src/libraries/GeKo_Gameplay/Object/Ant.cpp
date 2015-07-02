#include "GeKo_Gameplay/Object/Ant.h"
#include <stdexcept>

Ant::Ant(){
	m_id = 0;
	m_type = ObjectType::DEFAULTOBJECT;
	m_antType = AntType::DEFAULTANT;
	m_name = "defaultObject";

	m_myNodeName = "NULL";

	m_hunger = 10;

	m_hungerMax = 10;
	m_health = 1000;
	m_healthMax = 1000;
	m_strength = 0.5;
	m_hasDied = false;

	m_decisionTree = new DecisionTree();

	std::pair<States, bool> s(States::HUNGER, false);
	m_states.push_back(s);
	s.first = States::VIEW;
	s.second = false;
	m_states.push_back(s);
	s.first = States::HEALTH;
	s.second = true;
	m_states.push_back(s);

	m_speed = 0.01;
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

Ant::Ant(glm::vec4 position){
	m_id = 0;
	m_type = ObjectType::DEFAULTOBJECT;
	m_antType = AntType::DEFAULTANT;
	m_name = "defaultObject";

	m_myNodeName = "NULL";

	m_hunger = 10;
	m_hungerMax = 10;
	m_health = 1000;
	m_healthMax = 1000;
	m_strength = 0.5;
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

	m_speed = 0.01;
	m_epsilon = 0.1;

	m_position = position;

	AStarNode* defaultNode = new AStarNode();

	m_lastTarget = defaultNode;
	m_target = defaultNode;
	m_nextTarget = defaultNode;
	m_lastTargetOnGraph = defaultNode;

	m_homeNode = defaultNode;
	m_foodNodes.push_back(defaultNode);
	m_graph = new Graph<AStarNode, AStarAlgorithm>();

	m_viewRadius = 1.0f;

	m_inventory = new Inventory();

	m_targetType = TreeOutput::HOME;

	m_class = ClassType::AI;

}

Ant::~Ant(){}

void Ant::decide(){
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

void Ant::updatePath(){
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

void Ant::updatePathPlayer(){
	m_path.clear();

	m_path.push_back(m_target);
	//std::cout << "Laufe auf Player zu!!!" << std::endl;
	m_nextTarget = m_target;
	m_lastTargetOnGraph = m_lastTarget;
}

void Ant::updatePathPatrol(){
	m_path.clear();
	std::vector<AStarNode*> tmp = m_pathPatrol;
	m_pathPatrol.clear();
	for (int i = 0; i < tmp.size() - 1; i++){
		m_pathPatrol.push_back(tmp.at(i + 1));
	}
	m_pathPatrol.push_back(tmp.at(0));
	m_path = m_pathPatrol;
	m_path.pop_back();
	m_nextTarget = m_path.back();
	m_target = m_path.at(0);
	//std::cout << "Laufe auf Patrouille!!!" << " Next target at path: " << m_nextTarget->getName() << std::endl;
}

AStarNode* Ant::nextNodeOnPatrol(){
	std::vector<AStarNode*>* temp = m_graph->getGraph();
	if (m_target->getNodeType() != GraphNodeType::DEFAULT){
		if (checkPosition(glm::vec3(m_position), m_nextTarget->getPosition())){
			for (int i = 0; i < temp->size(); i++){
				if (m_lastTarget->getNodeType() == GraphNodeType::OBJECT || m_lastTarget->getNodeType() == GraphNodeType::FOOD){
					if (temp->at(i)->getNodeType() == GraphNodeType::OTHER){
						return temp->at(i);
					}
				}
				else {
					if (m_lastTarget == temp->at(i)){
						int j = (i + 1) % temp->size();
						while (temp->at(j) != m_lastTarget){
							if (temp->at(j)->getNodeType() == GraphNodeType::OTHER){
								return temp->at(j);
							}
							j = (j + 1) % temp->size();
						}
					}
				}
			}
		}
	}
}

void Ant::setAntAfraid(){
	m_type = ObjectType::ANT;
	m_name = "Flick";

	DecisionTree* tree = new DecisionTree();
	tree->setAntTreeAfraid();
	m_decisionTree = tree;

	m_speed = 0.1;

	glm::vec3 posFood(10.0, 0.0, -5.0);
	glm::vec3 posSpawn(3.0, 0.0, 3.0);
	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
	Graph<AStarNode, AStarAlgorithm>* antGraph = new Graph<AStarNode, AStarAlgorithm>();
	antGraph->setExampleAntAfraid(posSpawn, posFood, posDefaultPlayer);
	m_graph = antGraph;

	m_lastTarget = antGraph->searchNode(GraphNodeType::HOME);
	m_target = antGraph->searchNode(GraphNodeType::HOME);
	m_nextTarget = antGraph->searchNode(GraphNodeType::HOME);

	m_position = glm::vec4(posSpawn, 1.0);
	m_homeNode = antGraph->searchNode(GraphNodeType::HOME);
	m_foodNodes.pop_back();
	addFoodNodes();
}

void Ant::setAntAfraid(std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph){
	m_type = ObjectType::ANT;
	m_name = name;

	/*DecisionTree* tree = new DecisionTree();
	tree->setAntTreeAfraid();*/
	m_decisionTree = tree;

	m_speed = 0.1;
	m_strength = 5;
	//generiert hunger zwischen 5 und 10
	//m_hunger = (rand() % ((10 + 1) - 5)) + 5;

	m_graph = antGraph;

	m_lastTarget = antGraph->searchNode(GraphNodeType::HOME);
	m_target = m_lastTarget;
	m_nextTarget = m_lastTarget;

	m_homeNode = m_lastTarget;
	m_foodNodes.pop_back();
	addFoodNodes();
}

//void Ant::setAntAggressiv(){
//	m_type = ObjectType::ANT;
//	m_name = "Flack";
//
//	DecisionTree* tree = new DecisionTree();
//	tree->setAntTreeAggressiv();
//	m_decisionTree = tree;
//
//	m_speed = 0.1;
//	m_strength = 10;
//
//	glm::vec3 posFood(10.0, 0.0, -5.0);
//	glm::vec3 posSpawn(3.0, 0.0, 3.0);
//	glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
//	Graph<AStarNode, AStarAlgorithm>* antGraph = new Graph<AStarNode, AStarAlgorithm>();
//	antGraph->setExampleAntAggressiv(posSpawn, posSpawn, posDefaultPlayer);
//	m_graph = antGraph;
//	for (int i = 0; i < m_graph->getGraph()->size(); i++){
//		if (m_graph->getGraph()->at(i)->getNodeType() == GraphNodeType::OTHER){
//			m_pathPatrol.push_back(m_graph->getGraph()->at(i));
//		}
//	}
//
//	m_lastTarget = antGraph->searchNode(GraphNodeType::FOOD);
//	m_target = m_lastTarget;
//	m_nextTarget = m_lastTarget;
//	m_lastTargetOnGraph = m_lastTarget;
//
//	m_position = glm::vec4(posSpawn, 1.0);
//	m_homeNode = m_lastTarget;
//	m_foodNodes.pop_back(); //Delete DefaultNode
//	m_foodNodes.push_back(antGraph->searchNode(GraphNodeType::FOOD));
//}
//
//void Ant::setAntAggressiv(std::string name, DecisionTree *tree, Graph<AStarNode, AStarAlgorithm> *antGraph){
//	m_type = ObjectType::ANT;
//	m_name = name;
//
//	/*DecisionTree* tree = new DecisionTree();
//	tree->setAntTreeAggressiv();*/
//	m_decisionTree = tree;
//
//	m_speed = 0.1;
//	m_strength = 10;
//
//	//glm::vec3 posFood(10.0, 0.0, -5.0);
//	//glm::vec3 posSpawn(3.0, 0.0, 3.0);
//	//glm::vec3 posDefaultPlayer(0.0, 0.0, 0.0);
//	/*Graph<AStarNode, AStarAlgorithm>* antGraph = new Graph<AStarNode, AStarAlgorithm>();
//	antGraph->setExampleAntAggressiv(posSpawn, posSpawn, posDefaultPlayer);*/
//	m_graph = antGraph;
//	for (int i = 0; i < m_graph->getGraph()->size(); i++){
//		if (m_graph->getGraph()->at(i)->getNodeType() == GraphNodeType::OTHER){
//			m_pathPatrol.push_back(m_graph->getGraph()->at(i));
//		}
//	}
//
//	m_lastTarget = antGraph->searchNode(GraphNodeType::FOOD);
//	m_target = m_lastTarget;
//	m_nextTarget = m_lastTarget;
//	m_lastTargetOnGraph = m_lastTarget;
//
//	//m_position = glm::vec4(posSpawn, 1.0);
//	m_homeNode = m_lastTarget;
//	m_foodNodes.clear(); //Delete DefaultNode
//	m_foodNodes.push_back(m_lastTarget);
//}