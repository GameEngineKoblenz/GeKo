#pragma once

#include "GeKo_Gameplay/Questsystem/QuestHandler.h"

#include "GeKo_Gameplay/Questsystem/Goal_Kill.h"
#include "GeKo_Gameplay/Questsystem/Goal_Eaten.h"

#include "GeKo_Gameplay/Questsystem/ExpReward.h"

#include "GeKo_Gameplay/Observer/QuestObserver.h"

class QuestHandler_CVTag : public QuestHandler
{
public: 
	QuestHandler_CVTag();
	~QuestHandler_CVTag();

	void generateQuests();
	void addObserver(QuestObserver* qO);

protected:

};