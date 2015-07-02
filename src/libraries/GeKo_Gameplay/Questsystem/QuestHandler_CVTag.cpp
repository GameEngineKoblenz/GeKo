#include "GeKo_Gameplay/Questsystem/QuestHandler_CVTag.h"

QuestHandler_CVTag::QuestHandler_CVTag(){
	m_questGraph = new QuestGraph();
}

QuestHandler_CVTag::~QuestHandler_CVTag(){}

void QuestHandler_CVTag::generateQuests(){
	//Create Quests
	Quest* questWorker = new Quest(1);
	Quest* questGuard = new Quest(2);
	Quest* questQueen = new Quest(3);
	Quest* questBranches = new Quest(4);

	questWorker->setDescription("Kill and eat ten worker-ants.");
	questGuard->setDescription("Kill and eat five guard-ants.");
	questQueen->setDescription("Kill and eat one queen-ant.");
	questBranches->setDescription("Collect ");

	Goal_Kill* killWorker = new Goal_Kill(1);
	Goal_Kill* killGuard = new Goal_Kill(2);
	Goal_Kill* killQueen = new Goal_Kill(3);
	Goal_Eaten* eatWorker = new Goal_Eaten(4);
	Goal_Eaten* eatGuard = new Goal_Eaten(5);
	Goal_Eaten* eatQueen = new Goal_Eaten(6);

	questWorker->addGoal(killWorker);
	questWorker->addGoal(eatWorker);
	questGuard->addGoal(killGuard);
	questGuard->addGoal(eatGuard);
	questQueen->addGoal(killQueen);
	questQueen->addGoal(eatQueen);

	killWorker->setGoalCount(10);
	eatWorker->setGoalCount(10);
	killGuard->setGoalCount(5);
	eatGuard->setGoalCount(5);
	killQueen->setGoalCount(1);
	eatQueen->setGoalCount(1);

	killWorker->setAiTypeId(ObjectType::ANT);
	eatWorker->setAiTypeId(ObjectType::ANT);
	killGuard->setAiTypeId(ObjectType::ANT);
	eatGuard->setAiTypeId(ObjectType::ANT);
	killQueen->setAiTypeId(ObjectType::ANT);
	eatQueen->setAiTypeId(ObjectType::ANT);

	ExpReward* workerReward = new ExpReward(1);
	workerReward->setExp(10);
	ExpReward* guardReward = new ExpReward(2);
	guardReward->setExp(50);
	ExpReward* queenReward = new ExpReward(3);
	queenReward->setExp(100);

	questWorker->addReward(workerReward);
	questGuard->addReward(guardReward);
	questQueen->addReward(queenReward);


	//Create QuestGraphNodes and QuestGraph
	QuestGraphNode* workerNode = new QuestGraphNode();
	workerNode->setQuest(questWorker);
	m_questGraph->addNode(workerNode);
	questWorker->setActive(true);

	QuestGraphNode* guardNode = new QuestGraphNode();
	guardNode->setQuest(questGuard);
	guardNode->setParent(workerNode);
	m_questGraph->addNode(guardNode);

	QuestGraphNode* queenNode = new QuestGraphNode();
	queenNode->setQuest(questQueen);
	queenNode->setParent(guardNode);
	m_questGraph->addNode(queenNode);
	

	//Push Quest to Questlog
	m_questLog.push_back(questWorker);
	m_questLog.push_back(questGuard);
	m_questLog.push_back(questQueen);
}

void QuestHandler_CVTag::addObserver(QuestObserver* qO){
	for (int i = 0; i < m_questLog.size(); i++){
		m_questLog.at(i)->addObserver(qO);
	}
}