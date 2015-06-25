#pragma once
#include "GeKo_Gameplay/AI_Pathfinding/GraphNode.h"
#include "GeKo_Gameplay/AI_Pathfinding/Algorithm.h"
#include "GeKo_Gameplay/AI_Pathfinding/AStarNode.h"
#include "GeKo_Gameplay/AI_Pathfinding/AStarAlgorithm.h"
#include "GraphNodeType.h"
#include <sstream>
#include <vector>

/** The Graph-class provides a "bucket" for GraphNodes, which can be added to the vector-list of the Graph. A Graph represents one closed path-system on which an AI-Unit can run.
For example one Graph could represent the search for food or the area, which the AI should protect.*/

template<class T, class A>

class Graph
{
public: 
	///The constructor for a graph
	/**A Player-Node will be set automatically. The position of this fake-player is (0,0) and will be
	updated by the AI and observer classes*/
	Graph(){
		AStarNode* defaultNode = new AStarNode();

		AStarNode* player = new AStarNode("Player",defaultNode);
		player->setPosition(glm::vec3(0.0));
		player->setNodeType(GraphNodeType::OBJECT);
		player->setVisitor(defaultNode);
		player->setDefault(defaultNode);
		addGraphNode(player);
	}

	~Graph(){}
	
	///Adds a GraphNode-Object to the vector-list m_nodes
	/**/
	 void addGraphNode(T* node)
	 {
		 m_nodes.push_back(node);
	 }
	 
	 ///Returns the vector-list m_nodes completely
	 /**/
	 std::vector<T*>* getGraph()
	 {
		 return &m_nodes;
	 }

	 ///Returns a Node, which can be found with the help of its name
	 /**/
	 T* searchNode(std::string name)
	 {
		 T* returnNode;
		 for (T* s: m_nodes)
		 {
			 if (s->getName() == name)
			 {
				 returnNode = s;
			 }
		 }
		 return returnNode;
	 }

	 ///Returns a Node, which can be found with the help of its type
	 /**/
	 T* searchNode(GraphNodeType type)
	 {
		 T* returnNode;
		 for (T* s : m_nodes)
		 {
			 if (s->getNodeType() == type)
			 {
				 returnNode = s;
			 }
		 }
		 return returnNode;
	 }

	 ///Sets m_algorithm to a specific Algorithm
	 /**/
	 void setAlgorithm(A* algorithm)
	 {
		 m_algorithm = algorithm;
	 }

	 ///Returns m_algorithm
	 /**/
	 A* getAlgorithm()
	 {
		 return m_algorithm;
	 }

	 ///A Reset method to start the Algorithm on a clear Graph
	 /**The Visitor will be set back to default, the distance travelled will be set to 0 and the temporary Distance travelled 
	 will be set to 0, too. It is important to reset the whole Graph, if you use it more than once!*/
	 void resetAlgorithm(T* defaultNode)
	 {
		 for (T* s: m_nodes){
			s->setVisitor(s->getDefault());
			s->setDistanceTravelled(0);
			s->setTemporary(0);
		 }
	 }

	 ///The m_distanceToGoal of the Nodes will be calculated
	 /**The calculation takes the position of the target and substract the postion of the actual Node! This will be the
	 "air-line" distance!*/
	 void calculateDistanceToGoal(T* targetNode)
	 {
		 for (T* s : m_nodes)
		 {
			 s->setDistanceToGoal(glm::length(targetNode->getPosition() - s->getPosition()));
		 }
	 }

