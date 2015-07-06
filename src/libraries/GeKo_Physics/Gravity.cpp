#include "Gravity.h"

Gravity::Gravity()
{
	m_gravity = glm::vec3(0.0, -0.1, 0.0);
}


Gravity::~Gravity()
{

}

glm::mat4 Gravity::addGravity(glm::mat4 modelMatrix)
{
	glm::mat4 returnMatrix = glm::translate(modelMatrix, m_gravity);
	return returnMatrix;  
}

glm::vec3 Gravity::getGravity()
{
	return m_gravity;
}

void Gravity::setGravity(glm::vec3 newGravity)
{
	m_gravity = newGravity;
}

