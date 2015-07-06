#include "GeKo_Gameplay/Questsystem/QuestHandler_CVTag.h"

QuestHandler_CVTag::QuestHandler_CVTag(){
	m_questGraph = new QuestGraph();
}

QuestHandler_CVTag::~QuestHandler_CVTag(){}

void QuestHandler_CVTag::generateQuests(QuestObserver* qO, std::vector<glm::vec3> flowerPos){
	//Create Quests
	Quest* root = new Quest(0);
	Quest* questWorker = new Quest(1);
	Quest* questGuard = new Quest(2);
	Quest* questQueen = new Quest(3);
	Quest* questBranche = new Quest(4);
	Quest* questFire = new Quest(5);
	Quest* questMoreFire = new Quest(6);
	Quest* questCoin = new Quest(7);
	Quest* questFlower = new Quest(8);

	questWorker->setDescription("- Kill and eat ten worker-ants. \n A worker-ant has a black-colored skin.");
	questGuard->setDescription("- Kill and eat five guard-ants. \n A guard-ant has a red-colored skin.");
	questQueen->setDescription("- Kill and eat one queen-ant. \n The queen-ant has a gold-colored skin.");
	questBranche->setDescription("- Collect one branche. \n (You can earn branches from trees with bushes.)");
	questFire->setDescription("- Find a fireplace and set a fire there. \n (Press F to set a fire.)");
	questMoreFire->setDescription("- Find more branches \n and set ten fire at a fireplace.");
	questCoin->setDescription("- Find three coins anywhere on the terrain.");
	questFlower->setDescription("- Find beautiful flowers anywhere on the terrain.");

	//Create Goal and add to Quest

	Goal_Kill* killWorker = new Goal_Kill(1);
	Goal_Kill* killGuard = new Goal_Kill(2);
	Goal_Kill* killQueen = new Goal_Kill(3);
	Goal_Eaten* eatWorker = new Goal_Eaten(4);
	Goal_Eaten* eatGuard = new Goal_Eaten(5);
	Goal_Eaten* eatQueen = new Goal_Eaten(6);
	Goal_Collect* collectBranch = new Goal_Collect(7);
	Goal_Action* setFire = new Goal_Action(8, GoalType::ACTION_FIRE);
	Goal_Action* setMoreFire = new Goal_Action(9, GoalType::ACTION_FIRE);
	Goal_Action* collectCoin = new Goal_Action(10, GoalType::ACTION_COIN);
	Goal_Discover* findFlowers = new Goal_Discover(11);

	killWorker->setAiTypeId(ObjectType::ANT);
	eatWorker->setAiTypeId(ObjectType::ANT);
	killGuard->setAiTypeId(ObjectType::ANT);
	eatGuard->setAiTypeId(ObjectType::ANT);
	killQueen->setAiTypeId(ObjectType::ANT);
	eatQueen->setAiTypeId(ObjectType::ANT);
	collectBranch->setItemType(ItemType::BRANCH);
	findFlowers->setPositions(flowerPos);

	killWorker->setGoalCount(10);
	eatWorker->setGoalCount(10);
	killGuard->setGoalCount(5);
	eatGuard->setGoalCount(5);
	killQueen->setGoalCount(1);
	eatQueen->setGoalCount(1);
	collectBranch->setGoalCount(1);
	setFire->setGoalCount(2);
	setMoreFire->setGoalCount(20); //in Wirklichkeit sind es 10, weil pro F-Taste zwei Feuer gesetzt werden
	//TODO: Prüfen, warum am Anfang alle 6 coins auf einer Stelle sind
	collectCoin->setGoalCount(8); //Der erste coin sind 6 auf einmal, deswegen 5+CoalCount
	findFlowers->setGoalCount(10);

	questWorker->addGoal(killWorker);
	questWorker->addGoal(eatWorker);
	questGuard->addGoal(killGuard);
	questGuard->addGoal(eatGuard);
	questQueen->addGoal(killQueen);
	questQueen->addGoal(eatQueen);
	questBranche->addGoal(collectBranch);
	questFire->addGoal(setFire);
	questMoreFire->addGoal(setMoreFire);
	questCoin->addGoal(collectCoin);
	questFlower->addGoal(findFlowers);


	//Create Reward and add to quest

	ExpReward* workerReward = new ExpReward(1);
	workerReward->setExp(15);
	ExpReward* guardReward = new ExpReward(2);
	guardReward->setExp(50);
	ExpReward* queenReward = new ExpReward(3);
	queenReward->setExp(100);
	ExpReward* branchReward = new ExpReward(4);
	branchReward->setExp(5);
	ExpReward* fireReward = new ExpReward(5);
	fireReward->setExp(10);
	ExpReward* moreFireReward = new ExpReward(6);
	moreFireReward->setExp(30);
	ExpReward* coinReward = new ExpReward(7);
	coinReward->setExp(5);
	ExpReward* flowerReward = new ExpReward(8);
	flowerReward->setExp(5);


	questWorker->addReward(workerReward);
	questGuard->addReward(guardReward);
	questQueen->addReward(queenReward);
	questBranche->addReward(branchReward);
	questFire->addReward(fireReward);
	questMoreFire->addReward(moreFireReward);
	questCoin->addReward(coinReward);
	questFlower->addReward(flowerReward);

	//Create QuestGraphNodes and add it to the QuestGraph
	QuestGraphNode* rootNode = new QuestGraphNode();
	rootNode->setQuest(root);
	m_questGraph->addNode(rootNode);
	questWorker->setActive(true);

	QuestGraphNode* workerNode = new QuestGraphNode();
	workerNode->setQuest(questWorker);
	workerNode->setParent(rootNode);
	m_questGraph->addNode(workerNode);

	QuestGraphNode* guardNode = new QuestGraphNode();
	guardNode->setQuest(questGuard);
	guardNode->setParent(workerNode);
	m_questGraph->addNode(guardNode);

	QuestGraphNode* queenNode = new QuestGraphNode();
	queenNode->setQuest(questQueen);
	queenNode->setParent(guardNode);
	m_questGraph->addNode(queenNode);

	QuestGraphNode* brancheNode = new QuestGraphNode();
	brancheNode->setQuest(questBranche);
	brancheNode->setParent(rootNode);
	m_questGraph->addNode(brancheNode);

	QuestGraphNode* fireNode = new QuestGraphNode();
	fireNode->setQuest(questFire);
	fireNode->setParent(brancheNode);
	m_questGraph->addNode(fireNode);

	QuestGraphNode* moreFireNode = new QuestGraphNode();
	moreFireNode->setQuest(questMoreFire);
	moreFireNode->setParent(fireNode);
	m_questGraph->addNode(moreFireNode);
	
	QuestGraphNode* coinNode = new QuestGraphNode();
	coinNode->setQuest(questCoin);
	coinNode->setParent(rootNode);
	m_questGraph->addNode(coinNode);

	QuestGraphNode* flowerNode = new QuestGraphNode();
	flowerNode->setQuest(questFlower);
	flowerNode->setParent(rootNode);
	m_questGraph->addNode(flowerNode);

	//Push Quest to Questlog
	addQuest(root);
	addQuest(questWorker);
	addQuest(questGuard);
	addQuest(questQueen);
	addQuest(questBranche);
	addQuest(questFire);
	addQuest(questMoreFire);
	addQuest(questCoin);
	addQuest(questFlower);


	//Add Observer to Quest
	for (int i = 0; i < m_questLog.size(); i++){
		m_questLog.at(i)->addObserver(qO);
		for (int j = 0; j < m_questLog.at(i)->getGoals().size(); j++){
			m_questLog.at(i)->getGoals().at(j)->addObserver(qO);
		}
	}

	//Cheat
	root->isFinished();
}