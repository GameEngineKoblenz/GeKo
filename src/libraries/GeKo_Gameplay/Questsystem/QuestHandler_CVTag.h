#pragma once

#include "GeKo_Gameplay/Questsystem/QuestHandler.h"

#include "GeKo_Gameplay/Questsystem/Goal_Kill.h"
#include "GeKo_Gameplay/Questsystem/Goal_Eaten.h"
#include "GeKo_Gameplay/Questsystem/Goal_Collect.h"
#include "GeKo_Gameplay/Questsystem/Goal_Action.h"
#include "GeKo_Gameplay/Questsystem/Goal_Discover.h"

#include "GeKo_Gameplay/Questsystem/ExpReward.h"

#include "GeKo_Gameplay/Observer/QuestObserver.h"
#include "GeKo_Gameplay/Observer/ObjectObserver.h"

class QuestHandler_CVTag : public QuestHandler
{
public: 
	QuestHandler_CVTag();
	~QuestHandler_CVTag();

	void generateQuests(QuestObserver* qO, std::vector<glm::vec3> flowerPos);

protected:

};