	 ///Creates an example Graph to see how it will work. 
	 void setExampleGraph1(glm::vec3 posPlayer)
	 {
		 //Node creation
		 AStarNode* defaultNode = new AStarNode();
		 AStarNode* spawn = new AStarNode("Spawn", defaultNode);
		 AStarNode* a = new AStarNode("A", defaultNode);
		 AStarNode* B = new AStarNode("B", defaultNode);
		 AStarNode* Food = new AStarNode("Food", defaultNode);
		 AStarNode* player = new AStarNode("Player", defaultNode);

		 spawn->setNodeType(GraphNodeType::HOME);
		 Food->setNodeType(GraphNodeType::FOOD);
		 player->setNodeType(GraphNodeType::OBJECT);

		 //Position of the nodes
		 spawn->setPosition(glm::vec3(0.0));
		 a->setPosition(glm::vec3(1.0));
		 B->setPosition(glm::vec3(3.0));
		 Food->setPosition(glm::vec3(10.0, 10.0, 0.0));
		 player->setPosition(posPlayer);

		 //path creation //
		 Path<AStarNode>* spawnA = new Path<AStarNode>(2, spawn, a);
		 Path<AStarNode>* Aspawn = new Path<AStarNode>(2, a, spawn);
		 a->addPath(Aspawn);
		 spawn->addPath(spawnA);

		 Path<AStarNode>* spawnB = new Path<AStarNode>(3, spawn, B);
		 Path<AStarNode>* Bspawn = new Path<AStarNode>(3, B, spawn);
		 B->addPath(Bspawn);
		 spawn->addPath(spawnB);

		 Path<AStarNode>* AFood = new Path<AStarNode>(2, a, Food);
		 Path<AStarNode>* FoodA = new Path<AStarNode>(2, Food, a);
		 a->addPath(AFood);
		 Food->addPath(FoodA);

		 Path<AStarNode>* BFood = new Path<AStarNode>(15, B, Food);
		 Path<AStarNode>* FoodB = new Path<AStarNode>(15, Food, B);
		 B->addPath(BFood);
		 Food->addPath(FoodB);

		 Path<AStarNode>* AB = new Path<AStarNode>(1, a, B);
		 Path<AStarNode>* BA = new Path<AStarNode>(5, B, a);
		 a->addPath(AB);
		 B->addPath(BA);

		 //Nodes will be added to the graph
		 addGraphNode(spawn);
		 addGraphNode(a);
		 addGraphNode(B);
		 addGraphNode(Food);
		 addGraphNode(player);

		 //The Algorithm will be Set
		 AStarAlgorithm* pathfindingFood = new AStarAlgorithm("pathfindingFood");
		 setAlgorithm(pathfindingFood);
		 calculateDistanceToGoal(Food);
	 }

