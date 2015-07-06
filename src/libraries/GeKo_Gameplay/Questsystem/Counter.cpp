#include "GeKo_Gameplay/Questsystem/Counter.h"

Counter::Counter(){
	
	m_started = false;
	m_finished = false;
	m_frame = 0;
}

Counter::Counter(float time){
	m_time = time;
	m_started = false;
	m_finished = false;
	m_frame = 0;
}

Counter::~Counter(){}


void Counter::setTime(float time){
	m_time = time;
}
float Counter::getTime(){
	return m_time;
}

void Counter::start(){
	m_started = true;
	m_finished = false;
}

void Counter::update()
{

	if (m_started && !m_finished)
	{


		if (m_frame % 60 == 0)
		{
			m_frame = 0;
			m_time--;
		}
		else{
			m_frame++;
		}

		if (m_time <= 0)
		{
			end();
		}
	}
}
void Counter::end(){
	m_started = false;
	m_finished = true;
	std::cout << "TIME IS OVER!" << std::endl;
}