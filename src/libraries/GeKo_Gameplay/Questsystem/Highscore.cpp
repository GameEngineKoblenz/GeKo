#include "GeKo_Gameplay/Questsystem/Highscore.h"

Highscore::Highscore(){
	
	m_score = 0;
}

Highscore::~Highscore(){}

int Highscore::getScore()
{
	return m_score;
}

void Highscore::addScore(int score)
{
	m_score += score;
}

void Highscore::resetScore()
{
	m_score = 0;
}