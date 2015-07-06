#include "GeKo_Gameplay/Questsystem/QuestHandler_CVTag.h"

QuestHandler_CVTag::QuestHandler_CVTag(){
	m_questGraph = new QuestGraph();
}

QuestHandler_CVTag::~QuestHandler_CVTag(){}

void QuestHandler_CVTag::generateQuests(QuestObserver* qO){
	//Create Quests
	Quest* root = new Quest(0);
	Quest* questWorker = new Quest(1);
	Quest* questGuard = new Quest(2);
	Quest* questQueen = new Quest(3);
	Quest* questBranche = new Quest(4);
	Quest* questFire = new Quest(5);

	questWorker->setDescription("Kill and eat ten worker-ants. A worker-ant has a black-colored skin.");
	questGuard->setDescription("Kill and eat five guard-ants. A guard-ant has a red-colored skin.");
	questQueen->setDescription("Kill and eat one queen-ant. The queen-ant has a gold-colored skin.");
	questBranche->setDescription("Collect one branche. You can earn branches from trees with bushes. (Run Forest run.)");
	questFire->setDescription("Find a fireplace and there set a fire.");

	Goal_Kill* killWorker = new Goal_Kill(1);
	Goal_Kill* killGuard = new Goal_Kill(2);
	Goal_Kill* killQueen = new Goal_Kill(3);
	Goal_Eaten* eatWorker = new Goal_Eaten(4);
	Goal_Eaten* eatGuard = new Goal_Eaten(5);
	Goal_Eaten* eatQueen = new Goal_Eaten(6);
	Goal_Collect* collectBranch = new Goal_Collect(7);
	Goal_Action* setFire = new Goal_Action(8, GoalType::ACTION_FIRE);

	questWorker->addGoal(killWorker);
	questWorker->addGoal(eatWorker);
	questGuard->addGoal(killGuard);
	questGuard->addGoal(eatGuard);
	questQueen->addGoal(killQueen);
	questQueen->addGoal(eatQueen);
	questBranche->addGoal(collectBranch);
	questFire->addGoal(setFire);

	killWorker->setGoalCount(10);
	eatWorker->setGoalCount(10);
	killGuard->setGoalCount(5);
	eatGuard->setGoalCount(5);
	killQueen->setGoalCount(1);
	eatQueen->setGoalCount(1);
	collectBranch->setGoalCount(2);
	setFire->setGoalCount(1);

	killWorker->setAiTypeId(ObjectType::ANT);
	eatWorker->setAiTypeId(ObjectType::ANT);
	killGuard->setAiTypeId(ObjectType::ANT);
	eatGuard->setAiTypeId(ObjectType::ANT);
	killQueen->setAiTypeId(ObjectType::ANT);
	eatQueen->setAiTypeId(ObjectType::ANT);
	collectBranch->setItemType(ItemType::BRANCH);

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


	questWorker->addReward(workerReward);
	questGuard->addReward(guardReward);
	questQueen->addReward(queenReward);
	questBranche->addReward(branchReward);
	questFire->addReward(fireReward);


	//Create QuestGraphNodes and QuestGraph
	QuestGraphNode* rootNode = new QuestGraphNode();
	rootNode->setQuest(root);
	m_questGraph->addNode(rootNode);
	questWorker->setActive(true);

	QuestGraphNode* workerNode = new QuestGraphNode();
	workerNode->setQuest(questWorker);
	workerNode->setParent(rootNode);
	questWorker->setActive(false);
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
	questBranche->setActive(false);
	m_questGraph->addNode(brancheNode);


	QuestGraphNode* fireNode = new QuestGraphNode();
	fireNode->setQuest(questFire);
	fireNode->setParent(brancheNode);
	m_questGraph->addNode(fireNode);
	

	//Push Quest to Questlog
	/*m_questLog.push_back(root);
	m_questLog.push_back(questWorker);
	m_questLog.push_back(questGuard);
	m_questLog.push_back(questQueen);
	m_questLog.push_back(questBranche);
	m_questLog.push_back(questFire);*/

	addQuest(root);
	addQuest(questWorker);
	addQuest(questGuard);
	addQuest(questQueen);
	addQuest(questBranche);
	addQuest(questFire);


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