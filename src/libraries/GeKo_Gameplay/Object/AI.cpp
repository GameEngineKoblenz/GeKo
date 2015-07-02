#include "GeKo_Gameplay/Object/AI.h"
#include <stdexcept>

AI::AI(){
	m_id = 0;
	m_type = ObjectType::DEFAULTOBJECT;
	m_antType = AntType::NOANT;
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
	m_phi = 0.0;

	m_position = glm::vec4(0.0, 0.0, 0.0, 1.0);

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

AI::AI(glm::vec4 position){
	m_id = 0;
	m_type = ObjectType::DEFAULTOBJECT;
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

AI::~AI(){}

void AI::setPosition(glm::vec4 pos){
	m_position = pos;
	notify(*this, Object_Event::OBJECT_MOVED);
}

void AI::setPosition(glm::vec3 pos){
	m_position = glm::vec4(pos, 1.0);
	notify(*this, Object_Event::OBJECT_MOVED);
}

AStarNode* AI::getPosHome(){
	return m_homeNode;
}

void AI::setPosHome(AStarNode* pos){
	m_homeNode = pos;
	m_lastTarget = pos;
	m_target = pos;
}

Graph<AStarNode, AStarAlgorithm>* AI::getGraph(){
	return m_graph;
}

void AI::setGraph(Graph<AStarNode, AStarAlgorithm>* graph){
	m_graph = graph;
}

DecisionTree* AI::getDecisionTree(){
	return m_decisionTree;
}

void AI::setDecisionTree(DecisionTree* tree){
	m_decisionTree = tree;
}

float AI::getViewRadius(){
	return m_viewRadius;
}

void AI::addFoodNodes(){
	//TODO: Man sollte nur die GraphNodeType::DEFAULT vorher rausfiltern und nicht alle löschen...
	m_foodNodes.clear();
	std::vector<AStarNode*>* nodes = m_graph->getGraph();
	for (int i = 0; i < nodes->size(); i++){
		if (nodes->at(i)->getNodeType() == GraphNodeType::FOOD){
			m_foodNodes.push_back(nodes->at(i));
		}
	}
}

void AI::deleteFoodNode(glm::vec3 pos){
	for (int i = 0; i < m_foodNodes.size(); i++){
		if (m_foodNodes.at(i)->getPosition() == pos){
			m_foodNodes.erase(m_foodNodes.begin()+i);
		}
	}
}

AStarNode* AI::nearestFoodNode(){
	if (m_foodNodes.size() == 1){
		if (m_foodNodes.at(0)->getNodeType() != GraphNodeType::DEFAULT){
			return m_foodNodes.at(0);
		}
	}

	AStarNode* tempDistance;
	m_graph->getAlgorithm()->startAlgorithm2(m_lastTarget, m_foodNodes.at(0), m_path);
	tempDistance = m_path.at(m_path.size() - 1);
	std::vector<AStarNode*> shortestPath = m_path;

	for (int i = 0; i < m_foodNodes.size() - 1; i++){
		m_graph->getAlgorithm()->startAlgorithm2(m_lastTarget, m_foodNodes.at(i + 1), m_path);
		if (tempDistance->getDistanceTravelled() > m_path.at(m_path.size() - 1)->getDistanceTravelled()){
			tempDistance = m_path.at(m_path.size() - 1);
			shortestPath = m_path;
		}
	}

	return shortestPath.at(0);
}

void AI::update(){
	if (m_health <= 0){
		//std::cout << "Object" << m_name << ": is dead!" << std::endl;

		if (!m_hasDied)
		{
			notify(*this, Object_Event::OBJECT_STOPPED);
			notify(*this, Object_Event::OBJECT_DIED);
			m_hasDied = true;
		}

		setStates(States::HEALTH, false);
	}

	if (getStates(States::HEALTH)){
		//std::cout << "<<<<<<<< UpdateMethod <<<<<<<<" << std::endl;
		updateStates();
		GraphNodeType lastNodeType = m_target->getNodeType();
		decide();
		float eps = m_epsilon + m_speed;
		if (!checkPosition(glm::vec3(m_position), m_target->getPosition())){
			move();
		}
		//std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
	}

}

void AI::move(){
	float eps = m_epsilon + m_speed;
	if (checkPosition(glm::vec3(m_position), m_nextTarget->getPosition())){
		if (m_path.size() > 0){
			m_nextTarget = m_path.back();
			m_path.pop_back();
			//std::cout << "Move to: " << m_nextTarget->getName() << std::endl;
			m_lastTarget = m_nextTarget;
		}
	}

	glm::vec3 diff = glm::vec3(m_position) - m_nextTarget->getPosition();
	glm::vec3 diffNorm = glm::normalize(diff);
	bool moved = false;
	if (diff.x > 0){
		m_position.x -= m_speed * diffNorm.x;
		moved = true;
	}
	if (diff.y > 0){
		m_position.y -= m_speed;

	}
	if (diff.z > 0){
		m_position.z -= m_speed * diffNorm.z;
		moved = true;

	}
	if (diff.x < 0){
		moved = true;
		m_position.x += m_speed * -diffNorm.x;
	}
	if (diff.y < 0){
		m_position.y += m_speed;

	}
	if (diff.z < 0){
		m_position.z += m_speed * -diffNorm.z;
		moved = true;

	}

	if (moved){
		//TODO: Update rotation of the geometry
		//m_phi to the observer

		glm::vec2 targetDirection = glm::vec2(m_target->getPosition().x - m_position.x, m_target->getPosition().z - m_position.z);

		targetDirection = glm::normalize(targetDirection);

		float lengthFromTargetDirection = glm::length(targetDirection);

		float lengthViewDirection = glm::length(m_viewDirection);
		glm::vec2  m_viewDirection2 = glm::vec2(m_viewDirection.x, m_viewDirection.z);

		glm::vec3 cross = glm::cross(glm::vec3(targetDirection.x, 0, targetDirection.y), glm::vec3(m_viewDirection2.x, 0, m_viewDirection2.y));

		float dot = glm::dot(targetDirection, m_viewDirection2);
		float tmp = m_phi;
		m_phi = glm::acos(dot / (lengthFromTargetDirection * lengthViewDirection));
		m_phi = m_phi * (180 / glm::pi<float>());


		if (m_phi > 1.0f || m_phi < -1.0f){
			if (cross.y > 0){
				m_phi = 360.0f - m_phi;
			}

			notify(*this, Object_Event::OBJECT_ROTATED);
			m_viewDirection = glm::normalize(glm::vec4(m_target->getPosition().x - m_position.x, 0.0, m_target->getPosition().z - m_position.z, 0.0));
			int i = 0;
		}

		notify(*this, Object_Event::OBJECT_MOVED);
	} else
		notify(*this, Object_Event::OBJECT_STOPPED);

	//std::cout << "Aktuelle Position der AI: x_" << m_position.x << " y_" << m_position.y << " z_" << m_position.z << std::endl;
}

void AI::viewArea(bool state)
{
	setStates(States::VIEW, state);
}

bool AI::checkPosition(glm::vec3 p1, glm::vec3 p2){

	bool rb = false;

	if (p1.x >= (p2.x - (m_epsilon / m_speed)) & p1.x <= (p2.x + (m_epsilon / m_speed)))
	{
		if ((p1.y >= (p2.y - (m_epsilon / m_speed)))& (p1.y <= (p2.y + (m_epsilon / m_speed))))
		{
			if ((p1.z >= (p2.z - (m_epsilon / m_speed)))& (p1.z <= (p2.z + (m_epsilon / m_speed))))
				rb = true;
		}
	}
	else{
		rb = false;
	}
	return rb;
}

void AI::decide(){
	//std::cout << "********** DecideMethod **********" << std::endl;
	m_targetType = m_decisionTree->decide(m_states);

	switch (m_targetType) {
	case TreeOutput::HOME: m_target = m_homeNode; break;
	case TreeOutput::FOOD: m_target = nearestFoodNode(); break;
	case TreeOutput::PLAYER: m_target = m_graph->searchNode(GraphNodeType::OBJECT); break;
	}
	//std::cout << "**********************************" << std::endl;
}

std::string AI::getSourceName(SoundtypeAI type)
{
	try{
		return m_soundMap.at(type);
	}
	catch (const std::out_of_range& oor)
	{
		std::cout << "No Sound with this type was set!" << std::endl;
		return "oor";
	}

}

void AI::setSourceName(SoundtypeAI type, std::string sourceName, const char* filepath)
{
	m_sfh->generateSource(sourceName, glm::vec3(m_position), filepath);
	m_soundMap.insert(std::pair<SoundtypeAI, std::string>(type, sourceName));
}

void AI::updateSourcesInMap()
{
	for (std::map<SoundtypeAI, std::string>::iterator i = m_soundMap.begin(); i != m_soundMap.end(); ++i)
	{
		m_sfh->updateSourcePosition(i->second, glm::vec3(m_position));
	}
}

void AI::stopSourcesInMap(){
	for (std::map<SoundtypeAI, std::string>::iterator i = m_soundMap.begin(); i != m_soundMap.end(); ++i)
	{
		if(i->first != DEATHSOUND_AI)
			m_sfh->stopSource(i->second);
	}
}

void AI::setHasDied(bool b){
	m_hasDied = b;
}

bool AI::hasDied()
{
	return m_hasDied;
}

float AI::getPhi(){
	return m_phi;
}

AntType AI::getAntType(){
	return m_antType;
}