#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <string>

/**A class which provides a countable highscore*/
class Highscore 
{
public: 

	Highscore();
	~Highscore();

	int getScore();
	void addScore(int score);
	
	void resetScore();
protected:
	int m_score;
};