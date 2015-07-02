#include "DecisionTree.h"

DecisionTree::DecisionTree(){}

DecisionTree::DecisionTree(DecisionTreeNode* root){
	m_rootNode = root;
	m_currentNode = root;
}

DecisionTree::~DecisionTree(){

}

TreeOutput DecisionTree::decide(std::vector<std::pair<States, bool>> states){
	//Check if current node is not a leaf
	if (!m_currentNode->getIsLeaf()){
		bool stateCondition = NULL;
		if (m_currentNode->getStateID() != States::DEFAULTSTATE){
			stateCondition = getStates(states, m_currentNode->getStateID());
		}
		bool leftCondition = NULL;
		if (m_currentNode->getLeftNode()->getCondition() != NULL){
			leftCondition = m_currentNode->getLeftNode()->getCondition();
		}
	
		bool rightCondition = NULL;
		if (m_currentNode->getRightNode()->getCondition() != NULL){
			rightCondition = m_currentNode->getRightNode()->getCondition();
		}
		if (stateCondition == leftCondition){
			m_currentNode = m_currentNode->getLeftNode();
			return decide(states);
		}
		else if (stateCondition == rightCondition){
			m_currentNode = m_currentNode->getRightNode();
			return decide(states);
		}
		else {
		}
	}
		TreeOutput target = m_currentNode->getTarget();
		m_currentNode = m_rootNode;		
		return target;
}

void DecisionTree::setRootNode(DecisionTreeNode &root){
	m_rootNode = &root;
}

DecisionTreeNode* DecisionTree::getRootNode(){
	return m_rootNode;
}

void DecisionTree::setCurrentNode(DecisionTreeNode &current){
	m_currentNode = &current;
}

DecisionTreeNode* DecisionTree::getCurrentNode(){
	return m_currentNode;
}

bool DecisionTree::getStates(std::vector<std::pair<States, bool>> list, States state){
	for (int i = 0; i < list.size(); i++){
		if (list.at(i).first == state) {
			return list.at(i).second;
		}
	}
	return NULL;
}

void DecisionTree::setStates(std::vector<std::pair<States, bool>> list, States state, bool b){
	for (int i = 0; i < list.size(); i++){
		if (list.at(i).first == state){
			list.at(i).second = b;
		}
	}
}

void DecisionTree::setAntTreeAfraid(){
	DecisionTreeNode defaultNode;
	m_rootNode = new DecisionTreeNode(1, &defaultNode, "root -> State:VIEW");
	DecisionTreeNode* lView = new DecisionTreeNode(2, &defaultNode, "lView -> T:HOME");
	DecisionTreeNode* rView = new DecisionTreeNode(3, &defaultNode, "rView -> State:HUNGER");
	DecisionTreeNode* lHunger = new DecisionTreeNode(4, &defaultNode, "lHunger -> T:FOOD");
	DecisionTreeNode* rHunger = new DecisionTreeNode(5, &defaultNode, "rHunger -> T:HOME");

	m_rootNode->setStateID(States::VIEW);

	m_rootNode->setLeftNode(lView);
	lView->setCondition(true);
	lView->setLeaf(true);
	lView->setTarget(TreeOutput::HOME); 

	m_rootNode->setRightNode(rView);
	rView->setCondition(false);
	rView->setStateID(States::HUNGER);

	rView->setLeftNode(lHunger);
	lHunger->setCondition(true);
	lHunger->setLeaf(true);
	lHunger->setTarget(TreeOutput::FOOD);

	rView->setRightNode(rHunger);
	rHunger->setCondition(false);
	rHunger->setLeaf(true);
	rHunger->setTarget(TreeOutput::HOME);

	m_currentNode = m_rootNode;
}

void DecisionTree::setAntTreeAggressiv(){
	DecisionTreeNode defaultNode;
	m_rootNode = new DecisionTreeNode(1, &defaultNode, "root");
	DecisionTreeNode* lView = new DecisionTreeNode(2, &defaultNode, "lView");
	DecisionTreeNode* rView = new DecisionTreeNode(3, &defaultNode, "rView");
	DecisionTreeNode* lHunger = new DecisionTreeNode(4, &defaultNode, "lHunger");
	DecisionTreeNode* rHunger = new DecisionTreeNode(5, &defaultNode, "rHunger");

	m_rootNode->setStateID(States::VIEW);

	m_rootNode->setLeftNode(lView);
	lView->setCondition(true);
	lView->setLeaf(true);
	lView->setTarget(TreeOutput::PLAYER);

	m_rootNode->setRightNode(rView);
	rView->setCondition(false);
	rView->setStateID(States::HUNGER);

	rView->setLeftNode(lHunger);
	lHunger->setCondition(true);
	lHunger->setLeaf(true);
	lHunger->setTarget(TreeOutput::FOOD);

	rView->setRightNode(rHunger);
	rHunger->setCondition(false);
	rHunger->setLeaf(true);
	rHunger->setTarget(TreeOutput::PATROL);

	m_currentNode = m_rootNode;
}