	 ///An Example-Graph for a AI which will react afraid while the player is nearby
	 /***/
	 void setExampleAntAfraid(glm::vec3 posSpawn, glm::vec3 posFood, glm::vec3 posPlayer)
	 {
		 //This graph contains one anthome, a few foodNodes and some waypoints in between
		 AStarNode* defaultNode = new AStarNode();

		 AStarNode* nodeSpawn= new AStarNode("Spawn", defaultNode);
		 AStarNode* nodeB1 = new AStarNode("B1", defaultNode);
		 AStarNode* nodeB2 = new AStarNode("B2", defaultNode);
		 AStarNode* nodeW1 = new AStarNode("W1", defaultNode);
		 AStarNode* nodeW2 = new AStarNode("W2", defaultNode);
		 AStarNode* nodeE1 = new AStarNode("E1", defaultNode);
		 AStarNode* nodeE2 = new AStarNode("E2", defaultNode);
		 AStarNode* nodeFood = new AStarNode("Food", defaultNode);

		 nodeSpawn->setPosition(posSpawn);
		 nodeB1->setPosition(glm::vec3(6.0, 0.0, -3.0));
		 nodeB2->setPosition(glm::vec3(8.0, 0.0, -4.0));
		 nodeW1->setPosition(glm::vec3(6.0, 0.0, -9.0));
		 nodeW2->setPosition(glm::vec3(8.0, 0.0, -9.0));
		 nodeE1->setPosition(glm::vec3(7.0, 0.0, -12.0));
		 nodeE2->setPosition(glm::vec3(10.0, 0.0, -10.0));
		 nodeFood->setPosition(posFood);

		 nodeSpawn->setNodeType(GraphNodeType::HOME);
		 nodeFood->setNodeType(GraphNodeType::FOOD);
		 nodeB1->setNodeType(GraphNodeType::OTHER);
		 nodeB2->setNodeType(GraphNodeType::OTHER);
		 nodeW1->setNodeType(GraphNodeType::OTHER);
		 nodeW2->setNodeType(GraphNodeType::OTHER);
		 nodeE1->setNodeType(GraphNodeType::OTHER);
		 nodeE2->setNodeType(GraphNodeType::OTHER);

		 Path<AStarNode>* pathSB1 = new Path<AStarNode>(1, nodeSpawn, nodeB1);
		 Path<AStarNode>* pathSW1 = new Path<AStarNode>(1, nodeSpawn, nodeW1);
		 Path<AStarNode>* pathSE1 = new Path<AStarNode>(1, nodeSpawn, nodeE1);
		 nodeSpawn->addPath(pathSB1);
		 nodeSpawn->addPath(pathSW1);
		 nodeSpawn->addPath(pathSE1);

		 Path<AStarNode>* pathB1S = new Path<AStarNode>(1, nodeB1, nodeSpawn);
		 Path<AStarNode>* pathW1S = new Path<AStarNode>(1, nodeW1, nodeSpawn);
		 Path<AStarNode>* pathE1S = new Path<AStarNode>(1, nodeE1, nodeSpawn);
		 nodeB1->addPath(pathB1S);
		 nodeW1->addPath(pathW1S);
		 nodeE1->addPath(pathE1S);

		 Path<AStarNode>* pathB1B2 = new Path<AStarNode>(6, nodeB1, nodeB2);
		 Path<AStarNode>* pathW1W2 = new Path<AStarNode>(4, nodeW1, nodeW2);
		 Path<AStarNode>* pathE1E2 = new Path<AStarNode>(2, nodeE1, nodeE2);
		 nodeB1->addPath(pathB1B2);
		 nodeW1->addPath(pathW1W2);
		 nodeE1->addPath(pathE1E2);

		 Path<AStarNode>* pathB2B1 = new Path<AStarNode>(6, nodeB2, nodeB1);
		 Path<AStarNode>* pathW2W1 = new Path<AStarNode>(4, nodeW2, nodeW1);
		 Path<AStarNode>* pathE2E1 = new Path<AStarNode>(2, nodeE2, nodeE1);
		 nodeB2->addPath(pathB2B1);
		 nodeW2->addPath(pathW2W1);
		 nodeE2->addPath(pathE2E1);

		 Path<AStarNode>* pathB2Food = new Path<AStarNode>(1, nodeB2, nodeFood);
		 Path<AStarNode>* pathW2Food = new Path<AStarNode>(1, nodeW2, nodeFood);
		 Path<AStarNode>* pathE2Food = new Path<AStarNode>(1, nodeE2, nodeFood);
		 nodeB2->addPath(pathB2Food);
		 nodeW2->addPath(pathW2Food);
		 nodeE2->addPath(pathE2Food);

		 Path<AStarNode>* pathFoodB2 = new Path<AStarNode>(1, nodeFood, nodeB2);
		 Path<AStarNode>* pathFoodW2 = new Path<AStarNode>(1, nodeFood, nodeW2);
		 Path<AStarNode>* pathFoodE2 = new Path<AStarNode>(1, nodeFood, nodeE2);
		 nodeFood->addPath(pathFoodB2);
		 nodeFood->addPath(pathFoodW2);
		 nodeFood->addPath(pathFoodE2);

		 addGraphNode(nodeSpawn);
		 addGraphNode(nodeB1);
		 addGraphNode(nodeB2);
		 addGraphNode(nodeW1);
		 addGraphNode(nodeW2);
		 addGraphNode(nodeE1);
		 addGraphNode(nodeE2);
		 addGraphNode(nodeFood);

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeSpawn);
	 }

	 ///Another Example-Graph for a AI which will react afraid while the player is nearby
	 /***/
	 void setExampleAntAfraid2(glm::vec3 posSpawn, std::vector<std::vector<glm::vec3>> possFoods)
	 {
		 AStarNode* defaultNode = new AStarNode();

		 AStarNode* nodeSpawn = new AStarNode("Spawn", defaultNode, posSpawn, GraphNodeType::HOME);

		 for (int i = 0; i < possFoods.size(); i++){
			 generateForest(possFoods.at(i), nodeSpawn, defaultNode);
		 }
		 

		 addGraphNode(nodeSpawn);

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeSpawn);
	 }

	 ///An Example-Graph for a AI which will react aggresive while the player is nearby
	 /***/
	 void setExampleAntAggressiv(glm::vec3 posSpawn, int distance)
	 {
		//This example just contains the FoodNode on the position of the anthome and four nodes, which let the ant patrol around the anthome
		 AStarNode* defaultNode = new AStarNode();

		 AStarNode* nodeLO = new AStarNode("LO",defaultNode);
		 AStarNode* nodeLU = new AStarNode("LU", defaultNode);
		 AStarNode* nodeRO = new AStarNode("RO", defaultNode);
		 AStarNode* nodeRU = new AStarNode("RU", defaultNode);
		 AStarNode* nodeFood = new AStarNode("Food", defaultNode);

		 nodeLO->setPosition(glm::vec3(posSpawn.x - distance, 0.0, posSpawn.z + distance));
		 nodeLU->setPosition(glm::vec3(posSpawn.x - distance, 0.0, posSpawn.z - distance));
		 nodeRO->setPosition(glm::vec3(posSpawn.x + distance, 0.0, posSpawn.z + distance));
		 nodeRU->setPosition(glm::vec3(posSpawn.x + distance, 0.0, posSpawn.z - distance));
		 nodeFood->setPosition(posSpawn);

		 nodeFood->setNodeType(GraphNodeType::FOOD);
		 nodeLU->setNodeType(GraphNodeType::OTHER);
		 nodeLO->setNodeType(GraphNodeType::OTHER);
		 nodeRU->setNodeType(GraphNodeType::OTHER);
		 nodeRO->setNodeType(GraphNodeType::OTHER);

		 Path<AStarNode>* pathLORO = new Path<AStarNode>(1, nodeLO, nodeRO);
		 Path<AStarNode>* pathLOF = new Path<AStarNode>(1, nodeLO, nodeFood);
		 Path<AStarNode>* pathLOLU = new Path<AStarNode>(1, nodeLO, nodeLU);
		 nodeLO->addPath(pathLORO);
		 nodeLO->addPath(pathLOF);
		 nodeLO->addPath(pathLOLU);

		 Path<AStarNode>* pathLULO = new Path<AStarNode>(1, nodeLU, nodeLO);
		 Path<AStarNode>* pathLUF = new Path<AStarNode>(1, nodeLU, nodeFood);
		 Path<AStarNode>* pathLURU = new Path<AStarNode>(1, nodeLU, nodeRU);
		 nodeLU->addPath(pathLULO);
		 nodeLU->addPath(pathLUF);
		 nodeLU->addPath(pathLURU);

		 Path<AStarNode>* pathRULU = new Path<AStarNode>(6, nodeRU, nodeLU);
		 Path<AStarNode>* pathRUF = new Path<AStarNode>(4, nodeRU, nodeFood);
		 Path<AStarNode>* pathRURO = new Path<AStarNode>(2, nodeRU, nodeRO);
		 nodeRU->addPath(pathRULU);
		 nodeRU->addPath(pathRUF);
		 nodeRU->addPath(pathRURO);

		 Path<AStarNode>* pathROLO = new Path<AStarNode>(6, nodeRO, nodeLO);
		 Path<AStarNode>* pathROF = new Path<AStarNode>(4, nodeRO, nodeFood);
		 Path<AStarNode>* pathRORU = new Path<AStarNode>(2, nodeRO, nodeRU);
		 nodeRO->addPath(pathROLO);
		 nodeRO->addPath(pathROF);
		 nodeRO->addPath(pathRORU);

		 Path<AStarNode>* pathFLO = new Path<AStarNode>(1, nodeFood, nodeLO);
		 Path<AStarNode>* pathFLU = new Path<AStarNode>(1, nodeFood, nodeLU);
		 Path<AStarNode>* pathFRO = new Path<AStarNode>(1, nodeFood, nodeRO);
		 Path<AStarNode>* pathFRU = new Path<AStarNode>(1, nodeFood, nodeRU);
		 nodeFood->addPath(pathFLO);
		 nodeFood->addPath(pathFLU);
		 nodeFood->addPath(pathFRO);
		 nodeFood->addPath(pathFRU);

		 addGraphNode(nodeLU);
		 addGraphNode(nodeLO);
		 addGraphNode(nodeRO);
		 addGraphNode(nodeRU);
		 addGraphNode(nodeFood);

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeFood);
	 }

	 ///A method to create a Forest
	 /**This is just an example forest. It will contain paths and nodes for the trees.*/
	 void generateForest(std::vector<glm::vec3> treePositions,AStarNode *nodeSpawn, AStarNode *defaultNode){
		 //int i = 1;
		 std::stringstream name;
		 for (int i = 0; i < treePositions.size(); i++){
			 name << "Tree" << i;
			 AStarNode* tree = new AStarNode(name.str(), defaultNode, treePositions.at(i), GraphNodeType::FOOD);
			 Path<AStarNode>* pathSpawnTree = new Path<AStarNode>(1, nodeSpawn, tree);
			 nodeSpawn->addPath(pathSpawnTree);
			 Path<AStarNode>* pathTreeSpawn = new Path<AStarNode>(1, tree, nodeSpawn);
			 tree->addPath(pathTreeSpawn);
			 addGraphNode(tree);
			 name.str("");
		 }
	 }

	

protected:
	  std::vector<T*> m_nodes;

	  A* m_algorithm;
};